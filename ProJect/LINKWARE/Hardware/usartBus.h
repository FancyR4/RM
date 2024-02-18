//#ifndef __USARTBUS_H
//#define __USARTBUS_H

//#include "main.h"
//#include "stm32f4xx_usart.h"


//typedef struct
//{
//    struct
//    {
//        uint16_t ch0;
//        uint16_t ch1;
//        uint16_t ch2;
//        uint16_t ch3;
//				uint16_t ch4;
//        uint8_t s1;
//        uint8_t s2;
//    } rc;

//		struct
//		{
//				int16_t x;
//				int16_t y;
//				int16_t z;
//				uint8_t press_l;
//				uint8_t press_r;
//		} mouse;

//		struct
//		{
//				uint16_t v;
//		} key;
//		
//} RC_Ctl_t;


////extern volatile  RC_Ctl_t RC_Ctl;

//#define RC_CH_VALUE_MIN 364
//#define RC_CH_VALUE_MID 1024
//#define RC_CH_VALUE_MAX 1684
//#define RC_CH_VALUE_OFFSET 660

//#define RC_CH1_PITCH_OFFSET (RC_Ctl.rc.ch1 - RC_CH_VALUE_MID)
//#define RC_CH0_YAW_OFFSET   (RC_Ctl.rc.ch4 - RC_CH_VALUE_MID)

//#define RC_CH0_RLR_OFFSET (RC_Ctl.rc.ch0 - RC_CH_VALUE_MID)
//#define RC_CH1_RUD_OFFSET (RC_Ctl.rc.ch1 - RC_CH_VALUE_MID)
//#define RC_CH2_LLR_OFFSET (RC_Ctl.rc.ch2 - RC_CH_VALUE_MID)
//#define RC_CH3_LUD_OFFSET (RC_Ctl.rc.ch3 - RC_CH_VALUE_MID)
//#define RC_DIAL    				(RC_Ctl.rc.ch4 - RC_CH_VALUE_MID)

//#define SWITCH1_UP   (RC_Ctl.rc.s1==1) 
//#define SWITCH1_MID  (RC_Ctl.rc.s1==3) 
//#define SWITCH1_DOWN (RC_Ctl.rc.s1==2) 
//#define SWITCH2_UP   (RC_Ctl.rc.s2==1) 
//#define SWITCH2_MID  (RC_Ctl.rc.s2==3)
//#define SWITCH2_DOWN (RC_Ctl.rc.s2==2)

//#define MOUSE_X         RC_Ctl.mouse.x
//#define MOUSE_Y         RC_Ctl.mouse.y
//#define MOUSE_Z         RC_Ctl.mouse.z
//#define MOUSE_MIN -32768
//#define MOUSE_MAX 32767
//#define MOUSE_L_PRESS   RC_Ctl.mouse.press_l == 1
//#define MOUSE_R_PRESS   RC_Ctl.mouse.press_r == 1
//#define MOUSE_L_RELEASE RC_Ctl.mouse.press_l == 0
//#define MOUSE_R_RELEASE RC_Ctl.mouse.press_r == 0

//#define KEY_PRESSED_OFFSET_W ((uint16_t)0x01<<0)
//#define KEY_PRESSED_OFFSET_S ((uint16_t)0x01<<1)
//#define KEY_PRESSED_OFFSET_A ((uint16_t)0x01<<2)
//#define KEY_PRESSED_OFFSET_D ((uint16_t)0x01<<3)
//#define KEY_PRESSED_OFFSET_Q ((uint16_t)0x01<<4)
//#define KEY_PRESSED_OFFSET_E ((uint16_t)0x01<<5)
//#define KEY_PRESSED_OFFSET_SHIFT ((uint16_t)0x01<<6)
//#define KEY_PRESSED_OFFSET_CTRL ((uint16_t)0x01<<7)

//void USART1_Configuration(void);

//#endif

#ifndef __USARTBUS_H
#define __USARTBUS_H

#include "main.h"

#include "stm32f4xx.h"
#include "stm32f4xx_it.h"

void Usart1_Dbus_Init(uint8_t *rx1_buf, uint8_t *rx2_buf, uint16_t dma_buf_num);

#endif 



