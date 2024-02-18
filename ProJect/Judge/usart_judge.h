#ifndef __USART_JUDGE_H__
#define __USART_JUDGE_H__

#include "main.h"

#include "stm32f4xx.h"
#include "stm32f4xx_it.h"

void DJI_Judge_Usart_Init(void);
void USART6_SendData(uint8_t data);

#endif 
