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
  * @brief  摩擦轮状态检测
  * @param  左右转速:int16_t NowSpeed[2]
  * @note   正常/速度未达到/左右转速不一致/
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
  * @brief  异常处理(摩擦轮)
  * @param  none
  * @note   none
  * @retval none
  */

/**
  * @brief  遥控器控制模式(DJI遥控器)
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
  * @brief  电脑端控制模式
  * @param  void
  * @note   
  * @retval PCControllor_en
  */
static int16_t lastpressl;
static int16_t lastpressr;
PCControllor_en  ControlMethod2(void)
{
	//给一个标志,发射期间被冷却
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
	//高射(此时滚轮控制Pitch和射击速度瞄准)
	if(KEY_PRESSED_OFFSET_X)
	{
		return PC_LobShot;
	}
}

/**
  * @brief  判断当前遥控方式是否掉线(防暴走)
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
			//一旦遥控器有掉线情况抛出一个错误
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
		//发射控制初始化
		//1.判断是否掉线
		//2.异常检测
		//3.初始化参数(非必须)
		
		if(rc->ControlState_RECORD == SHOOT_INITAL)
		{
			isOnline = CheckControl(NowWay);
			//if()
			rc->ShootState_RECORD = ShootIdle;
			//reset while inited
			rc->ControlState_RECORD = SHOOT_NORMAL;
			
		}			
		
		//发弹机构正常工作(如果检测到不正常会重新初始化)
		if(rc->ControlState_RECORD == SHOOT_NORMAL)  
		{
			//遥控器/PC检查
			if(rc->ShootState_RECORD == ShootIdle)  
			{
				if(CheckControl(RC) == ControlOnline)
				{
					//遥控器还未掉线说明当前控制还是遥控器
					NowWay = RC;
					if(ControlMethod1() == Remote_DOFix)
							return Fix;
					if(ControlMethod1() == Remote_NOAction)
							return Idle;
				}
				if(CheckControl(RC) == ControlOffline)
				{
					//遥控器掉线:
					//1.完成当前工作
					//2.清除控制值
					//3.等待重连
									
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
  * @brief  下摩擦轮转动
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
	* @brief  拨弹动作:滑行/停止卡住
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
  * @brief  弹仓执行操作
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
	//卡弹
	if(Executor == MagazineStuck)
	{
	
	}
}

/**
  * @brief  空闲模式下
  * @param  void
  * @note   
  * @retval void
  */
void IdleFunction(Shoot_st* idle)
{
	//发射状态未被占用
	//if(idle->FuncStateCallback.GlobeState != ShootBusy)
	//{
		DownstirExecution(DownstirSlide);
		//摩擦轮正常运行
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
  * @brief  下微动开关实现功能(模式控制功能)
  * @param  
  * @note   单发/空闲/...
						单发：{闭合/松开/闭合}/{松开/闭合/松开}
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
	//本体主要功能
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
	
	//模式识别阶段
	if(mode == IdleMode)
	{
		if(NowReadSW == DownSWClose)
		{
			//卡住一发弹丸待命(记录是否有卡过弹)
			DownstirExecution(DownstirHalt);
			MagazineExecution(MagazineHalt, &Shoot);
		}
		if(NowReadSW == DownSWOpen)
		{
			//在空闲模式下如果没有弹丸则一直上弹
			MagazineExecution(MagazineNormal, &Shoot);
			DownstirExecution(DownstirSlide);
		}
	}
	
	if(mode == SingleShotMode)
	{
		while(!((HistorySW[0] != HistorySW[1]) &&
			      (HistorySW[0] == HistorySW[2])   ))
		{
			//当上面情况未满足就不停拨弹(摩擦轮必须得转着)
			MagazineExecution(MagazineNormal, &Shoot);
			DownstirExecution(DownstirSlide);
			//return ;
		}
//		if(!((HistorySW[0] != HistorySW[1]) &&
//			   (HistorySW[0] == HistorySW[2])   ))		
			//完成一次发射(摩擦轮得关)
		DownstirExecution(DownstirHalt);
		//return ; 
	}
	
}

/**
  * @brief  当前触发事件(当前只做了单发)
  * @param  
  * @note   
  * @retval ShootState_en
  */
ShootState_en FixFunction(Shoot_st* fix)
{
	
	FricSlef_FixCondition_en FricSlef_FixCondition;
	Projectile_FixCondition_en Projectile_FixCondition;
	//先开启摩擦轮
	FricExecution(FricMarch);
	
	//供弹环有弹(一般都会有弹丸)
	if(SWREAD == DownSWClose)
	{
		Projectile_FixCondition = Projectile_OK;
	}
	//供弹微动开关下无弹
	else if(SWREAD == DownSWOpen)
	{
		Projectile_FixCondition = Projectile_Wait;
	}
	//未知情况??????????????????
	else 
		Projectile_FixCondition = Projectile_Fail;
	
	//摩擦轮速度达到要求(可以放到摩擦轮函数内)	
	if(FricCheck(fix, DO_ENABLE) == LRALLRIGHT)
	{
		FricSlef_FixCondition = FricSlef_OK;
	}
	//摩擦轮速度不正常是需要排除的问题(实际遇到直接当bug收集)
	else if((FricCheck(fix, DO_ENABLE) == LeftSpeedLow) ||
					(FricCheck(fix, DO_ENABLE) == RightSpeedLow))
	{
		FricSlef_FixCondition = FricSlefWait;
	}
	
	//未知情况??????????????????
	else FricSlef_FixCondition = FricSlef_Fail; 	
	
	//满足发射需要条件
	if((Projectile_FixCondition == Projectile_OK) &&
		 (FricSlef_FixCondition   == FricSlef_OK)     )
	{
		DownstirExecution(DownstirSlide);
		DownSWFunction(SingleShotMode);
		
		//发射完了摩擦轮应该要停
		FricExecution(FricHalt);
	}

	//完成一次发射后发射机构进入空闲模式
	return ShootIdle;
}

ShootState_en StateCallback;

ShootState_en LunchFunction(ControlOut_en ControlOut, Shoot_st* downfric)
{
	if(ControlOut == Fix)
	{
		//执行的是发射的自动任务需要随时跟踪当前状态
		
		//这里不是并行,独立的,没有达到需要实现功能
		StateCallback = FixFunction(downfric);
	}
	else if(ControlOut == Idle)
	{
		//没有任务就一直保持这个任务状态
		IdleFunction(downfric);
		StateCallback = ShootIdle
	}
	//可添加更多功能
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
	//遥控器输出中间控制
	//当前状态输出值
	//RECORD起到控制与运行过程交互的功能
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

