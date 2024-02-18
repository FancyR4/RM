#include "Start_Task.h"
#include "ChassisTask.h"
#include "GimbalTask.h"
//#include "LunchTask.h"
//#include "IMUtask.h"
#include "judge_task.h"
#include "Calibrate_Task.h"
#include "INS_Task.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"


#define START_TASK_PRIO 1
#define START_TASK_STK_SIZE 128
static TaskHandle_t StartTask_Handler;

#define Chassis_TASK_PRIO 3
#define Chassis_TASK_STK_SIZE 512
static TaskHandle_t ChassisTask_Handler;

#define Gimbal_TASK_PRIO 4
#define Gimbal_TASK_STK_SIZE 512
static TaskHandle_t GimbalTask_Handler;

#define Lunch_TASK_PRIO 5
#define Lunch_TASK_STK_SIZE 512
static TaskHandle_t LunchTask_Handler;

//#define IMU_TASK_PRIO 7
//#define IMU_STK_SIZE 512
//static TaskHandle_t IMUTask_Handler;

#define JUDGE_TASK_PRIO 10
#define JUDGE_TASK_STK_SIZE 512
static TaskHandle_t JudgeTask_Handler;

#define INS_TASK_PRIO 20
#define INS_TASK_SIZE 512
static TaskHandle_t INSTask_Handler;

#define CALIBRATE_TASK_PRIO 5
#define CALIBRATE_STK_SIZE 512
static TaskHandle_t CalibrateTask_Handler;


void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();

	xTaskCreate((TaskFunction_t)CHASSIS_Task,
				(const char *)"Chassis_Task",
				(uint16_t)Chassis_TASK_STK_SIZE,
				(void *)NULL,
				(UBaseType_t)Chassis_TASK_PRIO,
				(TaskHandle_t *)&ChassisTask_Handler);
	xTaskCreate((TaskFunction_t)GIMBAL_Task,
				(const char *)"Giambl_Task",
				(uint16_t)Gimbal_TASK_STK_SIZE,
				(void *)NULL,
				(UBaseType_t)Gimbal_TASK_PRIO,
				(TaskHandle_t *)&GimbalTask_Handler);
//    xTaskCreate((TaskFunction_t)LUNCH_Task,
//				(const char *)"Lunch_Task",
//				(uint16_t)Lunch_TASK_STK_SIZE,
//				(void *)NULL,
//				(UBaseType_t)Lunch_TASK_PRIO,
//				(TaskHandle_t *)&LunchTask_Handler);
    xTaskCreate((TaskFunction_t)Judge_Task,(const char *)"Judge_Task",
		    (uint16_t)JUDGE_TASK_STK_SIZE,
	      (void *)NULL,
				(UBaseType_t)JUDGE_TASK_PRIO,
				(TaskHandle_t *)&JudgeTask_Handler);
//    xTaskCreate((TaskFunction_t)IMU_Task,
//		    (const char *)"IMU_Task",
//				(uint16_t)IMU_STK_SIZE,(void *)NULL,
//				(UBaseType_t)IMU_TASK_PRIO,
//				(TaskHandle_t *)&IMUTask_Handler);
    xTaskCreate((TaskFunction_t)calibrate_task,
                (const char *)"CaliTask",
                (uint16_t)CALIBRATE_STK_SIZE,
                (void *)NULL,
                (UBaseType_t)CALIBRATE_TASK_PRIO,
                (TaskHandle_t *)&CalibrateTask_Handler);
    xTaskCreate((TaskFunction_t)INSTask,
                (const char *)"INSTask",
                (uint16_t)INS_TASK_SIZE,
                (void *)NULL,
                (UBaseType_t)INS_TASK_PRIO,
                (TaskHandle_t *)&INSTask_Handler);
								
    
	vTaskDelete(StartTask_Handler); //删除开始任务
    taskEXIT_CRITICAL();            //退出临界区
}

void startTast(void)
{
    xTaskCreate((TaskFunction_t)start_task,          //任务函数
                (const char *)"start_task",          //任务名称
                (uint16_t)START_TASK_STK_SIZE,       //任务堆栈大小
                (void *)NULL,                        //传递给任务函数的参数
                (UBaseType_t)START_TASK_PRIO,        //任务优先级
                (TaskHandle_t *)&StartTask_Handler); //任务句柄
								
}

