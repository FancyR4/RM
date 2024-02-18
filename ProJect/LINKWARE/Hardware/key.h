#ifndef _KEY_H_
#define _KEY_H_
#include "main.h"
#include "stm32f4xx.h"

#define KEY_READ  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_2)

void key_init(void);
int KEY_Scan(void);

#endif



//KEY_IO
