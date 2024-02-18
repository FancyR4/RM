#include "GimbalTask.h"
//#include "IMUTask.h"
#include "RemoteTask.h"
#include "can.h"

#include "pid.h"
#include "user_lib.h"

#include "INS_Task.h"

#include "FreeRTOS.h"
#include "task.h"
#include "FreeRTOSConfig.h"

//P:PITCH
#define P 0
#define GimbalPitchMiddle 4.9
//Y:YAW
#define GimbalYawMiddle
#define Y 1

//#include "SysCtrl.h"

GIMBAL_st GIMBAL[2];

#define PitchAngle_OFFSET 6300
#define YawAngle_OFFSET   4100

//机械控制
static float PitchRCValue;
static float YawRCValue;

//IMU控制下
static float PitchIMUValue;
static float YawIMUValue;

//extern u8 InitFlag;

extern volatile  RC_Ctl_t RC_Ctl;

extern CanRx_Gimbal_st Gimbal_RX;

extern GimbalPidInformation HardPitch;
extern GimbalPidInformation Yaw;

//extern fp32 INS_Angle[3];

//遥控模式下比例系数,控制摇杆响应速度
static float kRc_RC_Pitch = 0.001;
static float kRc_RC_Yaw = 0.01;
//按键模式
static float kRc_PC_Pitch = 0.001;
static float kRc_PC_Yaw = 0.01;

void GIMBAL_Init(GIMBAL_st *gimbal)
{
  gimbal[P].Gimbal_Angle_Target = PitchAngle_OFFSET;
 	gimbal[Y].Gimbal_Angle_Target = YawAngle_OFFSET;
	
	gimbal[Y].Gimbal_IMU_Angle_Data  = get_INS_angle_point() + 0;
	gimbal[P].Gimbal_IMU_Angle_Data  = get_INS_angle_point() + 1;
	gimbal[Y].Gimbal_IMU_Aspeed_Data = get_MPU6500_Gyro_Data_Point() + 0;
	gimbal[P].Gimbal_IMU_Aspeed_Data = get_MPU6500_Gyro_Data_Point() + 1;
}

void GIMBALOutClean(void)
{
	//清除电机电流电机无力
	GIMBAL[P].Gimbal_Final_Out = 0;
	GIMBAL[Y].Gimbal_Final_Out = 0;
	GIMBAL_CanbusCtrlMotors(GIMBAL);
}

static int16_t Mode1Flag;//归中标志位
//int16_t P1;
//int16_t P2;
int16_t Y1;
int16_t Y2;

int16_t lastkey;
void GIMBAL_RC_Ctrl(GIMBAL_st *gimbal)
{
	PitchRCValue = kRc_RC_Pitch * float_map(0,660,0,PI,(float)abs(RC_CH1_PITCH_OFFSET));
	PitchRCValue = RC_CH1_PITCH_OFFSET > 0 ? PitchRCValue : -PitchRCValue;
	YawRCValue   = kRc_RC_Yaw   * float_map(0,660,0,PI,(float)abs(RC_CH0_YAW_OFFSET));
	YawRCValue = RC_CH0_YAW_OFFSET > 0 ? YawRCValue : -YawRCValue;
	
	//遥控器普通模式：云台单独运动调试（每次打开该模式会归中）
	if (SWITCH2_UP)
	{
	 //需要归中一次
	 if(Mode1Flag == 0)
		 {
			gimbal[P].Gimbal_Angle_Target = 5.0f;//float_map(0,8191,0,2*PI,PitchAngle_OFFSET); 

			gimbal[Y].Gimbal_Angle_Target = float_map(0,8191,0,2*PI,YawAngle_OFFSET);
			 
			Y1 = float_map(0,8191,0,2*PI,1200);
			Y2 = float_map(0,8191,0,2*PI,6800);
			//P1 = float_map(0,8191,0,2*PI,6000);
			//P2 = float_map(0,8191,0,2*PI,7200);
			 
			gimbal[P].Gimbal_Final_Out = 0;
			gimbal[Y].Gimbal_Final_Out = 0;
			 
			Mode1Flag = 1;
		 }
	 else
	 { 
	 gimbal[P].Gimbal_Angle_Target += PitchRCValue;//gimbal[P].Gimbal_Angle_Target + (kRc_RC_Pitch * (PitchRCValue/4096)*PI);
	 gimbal[Y].Gimbal_Angle_Target += YawRCValue;//gimbal[Y].Gimbal_Angle_Target + (kRc_RC_Yaw * (YawRCValue/4096)*PI); 		
	 
	 gimbal[P].Gimbal_Angle_Target = float_constrain(gimbal[P].Gimbal_Angle_Target, 4.70, 5.35);	
	 gimbal[Y].Gimbal_Angle_Target = float_constrain(gimbal[Y].Gimbal_Angle_Target, Y1, Y2);
	 lastkey = SWITCH2_UP;
	 }		 
	}
	else Mode1Flag = 0;
	
	//YAW轴跟随底盘方向（只有PITCH轴可以控制）
	if (SWITCH2_MID)    
	{
  //gimbal[P].Gimbal_Angle_Target = (kRc_RC_Pitch * PitchRCValue)+YawAngle_OFFSET;
  //gimbal[Y].Gimbal_Angle_Target = RC_CH0_YAW_OFFSET;
	 lastkey = SWITCH2_UP;		
	}

/*****************开启陀螺仪控制云台Yaw轴模式*****************/
  if (SWITCH2_DOWN)
	{
	 //转换控制模式云台输出清零	
	 if(lastkey != SWITCH2_DOWN)
	 {
	  GIMBALOutClean();
		//标定初始值
		gimbal[Y].Gimbal_Angle_Target = 0;
	 }
	 
	 gimbal[Y].GIMBALMode = ApartGimbal;
	 gimbal[P].GIMBALMode = ChassisCtrlGimbal;
	 //Pitch仍由电机传感器控制
	 gimbal[P].Gimbal_Angle_Target += PitchRCValue;
	 //gimbal[P].Gimbal_Angle_Target = float_constrain(gimbal[P].Gimbal_Angle_Target, P1, P2);
		//Yaw陀螺仪控制(手动小陀螺)
	 //YawRCValue = float_map();	
	 gimbal[Y].Gimbal_Angle_Target += YawIMUValue;
	 //gimbal[Y].Gimbal_Angle_Target = float_constrain(gimbal[Y].Gimbal_Angle_Target , , );
	 lastkey = SWITCH2_UP;		
	}
	 

}

/**
  * @brief  PC端控制
  * @param  GIMBAL_st *gimbal kRc_PC_Pitch kRc_PC_Yaw
  * @note   
  * @retval 
  */
void GIMBAL_PC_Ctrl(GIMBAL_st *gimbal)
{
	//鼠标直接控制
	PitchRCValue = kRc_PC_Pitch * float_map(0,MOUSE_MAX,0,PI,(float)abs(MOUSE_X));
	PitchRCValue = MOUSE_X > 0 ? PitchRCValue : -PitchRCValue;
	YawRCValue = kRc_PC_Yaw * float_map(0,MOUSE_MAX,0,PI,(float)abs(MOUSE_Y));
	YawRCValue = MOUSE_X > 0 ? YawRCValue : -YawRCValue;

	//把角度进行相对转换
	gimbal[P].Gimbal_Angle_Target += PitchRCValue;
	gimbal[P].Gimbal_Angle_Target = AngleRelativeTrans(gimbal[P].Gimbal_Angle_Target,0); 
	gimbal[Y].Gimbal_Angle_Target += YawRCValue;
	gimbal[Y].Gimbal_Angle_Target = AngleRelativeTrans(gimbal[Y].Gimbal_Angle_Target,0);
}

/**
  * @brief  将CAN原始数据转换
  * @param  更新方式：1.直接转化；2.转化为其它格式数据
  * @note   
  * @retval 
  */
void GIMBAL_RX_Pool(CanRx_Gimbal_st *can, GIMBAL_st *gimbal, RXDataType_en RXDataType, AxisSelect_en AxisSelect)
{
 if((RXDataType == Origin) && (AxisSelect == PITCH))
 {
 gimbal[AxisSelect].Angle_Pool = can->Gimbal_Pitch_Angle_RX;
 gimbal[AxisSelect].Speed_Pool = can->Gimbal_Pitch_Speed_RX;
 }

 if((RXDataType == PIType) && (AxisSelect == PITCH))
 { 
 gimbal[AxisSelect].Angle_Pool = float_map(0,8191,0,2*PI,(float)can->Gimbal_Pitch_Angle_RX);	 
 gimbal[AxisSelect].Speed_Pool = can->Gimbal_Pitch_Speed_RX*2*PI;	
 }

 if((RXDataType == Origin) && (AxisSelect == YAW))
 { 
 gimbal[AxisSelect].Angle_Pool = can->Gimbal_Yaw_Angle_RX;
 gimbal[AxisSelect].Speed_Pool = can->Gimbal_Yaw_Speed_RX;
 }
 
 if((RXDataType == PIType) && (AxisSelect == YAW))
 { 	
 //gimbal[AxisSelect].Angle_Pool = can->Gimbal_Yaw_Angle_RX;
 gimbal[AxisSelect].Angle_Pool = float_map(0,8191,0,2*PI,(float)can->Gimbal_Yaw_Angle_RX);	 
 //gimbal[AxisSelect].Angle_Pool = ((gimbal[AxisSelect].Angle_Pool-4096)/4096)*PI;
 gimbal[AxisSelect].Speed_Pool = can->Gimbal_Yaw_Speed_RX*2*PI;
 }
 
 //gimbal[AxisSelect].Angle_Pool = float_map(0,8191,0,2*PI,(float)can->Gimbal_Yaw_Angle_RX) - PI;
 //gimbal[AxisSelect].Angle_Pool = float_map(0,8191,0,2*PI,(float)can->Gimbal_Pitch_Angle_RX) - PI; 
}

/**
  * @brief  更新机械角度角速度
  * @param  速度环每个n*dt个周期抽样
  * @note   
  * @retval 
  */
void GIMBAL_RX_Sampled(GIMBAL_st *gimbal)
{	
 gimbal[P].Angle_Sampled = gimbal[P].Angle_Pool; 
 gimbal[Y].Angle_Sampled = gimbal[Y].Angle_Pool;
	
 gimbal[P].Speed_Sampled = gimbal[P].Speed_Pool;
 gimbal[Y].Speed_Sampled = gimbal[Y].Speed_Pool;
	
 gimbal[P].Current_Sampled = gimbal[P].Current_Pool;
 gimbal[Y].Current_Sampled = gimbal[Y].Current_Pool;	
}

/**
  * @brief  更新IMU角度和角速度
  * @param  
	* @note   PITCH(角度)
						YAW(角度 角速度)
  * @retval 
  */
void GMBAL_IMU_Update(GIMBAL_st* imu, GIMBALMode_en SelectMode, AxisSelect_en SelectforAxis)
{

		GIMBAL[P].IMU_Angle =  *(GIMBAL[P].Gimbal_IMU_Angle_Data);
		GIMBAL[Y].IMU_Angle =  *(GIMBAL[Y].Gimbal_IMU_Angle_Data);
	
//		GIMBAL[P].IMU_Speed = arm_cos_f32(GIMBAL[Y].Angle_Pool) * (*(GIMBAL[Y].Gimbal_IMU_Angle_Data + 1))
//		                    - arm_sin_f32(GIMBAL[Y].Angle_Pool) * (*(GIMBAL[P].Gimbal_IMU_Angle_Data + 1));
	
		GIMBAL[Y].IMU_Speed = arm_cos_f32(GIMBAL[P].Angle_Pool) * (*(GIMBAL[Y].Gimbal_IMU_Angle_Data + 0))
		                    - arm_sin_f32(GIMBAL[P].Angle_Pool) * (*(GIMBAL[P].Gimbal_IMU_Angle_Data + 0));

}

void GIMBAL_CanbusCtrlMotors(GIMBAL_st *gimbal)
{
// 	 if((RC_Ctl.rc.ch1 = 0x00) || (RC_Ctl.rc.ch4 = 0x00))
//			GIMBALOutClean();	
 CAN1_TX(0,0,0,0,
						 gimbal[P].Gimbal_Final_Out, gimbal[Y].Gimbal_Final_Out,
						 0,0, GimbalID, 0x1ff);
}

void GIMBAL_Process(GimbalPidInformation *pitch, GimbalPidInformation* yaw, GIMBAL_st *gimbal)
{
 //模式一
 if(gimbal[P].GIMBALMode == ChassisCtrlGimbal)
 { 
 gimbal[P].Gimbal_Final_Out = \
	GimbalPitch_PID_Calc\
	  (pitch, gimbal[P].Angle_Sampled, gimbal[P].Gimbal_Angle_Target,gimbal[P].Speed_Sampled);
 }		

 if(gimbal[Y].GIMBALMode == ChassisCtrlGimbal)
 {	
 gimbal[Y].Gimbal_Final_Out = \
	GimbalYaw_PID_Calc\
		(yaw, gimbal[Y].Angle_Sampled, gimbal[Y].Gimbal_Angle_Target,gimbal[Y].Speed_Sampled);
 }
 
 if (gimbal[P].GIMBALMode == ApartGimbal)
 {
 gimbal[P].Gimbal_Final_Out = \
	GimbalPitch_PID_Calc\
	  (pitch, gimbal[P].Angle_Sampled, gimbal[P].Gimbal_Angle_Target,gimbal[P].Speed_Sampled);
 }	 
 if (gimbal[Y].GIMBALMode == ApartGimbal)
 {
 gimbal[Y].Gimbal_Final_Out = \
	GimbalIMUYaw_PID_Calc\
		(yaw, gimbal[Y].IMU_Angle, gimbal[Y].Gimbal_Angle_Target,gimbal[Y].IMU_Speed); 
 }	 
}

/**
  * @brief  陀螺仪代替编码器控制云台
  * @param  
  * @note   PITCH陀螺仪提供角加速度
						YAW  陀螺仪直接控制
  * @retval 
  */
void GIMBALCtrlByIMU(GIMBAL_st* Ctrl)
{
  //把输入设定值转化为弧度
	Ctrl[Y].Gimbal_Angle_Target = float_map(0,2*PI,0,2*PI,Ctrl[Y].Gimbal_Angle_Target);
	//Ctrl[P].Gimbal_Angle_Target = float_map(0,2*PI,0,2*PI,Ctrl[P].Gimbal_Angle_Target);	
	//接受遥控器控制值
	//Ctrl[P].Gimbal_IMU_Angle_Set += (kRc_RC_Pitch * PitchRCValue);
}

void Gimbal_IMU_Aspeed_Get(GIMBAL_st* Gimbal_Aspeed)
{
//	Gimbal_Aspeed[P].IMU_Speed = *Gimbal_Aspeed[P].Gimbal_IMU_Aspeed_Data;
//	Gimbal_Aspeed[Y].IMU_Speed = arm_cos_f32(Gimbal_Aspeed->Gimbal_Pitch_Msg_t.Gimbal_Motor_Angle_TM) * (*(Gimbal_Aspeed->Gimbal_IMU_Aspeed + 0))
//                                                        - arm_sin_f32(Gimbal_Aspeed->Gimbal_Pitch_Msg_t.Gimbal_Motor_Angle_TM) * (*(Gimbal_Aspeed->Gimbal_IMU_Aspeed + 1));
//	
}

void GimbalModeChange(GIMBAL_st* Change)
{
	if ((Change[P].GIMBALMode == ChassisCtrlGimbal) && (Change[Y].GIMBALMode == ChassisCtrlGimbal))
	{
	
	}
	if ((Change[P].GIMBALMode == ChassisCtrlGimbal) && (Change[Y].GIMBALMode == ApartGimbal))
	{
	
	}
}

/**
  * @brief  角度相对坐标转换
  * @param  NowAngle AngleMiddle
  * @note   
  * @retval 越界后相对坐标
  */
float AngleRelativeTrans(float SetAngle, float AngleMiddle)
{
	  //float RelativeEcd =  NowAngle - AngleMiddle;
    if (SetAngle > PI)
    {
        SetAngle -= 2*PI;
    }
    else if (SetAngle < -PI)
    {
        SetAngle += 2*PI;
    }
		return SetAngle;
}


void GIMBAL_Task(void *pvParameters)
{
 while(1)
{
 GIMBAL_RC_Ctrl(GIMBAL);
 //GIMBAL_PC_Ctrl(GIMBAL);
 GimbalModeChange(GIMBAL);
	
 GIMBAL[P].GIMBALMode = ChassisCtrlGimbal;
 GIMBAL[Y].GIMBALMode = ChassisCtrlGimbal;	
	
 GIMBAL_RX_Pool(&Gimbal_RX, GIMBAL, PIType, PITCH);
 GIMBAL_RX_Pool(&Gimbal_RX, GIMBAL, PIType, YAW);	
 GIMBAL_RX_Sampled(GIMBAL);	
 GIMBAL_Process(&HardPitch, &Yaw, GIMBAL);
 //GIMBAL_Yaw_Process(&Yaw, GIMBAL);
 GIMBAL_CanbusCtrlMotors(GIMBAL); 
 vTaskDelay(10);
}
}








