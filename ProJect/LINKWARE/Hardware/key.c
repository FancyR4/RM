#include "key.h"

void key_init(void)//PB2
{
	GPIO_InitTypeDef GPIO_InitStructure;	
 	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

int KEY_Scan(void)
{
	return KEY_READ;
}
