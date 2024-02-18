#include "RemoteTask.h"

#include "usartBus.h"

extern unsigned char sbus_rx_buffer[25]; 

#define SBUS_RX_BUF_NUM 36u

#define RC_FRAME_LENGTH 18u

//接收原始数据，为18个字节，给了36个字节长度，防止DMA传输越界
static uint8_t SBUS_rx_buf[2][SBUS_RX_BUF_NUM];

void remote_control_init(void)
{
    Usart1_Dbus_Init(SBUS_rx_buf[0], SBUS_rx_buf[1], SBUS_RX_BUF_NUM);
}

/*
*************遥控器数据结构体*************
@Note 	需要使用遥控器相关数据的时候
				需要先引用其他页面中的RC_Ctl变量
				extern RC_Ctl_t RC_Ctl;
				然后直接用RC_Ctl结构体中的数据即可
******************************************
*/
RC_Ctl_t RC_Ctl;
//****************************************

void SBUS_TO_RC(volatile const uint8_t *sbus_buf, RC_Ctl_t *rc_ctrl)
{
    if (sbus_buf == NULL || rc_ctrl == NULL)
    {
        return;
    }

				rc_ctrl->rc.ch0 = (sbus_buf[0]| (sbus_buf[1] << 8)) & 0x07ff;          	//
				rc_ctrl->rc.ch1 = ((sbus_buf[1] >> 3) | (sbus_buf[2] << 5)) & 0x07ff;   //    
				rc_ctrl->rc.ch2 = ((sbus_buf[2] >> 6) | (sbus_buf[3] << 2) | (sbus_buf[4] << 10)) & 0x07ff;          
				rc_ctrl->rc.ch3 = ((sbus_buf[4] >> 1) | (sbus_buf[5] << 7)) & 0x07ff;   //  
				rc_ctrl->rc.ch4 = sbus_buf[16] | (sbus_buf[17] << 8);                 	//左上角波轮      
				rc_ctrl->rc.s1  = ((sbus_buf[5] >> 4)& 0x000C) >> 2;                          // 
				rc_ctrl->rc.s2  = ((sbus_buf[5] >> 4)& 0x0003);    														//
				
			
				rc_ctrl->mouse.x = sbus_buf[6] | (sbus_buf[7] << 8);                    //鼠标x轴        
				rc_ctrl->mouse.y = sbus_buf[8] | (sbus_buf[9] << 8);                    //鼠标y轴     
				rc_ctrl->mouse.z = sbus_buf[10] | (sbus_buf[11] << 8);                  //鼠标Z轴         
				rc_ctrl->mouse.press_l = sbus_buf[12];                                        //鼠标左键  
				rc_ctrl->mouse.press_r = sbus_buf[13];                                        //鼠标右键
				rc_ctrl->key.v = sbus_buf[14] | (sbus_buf[15] << 8);   									//键盘按键值					
}

void USART1_IRQHandler(void)
{
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        USART_ReceiveData(USART1);
    }
    else if (USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)
    {
        static uint16_t this_time_rx_len = 0;
        USART_ReceiveData(USART1);

        if(DMA_GetCurrentMemoryTarget(DMA2_Stream2) == 0)
        {
            //重新设置DMA
            DMA_Cmd(DMA2_Stream2, DISABLE);
            this_time_rx_len = SBUS_RX_BUF_NUM - DMA_GetCurrDataCounter(DMA2_Stream2);
            DMA_SetCurrDataCounter(DMA2_Stream2, SBUS_RX_BUF_NUM);
            DMA2_Stream2->CR |= DMA_SxCR_CT;
            //清DMA中断标志
            DMA_ClearFlag(DMA2_Stream2, DMA_FLAG_TCIF2 | DMA_FLAG_HTIF2);
            DMA_Cmd(DMA2_Stream2, ENABLE);
            if(this_time_rx_len == RC_FRAME_LENGTH)
            {
                //处理遥控器数据
                SBUS_TO_RC(SBUS_rx_buf[0], &RC_Ctl);
            }
        }
        else
        {
            //重新设置DMA
            DMA_Cmd(DMA2_Stream2, DISABLE);
            this_time_rx_len = SBUS_RX_BUF_NUM - DMA_GetCurrDataCounter(DMA2_Stream2);
            DMA_SetCurrDataCounter(DMA2_Stream2, SBUS_RX_BUF_NUM);
            DMA2_Stream2->CR &= ~(DMA_SxCR_CT);
            //清DMA中断标志
            DMA_ClearFlag(DMA2_Stream2, DMA_FLAG_TCIF2 | DMA_FLAG_HTIF2);
            DMA_Cmd(DMA2_Stream2, ENABLE);
            if(this_time_rx_len == RC_FRAME_LENGTH)
            {
                //处理遥控器数据
                SBUS_TO_RC(SBUS_rx_buf[1], &RC_Ctl);
            }

        }
    }
}

//void RC_Restart_Set(void)
//{
//	 RC_restart(SBUS_RX_BUF_NUM);
//}

const RC_Ctl_t *Get_DJI_RC_Data_Address(void)
{
	return &RC_Ctl;
}


