#ifndef _GIMBALTASK_H_
#define _GIMBALTASK_H_

#include "stdbool.h"
#include "main.h"
#include "can.h"
#include "pid.h"

typedef enum 
{
	Origin,
	PIType,
} RXDataType_en;

typedef enum
{
	PITCH,
	YAW,
} AxisSelect_en;

typedef enum
{
	ChassisCtrlGimbal,
	ApartGimbal,
} GIMBALMode_en;

//typedef struct 
//{
//	//云台角度目标值
//	float Gimbal_Angle_Target;
//} Control_st;

typedef struct
{
	//云台轴选择
	AxisSelect_en AxisSelect;
	//云台模式选择
  GIMBALMode_en GIMBALMode;
	//陀螺仪角度获取地址
	const float* Gimbal_IMU_Angle_Data;
	//陀螺仪角速度获取地址
	const float* Gimbal_IMU_Aspeed_Data;
	//陀螺仪角度获取
	float IMU_Angle,IMU_Speed;
	//陀螺仪角速度获取
	//云台绝对角度-陀螺仪角度差
	float Gimbal_IMU_Angle_Set;
	//云台陀螺仪控制输出
	float Gimbal_IMU_Out;
	//云台编码器信息获取
	CanRx_Gimbal_st *Motor_Data;
	//云台反馈电机信息
	float Speed_Pool;
	float Angle_Pool;
	float Current_Pool;
	//云台控制信息
  float Gimbal_Angle_Target;
	//转换后的相对控制（-PI~PI）
	float Gimbal_AngleTM_Target;
	//云台能达到的最大角度
	float Gimbal_Max_Angle;
	//云台达到最小角度
	float Gimbal_Min_Angle;
	//
	float Gimbal_Middle_Value; 
	//选中数据处理
	float Speed_Sampled;
	float Angle_Sampled;
	float Current_Sampled;
	//反馈数据类型
	RXDataType_en RXDataType;
	//云台最终输出值
	int16_t Gimbal_Final_Out;
} GIMBAL_st;

void GIMBAL_RC_Ctrl(GIMBAL_st *gimbal);
void GIMBAL_PC_Ctrl(GIMBAL_st *gimbal);
void GIMBAL_Init(GIMBAL_st *gimbal);

void GIMBAL_CanbusCtrlMotors(GIMBAL_st *gimbal);

void GIMBAL_Task(void *pvParameters);

void GIMBAL_Process(GimbalPidInformation *pitch, GimbalPidInformation* yaw, GIMBAL_st *gimbal);
void GIMBAL_RX_Pool(CanRx_Gimbal_st *can, GIMBAL_st *gimbal, RXDataType_en RXDataType, AxisSelect_en AxisSelect);
void GIMBAL_RX_Sampled(GIMBAL_st *gimbal);
void GIMBALOutClean(void);

void GMBAL_IMU_Update(GIMBAL_st* imu, GIMBALMode_en SelectMode, AxisSelect_en SelectforAxis);

void GIMBALCtrlByIMU(GIMBAL_st* Ctrl);
float AngleRelativeTrans(float NowAngle,float AngleMiddle);

#endif

