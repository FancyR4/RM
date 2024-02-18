#ifndef _LUNCHTASK_H_
#define _LUNCHTASK_H_
#include "main.h"
#include "pid.h"

///////////////////////////////////////////////////
//typedef enum
//{
//	RC,
//	PC,
//} ControlWay_en;

typedef enum
{
	DO_ENABLE,
	DO_DISABLE,
}DOMINATE_en;

//电机发送数据
typedef struct
{
  int16_t Fric_C620_3508[2];
  int16_t Upstir_C620_3508;
  int16_t Downstir_C610_P2006;
} Shoot_Set_MotorESC_st;

typedef struct
{
	int16_t FricRotatingSpeed_FB;
	float FricRotatingLineSpeed_FB;
	int16_t FricRotatingSpeed_SET;
} Fric_MSG_st;

/*
*发射机构状态
*/
typedef enum 
{
	ShootIdle,
	ShootBusy,
} ShootState_en;

typedef struct
{
	ShootState_en GlobeState;
} FuncStateCallback_st;

typedef enum
{
	Remote_NOAction,
	Remote_DOFix,
} RemoteControllor_en;
RemoteControllor_en ControlMethod1(void);
	
typedef enum
{
	PC_NOaction,
	PC_DOFix,
	PC_LobShot,
} PCControllor_en; 
PCControllor_en  ControlMethod2(void);
	
typedef enum
{
	ControlOnline,
	ControlOffline,
} ControlisOnline_en;
ControlisOnline_en CheckControl(ControlWay_en way);
	
typedef enum
{
	Idle,
	Fix,
} ControlOut_en;

typedef enum
{
	SHOOT_INITAL,
	SHOOT_NORMAL,
} ControlState_en;	
	
	typedef struct
{
	ControlOut_en ControlOut_RECORD;
	ShootState_en ShootState_RECORD;
	ControlState_en ControlState_RECORD;
	
	Shoot_Set_MotorESC_st Shoot_Set_MotorESC;
	Fric_MSG_st Fric_MSG[2];
	FuncStateCallback_st FuncStateCallback;
	
	FricPIDinformation PIDofFric[2];
	DownstirPIDinformation PIDofDownstir;
	UpstirPIDinformation PIDofUpstir;
} Shoot_st;	

ControlOut_en RCControl(Shoot_st* rc );
	
	typedef enum
	{
		FricHalt,
		FricMarch,
		FricTest,
	} FricExecutor_en;
void FricExecution(FricExecutor_en Executor, Shoot_st* ShootFric);
	
	typedef enum 
	{
		DownstirSlide, 
		DownstirHalt,
	} DownstirExecutor_en;
void DownstirExecution(DownstirExecutor_en Executor);
	
	typedef enum
	{
		MagazineNormal,
		MagazineHalt,
		MagazineStuck,
	} MagazineExecutor_en;
void MagazineExecution(MagazineExecutor_en Executor, Shoot_st* magazine);

	typedef enum
	{
		LRALLRIGHT,
		LeftSpeedLow,
		LeftSpeedNormal,
		RightSpeedLow,
		RightSpeedNormal,
		LeftMoreRight,
		LeftLessRight,
		LeftConsistentRight,
	} FricCheck_sit_en;
FricCheck_sit_en FricCheck(Shoot_st *friccheck, DOMINATE_en DOMINATE);
	
	typedef enum
	{
		FricSlef_OK,
		FricSlef_Wait,
		FricSlef_Fail,
	} FricSlef_FixCondition_en;
	typedef enum
	{
		Projectile_OK,
		Projectile_Wait,
		Projectile_Fail,
	} Projectile_FixCondition_en;	
	
void IdleFunction(Shoot_st* idle);
//void DownSWFunction(SWMode_en mode);
ShootState_en FixFunction(Shoot_st* fix);
ShootState_en LunchFunction(ControlOut_en ControlOut, Shoot_st* downfric);
void LUNCH_Console(Shoot_st *Console);
void LUNCH_Init(Shoot_st *Init);
void LUNCH_Task(void *pvParameters);		
void Lunch_CleanOut(void);

#endif






