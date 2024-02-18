#include "LunchTask.h"
#include "RemoteTask.h"
#include "Judge_Data.h"
#include "gpio.h"
#include "can.h"
#include "pid.h"
#include "user_lib.h"
#include "laser.h"
#include "timer.h"
#include "SysCtrl.h"

#include "FreeRTOS.h"
#include "task.h"
#include "FreeRTOSConfig.h"

#define FRICROTATESPEED 5000
#define FRICDifferentialRotateSpeedThreshold 50
#define L 0
#define R 1

#define DownSWClose 1
#define DownSWOpen  0

#define SWREAD   GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_0)

extern volatile  RC_Ctl_t RC_Ctl;

/**
  * @brief  Ħ����״̬���
  * @param  ����ת��:int16_t NowSpeed[2]
  * @note   ����/�ٶ�δ�ﵽ/����ת�ٲ�һ��/
  * @retval FricCheck_sit_en
  */
FricCheck_sit_en FricCheck(Shoot_st *friccheck, DOMINATE_en DOMINATE)
{
		if(DOMINATE == DO_ENABLE)
		{
		//Low Speed	NowSpeed[L]
		if(friccheck->Fric_MSG[L].FricRotatingSpeed_FB <= FRICROTATESPEED)
			return LeftSpeedLow;
		//else 
			//return LeftSpeedNormal;
		if(friccheck->Fric_MSG[R].FricRotatingSpeed_FB <= FRICROTATESPEED)
			return RightSpeedLow;
		//else 
			//return RightSpeedNormal;
		
		//Inconsistent Speeds
		if((friccheck->Fric_MSG[L].FricRotatingSpeed_FB - friccheck->Fric_MSG[R].FricRotatingSpeed_FB) > FRICDifferentialRotateSpeedThreshold)
			return LeftMoreRight;
		else if((friccheck->Fric_MSG[L].FricRotatingSpeed_FB - friccheck->Fric_MSG[R].FricRotatingSpeed_FB) < -FRICDifferentialRotateSpeedThreshold)
			return LeftLessRight;
		else
			return LeftConsistentRight;
		
		//Every is ok
		return LRALLRIGHT;		
		}
		else return 1;
}

/**
  * @brief  �쳣����(Ħ����)
  * @param  none
  * @note   none
  * @retval none
  */

/**
  * @brief  ң��������ģʽ(DJIң����)
  * @param  void
  * @note   
  * @retval RemoteControllor_en
  */
static int16_t lastkey;
RemoteControllor_en ControlMethod1(void)
{
	
	if((SWITCH1_UP) || (SWITCH1_MID))
	{
		if(SWITCH1_READ != lastkey)
		{
			return Remote_DOFix;
		}
		else 
			return Remote_NOAction;	
	}
	if(SWITCH1_DOWN)
	{
	
	}
	lastkey = SWITCH1_READ;
	
}

/**
  * @brief  ���Զ˿���ģʽ
  * @param  void
  * @note   
  * @retval PCControllor_en
  */
static int16_t lastpressl;
static int16_t lastpressr;
PCControllor_en  ControlMethod2(void)
{
	//��һ����־,�����ڼ䱻��ȴ
//	if(MOUSE_L_PRESS)
//		lastpressl = MOUSE_L_PRESS_READ;
	//if(MOUSE_L_RELEASE)
		lastpressl = MOUSE_L_PRESS_READ;
	//if(MOUSE_R_PRESS)
		lastpressr = MOUSE_R_PRESS_READ;
//	if(MOUSE_R_RELEASE)
//		lastpressr = MOUSE_R_PRESS_READ;
	
	if((MOUSE_L_RELEASE) && (lastpressl == MOUSE_PRESS))
	{
		return PC_DOFix;
	}
	
	if((MOUSE_L_PRESS) && (lastpressl == MOUSE_RELEASE))
	{
		return PC_NOaction;
	}
	//����(��ʱ���ֿ���Pitch������ٶ���׼)
	if(KEY_PRESSED_OFFSET_X)
	{
		return PC_LobShot;
	}
}

/**
  * @brief  �жϵ�ǰң�ط�ʽ�Ƿ����(������)
  * @param  way(ControlWay_en)
  * @note   
  * @retval 
  */
ControlisOnline_en CheckControl(ControlWay_en way)
{
	if(way == RC)
	{
		if(SWITCH1_NULL)
			return ControlOffline;
		else 
		{
			//һ��ң�����е�������׳�һ������
			return ControlOnline;	
		}
			
	}
	if(way == PC)
	{
		/////////////////////////
	}
}	


ControlOut_en RCControl(Shoot_st* rc )
{
		//ControlState_en ControlState;
		ControlWay_en NowWay;
		ControlisOnline_en isOnline;
		//������Ƴ�ʼ��
		//1.�ж��Ƿ����
		//2.�쳣���
		//3.��ʼ������(�Ǳ���)
		
		if(rc->ControlState_RECORD == SHOOT_INITAL)
		{
			isOnline = CheckControl(NowWay);
			//if()
			rc->ShootState_RECORD = ShootIdle;
			//reset while inited
			rc->ControlState_RECORD = SHOOT_NORMAL;
			
		}			
		
		//����������������(�����⵽�����������³�ʼ��)
		if(rc->ControlState_RECORD == SHOOT_NORMAL)  
		{
			//ң����/PC���
			if(rc->ShootState_RECORD == ShootIdle)  
			{
				if(CheckControl(RC) == ControlOnline)
				{
					//ң������δ����˵����ǰ���ƻ���ң����
					NowWay = RC;
					if(ControlMethod1() == Remote_DOFix)
							return Fix;
					if(ControlMethod1() == Remote_NOAction)
							return Idle;
				}
				if(CheckControl(RC) == ControlOffline)
				{
					//ң��������:
					//1.��ɵ�ǰ����
					//2.�������ֵ
					//3.�ȴ�����
									
				}
				if(CheckControl(PC) == ControlOnline)
				{
					NowWay = PC;
					if(ControlMethod2() == Remote_DOFix)
							return Fix;
					if(ControlMethod2() == Remote_NOAction)
							return Idle;
					if(ControlMethod2() == PC_LobShot)
							;
				}
				if(CheckControl(PC) == ControlOffline)
				{
								
				}
			}
			
			if(rc->ShootState_RECORD == ShootBusy) 
			{
							
			}								
}

/**
  * @brief  ��Ħ����ת��
  * @param  Executor(FricExecutor_en)
  * @note   
  * @retval void
  */
void FricExecution(FricExecutor_en Executor, Shoot_st* ShootFric)
{
	if(Executor == FricHalt)
	{
		Shoot.Fric_MSG[FRICL].FricRotatingSpeed_SET = 0;
		Shoot.Fric_MSG[FRICR].FricRotatingSpeed_SET = 0;
		
		ShootFric->Shoot_Set_MotorESC.Fric_C620_3508[L] = 
			Fric_PID_Calc(ShootFric->PIDofFric, 
										Shoot_RX.FricL_Motor_Speed_RX, 
										Shoot.Fric_MSG[FRICL].FricRotatingSpeed_SET,
										FRICL, 
										ENABLE);
		ShootFric->Shoot_Set_MotorESC.Fric_C620_3508[R] = 
			Fric_PID_Calc(ShootFric->PIDofFric, 
										Shoot_RX.FricR_Motor_Speed_RX, 
										Shoot.Fric_MSG[FRICR].FricRotatingSpeed_SET,
										FRICR, 
										ENABLE);
	}
	if(Executor == FricMarch)
	{
		Shoot.Fric_MSG[FRICL].FricRotatingSpeed_SET = -FRICROTATESPEED;
		Shoot.Fric_MSG[FRICR].FricRotatingSpeed_SET =  FRICROTATESPEED;
		
		ShootFric->Shoot_Set_MotorESC.Fric_C620_3508[L] = 
			Fric_PID_Calc(ShootFric->PIDofFric, 
										Shoot_RX.FricL_Motor_Speed_RX, 
										Shoot.Fric_MSG[FRICL].FricRotatingSpeed_SET,
										FRICL, 
										DISABLE);
		ShootFric->Shoot_Set_MotorESC.Fric_C620_3508[R] = 
			Fric_PID_Calc(ShootFric->PIDofFric, 
										Shoot_RX.FricR_Motor_Speed_RX, 
										Shoot.Fric_MSG[FRICR].FricRotatingSpeed_SET,
										FRICR, 
										DISABLE);		
	}
	//DEBUG MODE
	if(Executor == FricTest)
	{

	}
}

/**
	* @brief  ��������:����/ֹͣ��ס
  * @param  Executor(DownstirExecutor_en)
  * @note   
  * @retval void
  */
void DownstirExecution(DownstirExecutor_en Executor)
{
	if (Executor == DownstirSlide)
	{
	downstir->Shoot_Set_MotorESC.Downstir_C610_P2006 = 
		Downstir_PID_Calc
			(&DownstirPID, Shoot_RX.DownStir_Motor_Speed_RX, 0, downstirslide);	
	}
	
	if (Executor == DownstirHalt)
	{
	downstir->Shoot_Set_MotorESC.Downstir_C610_P2006 = 
		Downstir_PID_Calc
			(&DownstirPID, Shoot_RX.DownStir_Motor_Speed_RX, 0, downstirpidstop);
	}
}

/**
  * @brief  ����ִ�в���
  * @param  void
  * @note    
  * @retval void
  */
void MagazineExecution(MagazineExecutor_en Executor, Shoot_st* magazine)
{
	if(Executor == MagazineNormal)
	{
		magazine->Shoot_Set_MotorESC.Upstir_C620_3508 = 
		Upstir_PID_Calc(&PIDofUpstir, Shoot_RX.UpStir_Motor_Speed_RX, 300);
	}
	if(Executor == MagazineHalt)
	{
		magazine->Shoot_Set_MotorESC.Upstir_C620_3508 = 0;
	}
	//����
	if(Executor == MagazineStuck)
	{
	
	}
}

/**
  * @brief  ����ģʽ��
  * @param  void
  * @note   
  * @retval void
  */
void IdleFunction(Shoot_st* idle)
{
	//����״̬δ��ռ��
	//if(idle->FuncStateCallback.GlobeState != ShootBusy)
	//{
		DownstirExecution(DownstirSlide);
		//Ħ������������
		MagazineExecution(MagazineNormal);
	
		DownSWFunction(IdleMode);
	//}
	//else return ;
}

typedef struct
{
	int16_t Array[];
} DisplacementArray_st;

void DisplacementArray_Init(DisplacementArray_st *DisplacementArray ,int16_t Length)
{
	
}

/**
  * @brief  ��΢������ʵ�ֹ���(ģʽ���ƹ���)
  * @param  
  * @note   ����/����/...
						������{�պ�/�ɿ�/�պ�}/{�ɿ�/�պ�/�ɿ�}
  * @retval 
  */
	typedef enum
	{
		IdleMode,
		SingleShotMode,
	} SWMode_en;
	int16_t NowReadSW;
	int16_t LastSW;
	int16_t LastLastSW;
	int16_t HistorySW[3];
void DownSWFunction(SWMode_en mode)
{
	//������Ҫ����
	NowReadSW = SWREAD;
	if(NowReadSW != LastSW)
	{
		HistorySW[0] = NowReadSW;
		for(int i = 1; i <= 0; i--)
		{
			HistorySW[i] = HistorySW[i-1];
		}
	}
	LastSW = NowReadSW;
	
	//ģʽʶ��׶�
	if(mode == IdleMode)
	{
		if(NowReadSW == DownSWClose)
		{
			//��סһ���������(��¼�Ƿ��п�����)
			DownstirExecution(DownstirHalt);
			MagazineExecution(MagazineHalt, &Shoot);
		}
		if(NowReadSW == DownSWOpen)
		{
			//�ڿ���ģʽ�����û�е�����һֱ�ϵ�
			MagazineExecution(MagazineNormal, &Shoot);
			DownstirExecution(DownstirSlide);
		}
	}
	
	if(mode == SingleShotMode)
	{
		while(!((HistorySW[0] != HistorySW[1]) &&
			      (HistorySW[0] == HistorySW[2])   ))
		{
			//���������δ����Ͳ�ͣ����(Ħ���ֱ����ת��)
			MagazineExecution(MagazineNormal, &Shoot);
			DownstirExecution(DownstirSlide);
			//return ;
		}
//		if(!((HistorySW[0] != HistorySW[1]) &&
//			   (HistorySW[0] == HistorySW[2])   ))		
			//���һ�η���(Ħ���ֵù�)
		DownstirExecution(DownstirHalt);
		//return ; 
	}
	
}

/**
  * @brief  ��ǰ�����¼�(��ǰֻ���˵���)
  * @param  
  * @note   
  * @retval ShootState_en
  */
ShootState_en FixFunction(Shoot_st* fix)
{
	
	FricSlef_FixCondition_en FricSlef_FixCondition;
	Projectile_FixCondition_en Projectile_FixCondition;
	//�ȿ���Ħ����
	FricExecution(FricMarch);
	
	//�������е�(һ�㶼���е���)
	if(SWREAD == DownSWClose)
	{
		Projectile_FixCondition = Projectile_OK;
	}
	//����΢���������޵�
	else if(SWREAD == DownSWOpen)
	{
		Projectile_FixCondition = Projectile_Wait;
	}
	//δ֪���??????????????????
	else 
		Projectile_FixCondition = Projectile_Fail;
	
	//Ħ�����ٶȴﵽҪ��(���Էŵ�Ħ���ֺ�����)	
	if(FricCheck(fix, DO_ENABLE) == LRALLRIGHT)
	{
		FricSlef_FixCondition = FricSlef_OK;
	}
	//Ħ�����ٶȲ���������Ҫ�ų�������(ʵ������ֱ�ӵ�bug�ռ�)
	else if((FricCheck(fix, DO_ENABLE) == LeftSpeedLow) ||
					(FricCheck(fix, DO_ENABLE) == RightSpeedLow))
	{
		FricSlef_FixCondition = FricSlefWait;
	}
	
	//δ֪���??????????????????
	else FricSlef_FixCondition = FricSlef_Fail; 	
	
	//���㷢����Ҫ����
	if((Projectile_FixCondition == Projectile_OK) &&
		 (FricSlef_FixCondition   == FricSlef_OK)     )
	{
		DownstirExecution(DownstirSlide);
		DownSWFunction(SingleShotMode);
		
		//��������Ħ����Ӧ��Ҫͣ
		FricExecution(FricHalt);
	}

	//���һ�η����������������ģʽ
	return ShootIdle;
}

ShootState_en StateCallback;

ShootState_en LunchFunction(ControlOut_en ControlOut, Shoot_st* downfric)
{
	if(ControlOut == Fix)
	{
		//ִ�е��Ƿ�����Զ�������Ҫ��ʱ���ٵ�ǰ״̬
		
		//���ﲻ�ǲ���,������,û�дﵽ��Ҫʵ�ֹ���
		StateCallback = FixFunction(downfric);
	}
	else if(ControlOut == Idle)
	{
		//û�������һֱ�����������״̬
		IdleFunction(downfric);
		StateCallback = ShootIdle
	}
	//����Ӹ��๦��
	//.
	//..
	//...
	//....
	else 
	{
	
	}
	return StateCallback;
}

void LUNCH_Console(Shoot_st *Console)
{
	//ң��������м����
	//��ǰ״̬���ֵ
	//RECORD�𵽿��������й��̽����Ĺ���
	Console->ControlOut_RECORD = RCControl(Console); 
	Console->ShootState_RECORD = FixFunction(Console->ControlOut_RECORD, Console);
}

void LUNCH_Init(Shoot_st *Init)
{
	Fric_PID_Parameter(Init->PIDofFric);
	Downstir_PID_Parameter(Init->PIDofDownstir);
	Upstir_PID_Parameter(Init->PIDofUpstir);
	
	Init->->ShootState_RECORD = ShootIdle;
	rc->ControlState_RECORD = SHOOT_INITAL;
}

void StateCallBack(void)
{

}

void Lunch_CleanOut()
{
	
}

void LUNCH_Task(void *pvParameters)
{
	LUNCH_Init(&Shoot);
	while(1)
	{
			LUNCH_Console(&Shoot);
			vTaskDelay(10);
	}
}

