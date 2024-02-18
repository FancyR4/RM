#ifndef _CAN_H_
#define _CAN_H_
#include "main.h"

#include "stm32f4xx.h"
#include "stm32f4xx_it.h"

void CAN1_Configuration(void);
void Can1ReceiveMsgProcess(CanRxMsg *msg);
void CAN2_Configuration(void);
void Can2ReceiveMsgProcess(CanRxMsg *can_receive_message);

void CAN1_TX(int16_t Motor_LEFT_FRON_201,
             int16_t Motor_RIGH_FRON_202,
             int16_t Motor_LEFT_BACK_203,
             int16_t Motor_RIGH_BACK_204,
						 int16_t Motor_Gimbal_Pitch,
						 int16_t Motor_Gimbal_Yaw,
						 int16_t Motor_UpStir,
						uint16_t SuperCap_Power,	
						 int16_t Identify,
						 int16_t STDID);
//void Gimbal_Upstir_Run(int16_t Motor_Gimbal_Pitch,
//											 int16_t Motor_Gimbal_Yaw,
//											 int16_t Motor_UpStir,
//											 int16_t Identify);
#define GimbalID 1
#define UpstirID 2
#define SuperCapID 3
void Fric_DownStir_Run(int16_t Motor_FricL,
											 int16_t Motor_FricR,
											 int16_t Motor_DownStir,
											 int16_t Identify);
#define FricID 1
#define DownstirID 2
//void Upstir_Run(int16_t Motor_UpStir);
//void DownStir_Run(int16_t Motor_DownStir);

typedef struct
{
    int16_t Motor_LEFT_FRON_201_TX;
    int16_t Motor_RIGH_FRON_202_TX;
    int16_t Motor_LEFT_BACK_203_TX;
    int16_t Motor_RIGH_BACK_204_TX;
} CanTx_Chassis_st;

typedef struct
{
    int16_t FricL_Motor_TX;
    int16_t FricR_Motor_TX;
    int16_t UpStir_Motor_TX;
    int16_t DownStir_Motor_TX;
} CanTx_Shoot_st;

typedef struct
{
    int16_t Gimbal_Pitch_TX;
    int16_t Gimbal_Yaw_TX;
} CanTx_Gimbal_st;

typedef struct
{
    int16_t Motor_LEFT_FRON_201_Speed_RX;
    int16_t Motor_RIGH_FRON_202_Speed_RX;
    int16_t Motor_LEFT_BACK_203_Speed_RX;
    int16_t Motor_RIGH_BACK_204_Speed_RX;
} CanRX_Chassis_st;

typedef struct
{
    int16_t FricL_Motor_Speed_RX;
    int16_t FricR_Motor_Speed_RX;
    int16_t UpStir_Motor_Speed_RX;
    int16_t UpStir_Motor_Angle_RX;
    int16_t DownStir_Motor_Speed_RX;
    int16_t DownStir_Motor_Angle_RX;
} CanRx_Shoot_st;

typedef struct
{
    int16_t Gimbal_Pitch_Speed_RX;
    int16_t Gimbal_Pitch_Angle_RX;
    int16_t Gimbal_Yaw_Speed_RX;
    int16_t Gimbal_Yaw_Angle_RX;
	
    int16_t Gimbal_Speed_RX;
    int16_t Gimbal_Angle_RX;	
} CanRx_Gimbal_st;

typedef struct
{
		float InputVoltage;
		float CapacitorVoltage;
		float InputCurrent;
		float TargetPower;
} CanRX_Power_st;

typedef struct
{
	int16_t Angle;
	int16_t Speed;
	int16_t Current;
} CanRXMsg_st;

typedef struct
{
	int16_t Input_Voltage;
	int16_t Cap_Voltage;
	int16_t Input_Current;
	int16_t Output_Power;
} CanRXMsgCAP_st;

typedef enum
{
	Motor3508_0 = 0,
	Motor3508_1,
	Motor3508_2,
	Motor3508_3,	
	Motor6020_Yaw,
	Motor6020_Pitch,
	Motor3508_FricL,
	Motor3508_FricR,
	Motor2006_BD,
	Motor3508_Magazine,
	SuperCap_,
} CanDev_en;
CanRXMsg_st* CanRecvMsg_CallBack(CanDev_en CanDev);

//CanRX_Chassis_st Chassis_Rx;

//extern struct MotorInfomation Chassis_Motor[4];//, Chassis_Motor2, Chassis_Motor3, Chassis_Motor4;

#endif

