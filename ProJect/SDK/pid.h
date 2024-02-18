#ifndef _PID_H_
#define _PID_H_

#include "main.h"
#include "ChassisTask.h"

typedef struct
{
		float _errorIN;
		float _ScalarA;
		float _ScalarB;
		float _Iout;
		float _I_Multiply;
} PID_EXTEND;

typedef struct  {

    float Kp, Ki, Kd;		//PID����
    float Now_Error;			//��ǰ���
    float Last_Error;			//��һ�����
    float Last_Last_Error;	//���ϴ����
    float Error_Buf[3];     //����
		float Sum_Error;      //��ʷ�����
    float maxoutput;      //������ֵ
    float limit_err;      //�޶������
    float Set;						//����ֵ
    float Now;						//��ǰֵ
    float Out;		//���ֵ
    float Pout;
    float Iout;
    float Dout;
    float Dbuf[3];

}ChassisPidInformation;

typedef enum
{
		FRICL = 0 ,
		FRICR = 1,
} FricSelect_en;
typedef struct
{
		FricSelect_en FricSelect;
} Functional_st;
typedef struct {
	
		float measure;
		float give;
		float Kp,Ki,Kd;
		float Now_Error;			//��ǰ���
    float Last_Error;			//��һ�����
    float Last_Last_Error;	//���ϴ����
		float Current_Out;		        //�������ֵ
    float Voltage_Out;              //��ѹ���ֵ
    float Pout;
    float Iout;
    float Dout;
    float Error_buf[3];
		Functional_st Functional;

} FricPIDinformation;

typedef struct
{
		float Kp,Ki,Kd;
		float Now_Error;			//��ǰ���
    float Last_Error;			//��һ�����
    float Last_Last_Error;	//���ϴ����
		float Angle_Loop_Out;
    float Palstance_Loop_Out;
    float Current_Out;		        //�������ֵ
    float Voltage_Out;              //��ѹ���ֵ
    float Pout;
    float Iout;
    float Dout;
    float Error_buf[3];        
} UpstirPIDinformation;

typedef enum
{
	downstirpidrun,
	downstirpidstop,
	downstirslide,
} DownstirMode_en;
typedef struct
{
		float Kp,Ki,Kd;
		float Now_Error;			//��ǰ���
    float Last_Error;			//��һ�����
    float Last_Last_Error;	//���ϴ����
		float Angle_Loop_Out;
    float Palstance_Loop_Out;
    float Current_Out;		        //�������ֵ
    float Voltage_Out;              //��ѹ���ֵ
    float Pout;
    float Iout;
    float Dout;
    float Error_buf[3];    
} DownstirPIDinformation;

typedef enum
{
	Trap,
	Continue,
} LoopSit_en;

typedef struct {

    float Kp[2],Ki[2],Kd[2];
	struct {
        float Now;					//��ǰ���
        float Last;					//��һ�����
        float Last_Last;		//���ϴ����
        float Sum;        	//��ʷ�����	    
	} Error[2];
    float maxoutput;        //������ֵ
    float limit_err;        //�޶������
    struct {
				float Angle;
				float Speed;
	} Set;
    struct {                 //��ǰֵ

        float Angle;
        float Speed;
        float accel;

    } Now;
	struct {
        float P;
		float I;
		float D;
        float P_1;
		float I_1;
		float D_1;		
		float Current;
		float Voltage;
		float Loop_1;
		float Loop_2;
		float final;
	
	} Out;
		int16_t loopCnt;
		int16_t loop_1Cnt;
		int16_t loopF;
		int16_t loop_1F;
		LoopSit_en LoopSit;
    float Error_Buf[3];
	  float Error_Buf_1[3];
		float loopK;
		float add;
		float base;
		float Deadlinemin[2];
		float Deadlinemax[2];
		PID_EXTEND EXTEND;
} GimbalPidInformation;

void PID_limit(float *a, float PID_MAX);
void PID_limit_(float *a, float PID_MAX);
void PID_juedui(float *a);

void CHASSIS_PID_Parameter(ChassisPidInformation *PID);
void Upstir_PID_Parameter(UpstirPIDinformation *PID);
void Downstir_PID_Parameter(DownstirPIDinformation *PID);
void Fric_PID_Parameter(FricPIDinformation *PID);
void GIMBALPitch_PID_Parameter(GimbalPidInformation *PID);
void GIMBALYaw_PID_Parameter(GimbalPidInformation *PID);
void GIMBALIMUYaw_PID_Parameter(GimbalPidInformation *PID);

void PID_Init(void);

float CHASSIS_PID_Calc(ChassisPidInformation *Chassis_Pid_Motor, float ref, float set, emTYPE_ChassisWheel Wheel);
float Upstir_PID_Calc(UpstirPIDinformation *PID, float ref, float set);
float Downstir_PID_Calc(DownstirPIDinformation *PID, float ref, float set, DownstirMode_en mode);
float Fric_PID_Calc(FricPIDinformation *PID, float ref, float set, FricSelect_en Select, int16_t Hlat);
float GimbalYaw_PID_Calc(GimbalPidInformation *PID, float ref, float set, float get);
float GimbalPitch_PID_Calc(GimbalPidInformation *PID, float ref, float set, float get);
float GimbalIMUYaw_PID_Calc(GimbalPidInformation *PID, float IMUA, float set, float IMUS);

static void f_Changing_Integral_Rate(PID_EXTEND *PID);

#endif

