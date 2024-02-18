#ifndef DELAY_H
#define DELAY_H
#include "main.h"
#include "stm32f4xx.h"

extern void delay_init(uint32_t TICK_RATE_HZ);
extern void delay_us(uint16_t nus);
extern void delay_ms(uint16_t nms);
#endif


//#ifndef __DELAY_H
//#define __DELAY_H 			   
//#include "sys.h"	  
//#include "main.h"

//void Delay_init(u8 SYSCLK);
//void delay_us(u32 nus);
//void delay_ms(u32 nms);
//void delay_xms(u32 nms);

//#endif






























