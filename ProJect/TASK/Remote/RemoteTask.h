#ifndef _REMOTETASK_H_
#define _REMOTETASK_H_

#include "main.h"

#include "stm32f4xx.h"
#include "stm32f4xx_it.h"

#define RC_CH_VALUE_MIN 364
#define RC_CH_VALUE_MID 1024
#define RC_CH_VALUE_MAX 1684
#define RC_CH_VALUE_OFFSET 660

#define RC_CH1_PITCH_OFFSET (RC_Ctl.rc.ch1 - RC_CH_VALUE_MID)
#define RC_CH0_YAW_OFFSET   (RC_Ctl.rc.ch0 - RC_CH_VALUE_MID)

#define RC_CH0_RLR_OFFSET (RC_Ctl.rc.ch0 - RC_CH_VALUE_MID)
#define RC_CH1_RUD_OFFSET (RC_Ctl.rc.ch1 - RC_CH_VALUE_MID)
#define RC_CH2_LLR_OFFSET (RC_Ctl.rc.ch2 - RC_CH_VALUE_MID)
#define RC_CH3_LUD_OFFSET (RC_Ctl.rc.ch3 - RC_CH_VALUE_MID)
#define RC_CH4_RLR_OFFSET (RC_Ctl.rc.ch4 - RC_CH_VALUE_MID)

#define SWITCH1_READ RC_Ctl.rc.s1
#define SWITCH2_READ RC_Ctl.rc.s2
#define SWITCH1_UP   (RC_Ctl.rc.s1==1) 
#define SWITCH1_MID  (RC_Ctl.rc.s1==3) 
#define SWITCH1_DOWN (RC_Ctl.rc.s1==2) 
#define SWITCH2_UP   (RC_Ctl.rc.s2==1) 
#define SWITCH2_MID  (RC_Ctl.rc.s2==3)
#define SWITCH2_DOWN (RC_Ctl.rc.s2==2)
#define SWITCH2_NULL (RC_Ctl.rc.s2==0)
#define SWITCH1_NULL (RC_Ctl.rc.s1==0)

#define MOUSE_X         RC_Ctl.mouse.x
#define MOUSE_Y         RC_Ctl.mouse.y
#define MOUSE_Z         RC_Ctl.mouse.z
#define MOUSE_MIN -32768
#define MOUSE_MAX 32767

#define MOUSE_PRESS					1
#define MOUSE_RELEASE				0
#define MOUSE_L_PRESS_READ  RC_Ctl.mouse.press_l
#define MOUSE_R_PRESS_READ	RC_Ctl.mouse.press_r
#define MOUSE_L_PRESS       RC_Ctl.mouse.press_l == 1
#define MOUSE_R_PRESS       RC_Ctl.mouse.press_r == 1
#define MOUSE_L_RELEASE     RC_Ctl.mouse.press_l == 0
#define MOUSE_R_RELEASE 		RC_Ctl.mouse.press_r == 0

#define KEY_PRESSED_OFFSET_W 		 ((uint16_t)1 << 0)
#define KEY_PRESSED_OFFSET_S 		 ((uint16_t)1 << 1)
#define KEY_PRESSED_OFFSET_A 		 ((uint16_t)1 << 2)
#define KEY_PRESSED_OFFSET_D 		 ((uint16_t)1 << 3)
#define KEY_PRESSED_OFFSET_SHIFT ((uint16_t)1 << 4)
#define KEY_PRESSED_OFFSET_CTRL  ((uint16_t)1 << 5)
#define KEY_PRESSED_OFFSET_Q 		 ((uint16_t)1 << 6)
#define KEY_PRESSED_OFFSET_E		 ((uint16_t)1 << 7)

#define KEY_PRESSED_OFFSET_R 		 ((uint16_t)1 << 8)

#define KEY_PRESSED_OFFSET_F 		 ((uint16_t)1 << 9)
#define KEY_PRESSED_OFFSET_G 		 ((uint16_t)1 << 10)

#define KEY_PRESSED_OFFSET_Z 		 ((uint16_t)1 << 11)
#define KEY_PRESSED_OFFSET_X 		 ((uint16_t)1 << 12)
#define KEY_PRESSED_OFFSET_C 		 ((uint16_t)1 << 13)
#define KEY_PRESSED_OFFSET_V 		 ((uint16_t)1 << 14)
#define KEY_PRESSED_OFFSET_B 		 ((uint16_t)1 << 15)

/* 检测键盘按键状态 
   若对应按键被按下，则逻辑表达式的值为1，否则为0 */
#define    IF_KEY_PRESSED         (  RC_Ctl.key.v  )
#define    IF_KEY_PRESSED_W       ( (RC_Ctl.key.v & KEY_PRESSED_OFFSET_W)    != 0 )
#define    IF_KEY_PRESSED_S       ( (RC_Ctl.key.v & KEY_PRESSED_OFFSET_S)    != 0 )
#define    IF_KEY_PRESSED_A       ( (RC_Ctl.key.v & KEY_PRESSED_OFFSET_A)    != 0 )
#define    IF_KEY_PRESSED_D       ( (RC_Ctl.key.v & KEY_PRESSED_OFFSET_D)    != 0 )
#define    IF_KEY_PRESSED_Q       ( (RC_Ctl.key.v & KEY_PRESSED_OFFSET_Q)    != 0 )
#define    IF_KEY_PRESSED_E       ( (RC_Ctl.key.v & KEY_PRESSED_OFFSET_E)    != 0 )
#define    IF_KEY_PRESSED_G       ( (RC_Ctl.key.v & KEY_PRESSED_OFFSET_G)    != 0 )
#define    IF_KEY_PRESSED_X       ( (RC_Ctl.key.v & KEY_PRESSED_OFFSET_X)    != 0 )
#define    IF_KEY_PRESSED_Z       ( (RC_Ctl.key.v & KEY_PRESSED_OFFSET_Z)    != 0 )
#define    IF_KEY_PRESSED_C       ( (RC_Ctl.key.v & KEY_PRESSED_OFFSET_C)    != 0 )
#define    IF_KEY_PRESSED_B       ( (RC_Ctl.key.v & KEY_PRESSED_OFFSET_B)    != 0 )
#define    IF_KEY_PRESSED_V       ( (RC_Ctl.key.v & KEY_PRESSED_OFFSET_V)    != 0 )
#define    IF_KEY_PRESSED_F       ( (RC_Ctl.key.v & KEY_PRESSED_OFFSET_F)    != 0 )
#define    IF_KEY_PRESSED_R       ( (RC_Ctl.key.v & KEY_PRESSED_OFFSET_R)    != 0 )
#define    IF_KEY_PRESSED_CTRL    ( (RC_Ctl.key.v & KEY_PRESSED_OFFSET_CTRL) != 0 )
#define    IF_KEY_PRESSED_SHIFT   ( (RC_Ctl.key.v & KEY_PRESSED_OFFSET_SHIFT) != 0 )

typedef __packed struct
{
	__packed struct
	{ 
		int16_t ch0;
		int16_t ch1;
		int16_t ch2;
		int16_t ch3;
		int16_t ch4;
		char s1;
		char s2;
	}rc;
	
	__packed struct 
	{
		int16_t x;
		int16_t y;
		int16_t z;
		uint8_t press_l;
		uint8_t press_r;
	}mouse;
	
	__packed struct
	{
		uint16_t v;
	}key;
}RC_Ctl_t;

//void RC_Restart_Set(void);

void remote_control_init(void);

const RC_Ctl_t *Get_DJI_RC_Data_Address(void);


#endif

