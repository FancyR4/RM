/**
  ****************************(C) COPYRIGHT 2020 HRBUST_AIR****************************
  * @file    judge_task.c
	* @brief   裁判系统任务           
  * @author  JackyJuu , HRBUST_AIR_TEAM , website:www.airclub.tech
	* @Note 	 定期发送
  * @version V1.0.0
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     12-4-2020      JackyJuu            Done
  ****************************(C) COPYRIGHT 2020 HRBUST_AIR****************************
	* @describe OLED显示屏任务
*/
#include "judge_task.h"

#include "Judge_Data.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"



extern DJI_Judge_t DJI_Judge; 

uint8_t Judge_Test_Send_Data[6] = {0x11,0x22,0x33,0x44,0x55,0x66};
uint8_t Judge_Test_Graphic_Name[3] = {0x11,0x22,0x33};

//uint8_t	Judge_Test_Graphic_Data[200];

int test_size;

uint32_t test_judge_graphic_data;

int write_mode = 0;

uint8_t	Judge_Test_Graphic_Data[] = "Hello World!!";
uint8_t Judge_Test_Graphic_Data_Send[45];

float test_nummmm = 123.123123;
int32_t test_nummmmmm = 12888;

int key_last;
void Judge_Task(void *pvParameters)
{

	while(1)
	{

					vTaskDelay(10);
	}
}
