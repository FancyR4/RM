//#include "SysCtrl.h"


/*
channel 最大值 1684中间值 1024最小值 364
电流值范围：-32768~32767
channel 0 rotate

channel 2:left_right
channel 3 forward_back

*/
#include "ChassisTask.h"
#include "Judge_Data.h"
#include "RemoteTask.h"
#include "pid.h"
#include "can.h"
#include "user_lib.h"

#include "FreeRTOS.h"
#include "task.h"
#include "FreeRTOSConfig.h"

#define RCKEYVALUE 3000.0f
#define KEYVALUE 0.50f

extern volatile  RC_Ctl_t RC_Ctl;

//升级底盘功率
const float ChassisPowerLevel[3];

extern ChassisPidInformation Chassis_Pid_Motor[4];

extern CanRX_Chassis_st Chassis_Rx;
	
extern DJI_Judge_t DJI_Judge; 	
	
CHASSIS_st CHASSIS;	

//底盘期望速度
static float Chassis_Speed_Target[4];//ID

//底盘电机测量速度
static int16_t Chassis_Speed_Measure[4];

//整车全向移动方向变量
static float Chassis_Move_X;
static float Chassis_Move_Y;
static float Chassis_Move_Z;
//不同方向运动比值分量
static float Chassis_Move_K[3];
//底盘功率限制
const int16_t Z = 60;//焦耳


static float Chassis_Via_PID_Output[4];

/**
	* @brief  发送电机
  * @param  功率  电压限幅
  * @note   功率限制
  * @retval 
  */
void CHASSIS_CAN_CtlMotor(CtrlChassisMotor_en Ctrl)
{
    int i;
		for(i = LEFT_FRON_201; i < (RIGH_BACK_204+1); i++)
	{
	  Chassis_Via_PID_Output[i] =	ChassPowerEdg(Chassis_Via_PID_Output[i],
																													55, 60, 0, 0);		
	  Chassis_Via_PID_Output[i] =	ChassPowerEdg(Chassis_Via_PID_Output[i],
																													50, 55, -1000, 1000);
	  Chassis_Via_PID_Output[i] =	ChassPowerEdg(Chassis_Via_PID_Output[i],
																													30, 50, -10000, 10000);
	  Chassis_Via_PID_Output[i] =	ChassPowerEdg(Chassis_Via_PID_Output[i],
																													0, 30, -25000, 25000);		
	}
	if (Ctrl == ChassisNormal)
	{
		CAN1_TX((int16_t)(Chassis_Via_PID_Output[LEFT_FRON_201]), 
            (int16_t)(Chassis_Via_PID_Output[RIGH_FRON_202]), 
            (int16_t)(Chassis_Via_PID_Output[LEFT_BACK_203]), 
            (int16_t)(Chassis_Via_PID_Output[RIGH_BACK_204]),
						0,0,0,0,0,
						0x200);
	}
	if (Ctrl == ChassisCleanOut)
	{
		CAN1_TX(0, 
            0, 
            0, 
            0,
						0,0,0,0,0,
						0x200);	
	}
}

void CHASSISOutClear(void)
{
	Chassis_Speed_Target[LEFT_FRON_201] = 0;
  Chassis_Speed_Target[RIGH_FRON_202] = 0;
  Chassis_Speed_Target[LEFT_BACK_203] = 0;
  Chassis_Speed_Target[RIGH_BACK_204] = 0;
}

void CHASSISCtrlByMB()
{
	
	Chassis_Move_X += (KEY_PRESSED_OFFSET_A + (-KEY_PRESSED_OFFSET_D)) *KEYVALUE;
	Chassis_Move_Y += (KEY_PRESSED_OFFSET_W + (-KEY_PRESSED_OFFSET_S)) *KEYVALUE;
	Chassis_Move_Z += (KEY_PRESSED_OFFSET_Q + (-KEY_PRESSED_OFFSET_E)) *KEYVALUE;
	
	Chassis_Move_X = float_constrain(Chassis_Move_X, -RCKEYVALUE, RCKEYVALUE);
	Chassis_Move_Y = float_constrain(Chassis_Move_X, -RCKEYVALUE, RCKEYVALUE);
	Chassis_Move_Z = float_constrain(Chassis_Move_X, -RCKEYVALUE, RCKEYVALUE);	
}

/**
  * @brief  麦克纳姆轮运动学解算
  * @param  void
  * @note   
  * @retval void
  */
void CHASSIS_Mecanum_Anlysis(void)
{
	Chassis_Move_K[0] = 1;
	Chassis_Move_K[1] = 1;
	Chassis_Move_K[2] = 1;
	Chassis_Move_Y = ((float)RC_CH3_LUD_OFFSET/(float)RC_CH_VALUE_OFFSET * RCKEYVALUE)*Chassis_Move_K[0];
  Chassis_Move_X = ((float)RC_CH2_LLR_OFFSET/(float)RC_CH_VALUE_OFFSET * RCKEYVALUE)*Chassis_Move_K[1];
  Chassis_Move_Z = ((float)RC_CH4_RLR_OFFSET/(float)RC_CH_VALUE_OFFSET * RCKEYVALUE)*Chassis_Move_K[2];

//全向移动算法
  Chassis_Speed_Target[LEFT_FRON_201] = (- Chassis_Move_Y +Chassis_Move_X -Chassis_Move_Z );
  Chassis_Speed_Target[RIGH_FRON_202] = (+ Chassis_Move_Y +Chassis_Move_X -Chassis_Move_Z );
  Chassis_Speed_Target[LEFT_BACK_203] = (+ Chassis_Move_Y -Chassis_Move_X -Chassis_Move_Z );
  Chassis_Speed_Target[RIGH_BACK_204] = (- Chassis_Move_Y -Chassis_Move_X -Chassis_Move_Z );
//
}

/**
  * @brief  运动计算过程
  * @param  
  * @note   底盘功率限制
  * @retval 
  */
void CHASSIS_Motor_Process(void)
{	
//	Chassis_Speed_Target[LEFT_FRON_201] = (-( +Chassis_Move_Y +Chassis_Move_X +Chassis_Move_Z ));
//  Chassis_Speed_Target[RIGH_FRON_202] = (+( -Chassis_Move_Y +Chassis_Move_X -Chassis_Move_Z ));
//  Chassis_Speed_Target[LEFT_BACK_203] = (+( -Chassis_Move_Y +Chassis_Move_X +Chassis_Move_Z ));
//  Chassis_Speed_Target[RIGH_BACK_204] = (-( +Chassis_Move_Y +Chassis_Move_X -Chassis_Move_Z ));
	
  Chassis_Speed_Measure[0] = Chassis_Rx.Motor_LEFT_FRON_201_Speed_RX;
	Chassis_Speed_Measure[1] = Chassis_Rx.Motor_RIGH_FRON_202_Speed_RX;
	Chassis_Speed_Measure[2] = Chassis_Rx.Motor_LEFT_BACK_203_Speed_RX;
	Chassis_Speed_Measure[3] = Chassis_Rx.Motor_RIGH_BACK_204_Speed_RX;
	Chassis_Via_PID_Output[LEFT_FRON_201] = CHASSIS_PID_Calc(Chassis_Pid_Motor, Chassis_Speed_Measure[LEFT_FRON_201], Chassis_Speed_Target[LEFT_FRON_201], LEFT_FRON_201);
  Chassis_Via_PID_Output[RIGH_FRON_202] = CHASSIS_PID_Calc(Chassis_Pid_Motor, Chassis_Speed_Measure[RIGH_FRON_202], Chassis_Speed_Target[RIGH_FRON_202], RIGH_FRON_202);
  Chassis_Via_PID_Output[LEFT_BACK_203] = CHASSIS_PID_Calc(Chassis_Pid_Motor, Chassis_Speed_Measure[LEFT_BACK_203], Chassis_Speed_Target[LEFT_BACK_203], LEFT_BACK_203);
  Chassis_Via_PID_Output[RIGH_BACK_204] = CHASSIS_PID_Calc(Chassis_Pid_Motor, Chassis_Speed_Measure[RIGH_BACK_204], Chassis_Speed_Target[RIGH_BACK_204], RIGH_BACK_204);
}

/* 底盘功率管理  */

float ChassisPowerRule(float PIDOutIn)
{
		if(DJI_Judge.DJI_Judge_Mes.Judge_power_heat_data.chassis_power > CHASSIS.ChassisManage.PowerEdg[0])
			return float_constrain(PIDOutIn, -10000, 10000);
		else if ((DJI_Judge.DJI_Judge_Mes.Judge_power_heat_data.chassis_power >= CHASSIS.ChassisManage.PowerEdg[0]) && 
						 (DJI_Judge.DJI_Judge_Mes.Judge_power_heat_data.chassis_power < CHASSIS.ChassisManage.PowerEdg[1]))
			return float_constrain(PIDOutIn, -18000, 18000);
		else 
			return PIDOutIn;
}

float ChassPowerEdg(float PIDOut, float EdgMIN, float EdgMAX, float BonMIN, float BonMAX)
{
		float Out;
		if((DJI_Judge.DJI_Judge_Mes.Judge_power_heat_data.chassis_power > EdgMIN) &&
			 (DJI_Judge.DJI_Judge_Mes.Judge_power_heat_data.chassis_power < EdgMAX))
		{
		Out = float_constrain(PIDOut, BonMIN, BonMAX);
		return Out;
		}
		else 
			return PIDOut;
		
}

float Chassis_All_Speed_Target()
{
	return (abs((float)Chassis_Rx.Motor_LEFT_FRON_201_Speed_RX)+
				  abs((float)Chassis_Rx.Motor_RIGH_FRON_202_Speed_RX)+ 
				  abs((float)Chassis_Rx.Motor_LEFT_BACK_203_Speed_RX)+ 
				  abs((float)Chassis_Rx.Motor_RIGH_BACK_204_Speed_RX));
}

/* 底盘功率管理  */

void CHASSIS_Task(void *pvParameters)
{
while(1)
{
	ChassisRC();
	vTaskDelay(10);
}

}

int16_t Lastchassissw;
void ChassisRC()
{
	
	if (SWITCH2_UP)
    {
     CHASSIS_Mecanum_Anlysis();
     CHASSIS_Motor_Process();
     CHASSIS_CAN_CtlMotor(ChassisNormal);
			Lastchassissw = SWITCH2_UP;
    }
    if (SWITCH2_MID)
    {
     CHASSIS_Mecanum_Anlysis();
     CHASSIS_Motor_Process();
     CHASSIS_CAN_CtlMotor(ChassisNormal);
			Lastchassissw = SWITCH2_MID;			
    }
    if (SWITCH2_DOWN)
    {
			CHASSIS_CAN_CtlMotor(ChassisCleanOut);
			Lastchassissw = SWITCH2_DOWN;
    }
		
		if (SWITCH2_NULL)
		{
		 CHASSISCtrlByMB();
		}
}

