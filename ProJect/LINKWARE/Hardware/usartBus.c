/**
  ****************************(C) COPYRIGHT 2020 HRBUST_AIR****************************
  * @file    usart.c
  * @brief   遥控器接收与数据处理
  * @author  WJR , HRBUST_AIR_TEAM , website:www.airclub.tech
  * @Note
  * @version V1.0.0
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     11-18-2020      WJR             Done
  ****************************(C) COPYRIGHT 2020 HRBUST_AIR****************************
	* @describe 程序启动入口
*/

#include "usartBus.h"

volatile unsigned char sbus_rx_buffer[25]; 

void Usart1_Dbus_Init(uint8_t *rx1_buf, uint8_t *rx2_buf, uint16_t dma_buf_num)
{
	        /* -------------- Enable Module Clock Source ----------------------------*/
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_DMA2, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

        RCC_APB2PeriphResetCmd(RCC_APB2Periph_USART1, ENABLE);
        RCC_APB2PeriphResetCmd(RCC_APB2Periph_USART1, DISABLE);

        GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_USART1); //PB7  usart1 rx
                                                                  /* -------------- Configure GPIO ---------------------------------------*/
{
                GPIO_InitTypeDef GPIO_InitStructure;
                USART_InitTypeDef USART_InitStructure;
                GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
                GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
                GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
                GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
                GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
                GPIO_Init(GPIOB, &GPIO_InitStructure);

                USART_DeInit(USART1);

                USART_InitStructure.USART_BaudRate = 100000;
                USART_InitStructure.USART_WordLength = USART_WordLength_8b;
                USART_InitStructure.USART_StopBits = USART_StopBits_1;
                USART_InitStructure.USART_Parity = USART_Parity_Even;
                USART_InitStructure.USART_Mode = USART_Mode_Rx;
                USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
                USART_Init(USART1, &USART_InitStructure);

                USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);

                USART_ClearFlag(USART1, USART_FLAG_IDLE);
                USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);

                USART_Cmd(USART1, ENABLE);
							}

        /* -------------- Configure NVIC ---------------------------------------*/
{
                NVIC_InitTypeDef NVIC_InitStructure;
                NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
                NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
                NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
                NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
                NVIC_Init(&NVIC_InitStructure);
}
        //DMA2 stream5 ch4  or (DMA2 stream2 ch4)    !!!!!!! P206 of the datasheet
        /* -------------- Configure DMA -----------------------------------------*/
   {
								DMA_InitTypeDef DMA_InitStructure;
                DMA_DeInit(DMA2_Stream2);

                DMA_InitStructure.DMA_Channel = DMA_Channel_4;
                DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (USART1->DR);
                DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)rx1_buf;
                DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
                DMA_InitStructure.DMA_BufferSize = dma_buf_num;
                DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
                DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
                DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
                DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
                DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
                DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
                DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
                DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
                DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
                DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
                DMA_Init(DMA2_Stream2, &DMA_InitStructure);
                DMA_DoubleBufferModeConfig(DMA2_Stream2, (uint32_t)rx2_buf, DMA_Memory_0);
                DMA_DoubleBufferModeCmd(DMA2_Stream2, ENABLE);
                DMA_Cmd(DMA2_Stream2, DISABLE); //Add a disable
                DMA_Cmd(DMA2_Stream2, ENABLE);

	}


}









//#include "usartBus.h"

//volatile unsigned char sbus_rx_buffer[36];
//volatile  RC_Ctl_t RC_Ctl;




//void USART1_Configuration(void)
//{

//    USART_InitTypeDef usart1;
//    GPIO_InitTypeDef  gpio;
//    NVIC_InitTypeDef  nvic;
//    DMA_InitTypeDef   dma;

//    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_DMA2, ENABLE);	//使能GPIOB和DMA的时钟
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);												//使能USART1时钟

//    GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_USART1); 	//GPIOB7复用为USART1

//    gpio.GPIO_Pin = GPIO_Pin_7;
//    gpio.GPIO_Mode = GPIO_Mode_AF;
//    gpio.GPIO_OType = GPIO_OType_PP;
//    gpio.GPIO_Speed = GPIO_Speed_100MHz;
//    gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
//    GPIO_Init(GPIOB, &gpio);

//    USART_DeInit(USART1);
//    usart1.USART_BaudRate = 100000;												//波特率为100000，参照遥控器说明书
//    usart1.USART_WordLength = USART_WordLength_8b;				//数据字长度为8位
//    usart1.USART_StopBits = USART_StopBits_1;							//1位停止位
//    usart1.USART_Parity = USART_Parity_Even;							//奇校验
//    usart1.USART_Mode = USART_Mode_Rx;											//读数据
//    usart1.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//    USART_Init(USART1, &usart1);				//初始化USART1

//    USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
//    USART_Cmd(USART1, ENABLE);			//使能USART1

//    USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);		//使能DMA

//    nvic.NVIC_IRQChannel = DMA2_Stream2_IRQn;				//设置中断通道
//    nvic.NVIC_IRQChannelPreemptionPriority = 0;			//设置中断的抢占优先级为0
//    nvic.NVIC_IRQChannelSubPriority = 0;						//设置中断的相应优先级为0
//    nvic.NVIC_IRQChannelCmd = ENABLE;								//IRQ通道使能    IRQ——中断请求
//    NVIC_Init(&nvic);				//根据上面的参数初始化NVIC寄存器

//    DMA_Cmd(DMA2_Stream2, DISABLE);
//    while (DMA2_Stream2->CR & DMA_SxCR_EN);
//    DMA_DeInit(DMA2_Stream2);
//    dma.DMA_Channel = DMA_Channel_4;
//    dma.DMA_PeripheralBaseAddr = (uint32_t) & (USART1->DR);
//    dma.DMA_Memory0BaseAddr = (uint32_t)sbus_rx_buffer;
//    dma.DMA_DIR = DMA_DIR_PeripheralToMemory;
//    dma.DMA_BufferSize = 13;
//    dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//    dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
//    dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
//    dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
//    dma.DMA_Mode = DMA_Mode_Circular;
//    dma.DMA_Priority = DMA_Priority_VeryHigh;
//    dma.DMA_FIFOMode = DMA_FIFOMode_Disable;
//    dma.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
//    dma.DMA_MemoryBurst = DMA_Mode_Normal;
//    dma.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
//    DMA_Init(DMA2_Stream2, &dma);

//    DMA_ITConfig(DMA2_Stream2, DMA_IT_TC, ENABLE);
//    DMA_Cmd(DMA2_Stream2, ENABLE);

//}



//void DMA2_Stream2_IRQHandler(void)
//{
//    if(DMA_GetITStatus(DMA2_Stream2, DMA_IT_TCIF2))  //判断DMA是否结束
//    {
//        DMA_ClearFlag(DMA2_Stream2, DMA_FLAG_TCIF2);		//清楚标志位
//        DMA_ClearITPendingBit(DMA2_Stream2, DMA_IT_TCIF2);

//        RC_Ctl.rc.ch0 = (sbus_rx_buffer[0] | (sbus_rx_buffer[1] << 8)) & 0x07ff;          		//获取通道0数据
//        RC_Ctl.rc.ch1 = ((sbus_rx_buffer[1] >> 3) | (sbus_rx_buffer[2] << 5)) & 0x07ff;   	//获取通道1数据
//        RC_Ctl.rc.ch2 = ((sbus_rx_buffer[2] >> 6) | (sbus_rx_buffer[3] << 2) | (sbus_rx_buffer[4] << 10)) & 0x07ff;   	//获取通道2数据
//        RC_Ctl.rc.ch3 = ((sbus_rx_buffer[4] >> 1) | (sbus_rx_buffer[5] << 7)) & 0x07ff;    	//获取通道3数据
//        RC_Ctl.rc.ch4 = ((sbus_rx_buffer[16]) | (sbus_rx_buffer[17] << 8)) & 0x07FF;
//				RC_Ctl.rc.s1  = ((sbus_rx_buffer[5] >> 4) & 0x000C) >> 2;                          	//获取S1数据
//        RC_Ctl.rc.s2  = ((sbus_rx_buffer[5] >> 4) & 0x0003);		//获取S2数据
//		
//				RC_Ctl.mouse.x = (sbus_rx_buffer[6]) | (sbus_rx_buffer[7] << 8);
//				RC_Ctl.mouse.y = (sbus_rx_buffer[8]) | (sbus_rx_buffer[9] << 8);
//				RC_Ctl.mouse.z = (sbus_rx_buffer[10]) | (sbus_rx_buffer[11] << 8); 
//				RC_Ctl.mouse.press_l = sbus_rx_buffer[12];
//				RC_Ctl.mouse.press_r = sbus_rx_buffer[13];
//				RC_Ctl.key.v = (sbus_rx_buffer[14]);// | (sbus_rx_buffer[15] << 8);
//    }


//}


//void USART1_IRQHandler(void)
//{
//    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
//    {


//        USART_ClearITPendingBit(USART1, USART_IT_RXNE);



//    }
//}



///**
//  ****************************(C) COPYRIGHT 2020 HRBUST_AIR****************************
//  * @file    usart.c
//  * @brief   遥控器接收与数据处理
//  * @author  WJR , HRBUST_AIR_TEAM , website:www.airclub.tech
//  * @Note
//  * @version V1.0.0
//  * @history
//  *  Version    Date            Author          Modification
//  *  V1.0.0     11-18-2020      WJR             Done
//  ****************************(C) COPYRIGHT 2020 HRBUST_AIR****************************
//	* @describe 程序启动入口
//*/
//#include "usart.h"

//volatile unsigned char sbus_rx_buffer[25];
//RC_Ctl_t RC_Ctl;

////const RC_Ctl_t *get_remote_control_point(void)
////{
////     return &RC_Ctl;
////}


//void USART1_Configuration(void)
//{
//
//    USART_InitTypeDef usart1;
//	  GPIO_InitTypeDef  gpio;
//    NVIC_InitTypeDef  nvic;
//		DMA_InitTypeDef   dma;
//
//    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_DMA2,ENABLE);	//使能GPIOB和DMA的时钟
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);												//使能USART1时钟
//
//		GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_USART1); 	//GPIOB7复用为USART1
//
//    gpio.GPIO_Pin = GPIO_Pin_7;
//    gpio.GPIO_Mode = GPIO_Mode_AF;
//    gpio.GPIO_OType = GPIO_OType_PP;
//    gpio.GPIO_Speed = GPIO_Speed_100MHz;
//    gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
//    GPIO_Init(GPIOB,&gpio);
//
//		USART_DeInit(USART1);
//    usart1.USART_BaudRate = 100000;												//波特率为100000，参照遥控器说明书
//		usart1.USART_WordLength = USART_WordLength_8b;				//数据字长度为8位
//    usart1.USART_StopBits = USART_StopBits_1;							//1位停止位
//		usart1.USART_Parity = USART_Parity_Even;							//奇校验
//		usart1.USART_Mode =USART_Mode_Rx;											//读数据
//    usart1.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//    USART_Init(USART1,&usart1);				//初始化USART1

//    USART_ITConfig(USART1,USART_IT_RXNE,DISABLE);
//    USART_Cmd(USART1,ENABLE);			//使能USART1
//
//		USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);		//使能DMA
//
//	  nvic.NVIC_IRQChannel = DMA2_Stream2_IRQn;				//设置中断通道
//    nvic.NVIC_IRQChannelPreemptionPriority = 0;			//设置中断的抢占优先级为0
//    nvic.NVIC_IRQChannelSubPriority = 0;						//设置中断的相应优先级为0
//    nvic.NVIC_IRQChannelCmd = ENABLE;								//IRQ通道使能    IRQ——中断请求
//    NVIC_Init(&nvic);				//根据上面的参数初始化NVIC寄存器
//
//		DMA_Cmd(DMA2_Stream2, DISABLE);
//    while (DMA2_Stream2->CR & DMA_SxCR_EN);
//		DMA_DeInit(DMA2_Stream2);
//    dma.DMA_Channel= DMA_Channel_4;
//    dma.DMA_PeripheralBaseAddr = (uint32_t)&(USART1->DR);
//    dma.DMA_Memory0BaseAddr = (uint32_t)sbus_rx_buffer;
//    dma.DMA_DIR = DMA_DIR_PeripheralToMemory;
//    dma.DMA_BufferSize = 18;
//    dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//    dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
//    dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
//    dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
//    dma.DMA_Mode = DMA_Mode_Circular;
//    dma.DMA_Priority = DMA_Priority_VeryHigh;
//    dma.DMA_FIFOMode = DMA_FIFOMode_Disable;
//    dma.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
//    dma.DMA_MemoryBurst = DMA_Mode_Normal;
//    dma.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
//    DMA_Init(DMA2_Stream2,&dma);

//		DMA_ITConfig(DMA2_Stream2,DMA_IT_TC,ENABLE);
//    DMA_Cmd(DMA2_Stream2,ENABLE);
//
//}



//void DMA2_Stream2_IRQHandler(void)
//{
//    if(DMA_GetITStatus(DMA2_Stream2, DMA_IT_TCIF5))  //判断DMA是否结束
//    {
//        DMA_ClearFlag(DMA2_Stream2, DMA_FLAG_TCIF5);		//清楚标志位
//        DMA_ClearITPendingBit(DMA2_Stream2, DMA_IT_TCIF5);
//
//				RC_Ctl.rc.ch0 = (sbus_rx_buffer[0]| (sbus_rx_buffer[1] << 8)) & 0x07ff;          		//获取通道0数据
//				RC_Ctl.rc.ch1 = ((sbus_rx_buffer[1] >> 3) | (sbus_rx_buffer[2] << 5)) & 0x07ff;   	//获取通道1数据
//				RC_Ctl.rc.ch2 = ((sbus_rx_buffer[2] >> 6) | (sbus_rx_buffer[3] << 2) | (sbus_rx_buffer[4] << 10)) & 0x07ff;   	//获取通道2数据
//				RC_Ctl.rc.ch3 = ((sbus_rx_buffer[4] >> 1) | (sbus_rx_buffer[5] << 7)) & 0x07ff;    	//获取通道3数据
//				RC_Ctl.rc.s1  = ((sbus_rx_buffer[5] >> 4)& 0x000C) >> 2;                           	//获取S1数据
//				RC_Ctl.rc.s2  = ((sbus_rx_buffer[5] >> 4)& 0x0003);    															//获取S2数据
//    }
//
//
//}
//

//	void USART1_IRQHandler(void)
//{
//    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
//    {
//

//			USART_ClearITPendingBit(USART1,USART_IT_RXNE);		//清楚标志位
//
//
//
//  }
//}


