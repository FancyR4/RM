#ifndef _IMUTASK_H_
#define _IMUTASK_H_ 

#include "main.h"

#define USE_IST8310 //是否使用IST8310磁力计，不使用注释定义

//#define MPU6500_USE_DATA_READY_EXIT //是否使用MPU6500的外部中断，不使用注释定义

//#define MPU6500_USE_SPI_DMA //是否使用SPI的DMA传输，不使用注释定义

//如果用了IST8310，DMA传输23个字节，如果不用，少7个字节，为16个字节
#if defined(USE_IST8310)
#define DMA_RX_NUM 23
#else
#define DMA_RX_NUM 16
#endif

//mpu6500原始数据在缓冲区buf的位置
#ifdef MPU6500_USE_SPI_DMA
#define MPU6500_RX_BUF_DATA_OFFSET 1
#else
#define MPU6500_RX_BUF_DATA_OFFSET 0
#endif

//ist83100原始数据在缓冲区buf的位置
#ifdef MPU6500_USE_SPI_DMA
#define IST8310_RX_BUF_DATA_OFFSET 16
#else
#define IST8310_RX_BUF_DATA_OFFSET 15
#endif

#define MPU6500_TEMPERATURE_PID_KP 1600.0f //温度控制PID的kp
#define MPU6500_TEMPERATURE_PID_KI 0.2f    //温度控制PID的ki
#define MPU6500_TEMPERATURE_PID_KD 0.0f    //温度控制PID的kd

#define MPU6500_TEMPERATURE_PID_MAX_OUT 4500.0f  //温度控制PID的max_out
#define MPU6500_TEMPERATURE_PID_MAX_IOUT 4400.0f //温度控制PID的max_iout

#define INS_DELTA_TICK 1 //任务调用的间隔

#define INS_TASK_INIT_TIME 10 //任务开始初期 delay 一段时间

#define MPU6500_TEMP_PWM_MAX 5000 //mpu6500控制温度的设置TIM的重载值，即给PWM最大为 MPU6500_TEMP_PWM_MAX - 1

//获取姿态角指针地址后，对应姿态角的地址偏移量 fp32类型
#define INS_YAW_ADDRESS_OFFSET 0
#define INS_PITCH_ADDRESS_OFFSET 1
#define INS_ROLL_ADDRESS_OFFSET 2

#define INS_GYRO_X_ADDRESS_OFFSET 0
#define INS_GYRO_Y_ADDRESS_OFFSET 1
#define INS_GYRO_Z_ADDRESS_OFFSET 2

#define INS_ACCEL_X_ADDRESS_OFFSET 0
#define INS_ACCEL_Y_ADDRESS_OFFSET 1
#define INS_ACCEL_Z_ADDRESS_OFFSET 2

enum PID_MODE
{
    PID_POSITION = 0,
    PID_DELTA
};

typedef struct
{
    uint8_t mode;
    //PID 三参数
    fp32 Kp;
    fp32 Ki;
    fp32 Kd;

    fp32 max_out;  //最大输出
    fp32 max_iout; //最大积分输出

    fp32 set;
    fp32 fdb;

    fp32 out;
    fp32 Pout;
    fp32 Iout;
    fp32 Dout;
    fp32 Dbuf[3];  //微分项 0最新 1上一次 2上上次
    fp32 error[3]; //误差项 0最新 1上一次 2上上次
	
		fp32 sum_of_error;
		fp32 plus;
		fp32 plus_out;
		fp32 last_plus_out;

} PidTypeDef;

extern void IMU_Task(void *pvParameters);

extern void INS_cali_gyro(fp32 cali_scale[3], fp32 cali_offset[3], uint16_t *time_count);
extern void INS_set_cali_gyro(fp32 cali_scale[3], fp32 cali_offset[3]);
extern const fp32 *get_INS_angle_point(void);
extern const fp32 *get_MPU6500_Gyro_Data_Point(void);
extern const fp32 *get_MPU6500_Accel_Data_Point(void);

#endif
