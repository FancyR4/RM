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
//	//��̨�Ƕ�Ŀ��ֵ
//	float Gimbal_Angle_Target;
//} Control_st;

typedef struct
{
	//��̨��ѡ��
	AxisSelect_en AxisSelect;
	//��̨ģʽѡ��
  GIMBALMode_en GIMBALMode;
	//�����ǽǶȻ�ȡ��ַ
	const float* Gimbal_IMU_Angle_Data;
	//�����ǽ��ٶȻ�ȡ��ַ
	const float* Gimbal_IMU_Aspeed_Data;
	//�����ǽǶȻ�ȡ
	float IMU_Angle,IMU_Speed;
	//�����ǽ��ٶȻ�ȡ
	//��̨���ԽǶ�-�����ǽǶȲ�
	float Gimbal_IMU_Angle_Set;
	//��̨�����ǿ������
	float Gimbal_IMU_Out;
	//��̨��������Ϣ��ȡ
	CanRx_Gimbal_st *Motor_Data;
	//��̨���������Ϣ
	float Speed_Pool;
	float Angle_Pool;
	float Current_Pool;
	//��̨������Ϣ
  float Gimbal_Angle_Target;
	//ת�������Կ��ƣ�-PI~PI��
	float Gimbal_AngleTM_Target;
	//��̨�ܴﵽ�����Ƕ�
	float Gimbal_Max_Angle;
	//��̨�ﵽ��С�Ƕ�
	float Gimbal_Min_Angle;
	//
	float Gimbal_Middle_Value; 
	//ѡ�����ݴ���
	float Speed_Sampled;
	float Angle_Sampled;
	float Current_Sampled;
	//������������
	RXDataType_en RXDataType;
	//��̨�������ֵ
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

