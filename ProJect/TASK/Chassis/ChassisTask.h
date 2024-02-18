#ifndef _CHASSIS_TASK_H_
#define _CHASSIS_TASK_H_

#include "main.h"


//���̵��ID
typedef enum
{
    LEFT_FRON_201 = 0,  // ��ǰ
    RIGH_FRON_202 = 1,  // ��ǰ
    LEFT_BACK_203 = 2,  // ���
    RIGH_BACK_204 = 3,  // �Һ�

} emTYPE_ChassisWheel;

typedef enum
{
	Base,
} ChassisEdg_en;

typedef enum
{
	ChassisNormal,
	ChassisCleanOut,
} CtrlChassisMotor_en;

typedef struct
{
		float PowerEdg[4];
		float PowerLimit[2];
		
} ChassisManage_st;

typedef struct
{
	 ChassisManage_st ChassisManage;
} CHASSIS_st;

void CHASSIS_Task(void *pvParameters);

float ChassPowerEdg(float PIDOut, float EdgMIN, float EdgMAX, float BonMIN, float BonMAX);
void CHASSIS_Motor_Process(void);
void CHASSIS_Mecanum_Anlysis(void);
void CHASSIS_CAN_CtlMotor(CtrlChassisMotor_en Ctrl);
void CHASSISOutClear(void);
float Chassis_All_Speed_Target(void);
void ChassisRC(void);

#endif

