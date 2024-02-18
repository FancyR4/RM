#include "main.h"
#include "SysCtrl.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"

#include "delay.h"

#include "RemoteTask.h"
#include "can.h"
#include "ChassisTask.h"
//#include "LunchTask.h"
#include "GimbalTask.h"
#include "pid.h"
#include "laser.h"
#include "power.h"
#include "gpio.h"
#include "led.h"
#include "adc.h"
#include "flash.h"
#include "timer.h"
#include "usart_judge.h"
#include "calibrate_Task.h"
#include "Start_Task.h"


int main(void)
{
 delay_init(configTICK_RATE_HZ);
 delay_ms(10);
 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);  

 temperature_ADC_init();	

 CAN1_Configuration();
 CAN2_Configuration();
 DJI_Judge_Usart_Init(); 

 //led_configuration();
 PID_Init();
 GPIOInit();

 TIM5_Init();
 //TIM6_Init(60000, 90);	
 
 //开启激光	
 laser_configuration();
 //laser_on();
 Power_24V_Init();
 
 AllInit();	
 //GIMBAL_Init();
 cali_param_init();
 remote_control_init();
 startTast();
 vTaskStartScheduler();//开启任务调度
 while(1)
 { 
 }         
}



