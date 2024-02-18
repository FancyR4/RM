#include "can.h"
#include "ChassisTask.h"
#include "GimbalTask.h"
//#include "LunchTask.h"

CanRX_Chassis_st Chassis_Rx;
CanRx_Shoot_st Shoot_RX;
CanRx_Gimbal_st Gimbal_RX[2];
CanRX_Power_st CanRX_Power;
CanRXMsg_st Msg[20];
CanRXMsgCAP_st MsgCap;

#define P 0
#define Y 1

//extern CanTx_Shoot_st Shoot_TX;
//extern CanTx_Gimbal_st Gimbal_TX;

/*******************初始化CAN1函数*********************/
void CAN1_Configuration(void)
{
    CAN_InitTypeDef        can;
    CAN_FilterInitTypeDef  can_filter;
    GPIO_InitTypeDef       gpio;
    NVIC_InitTypeDef       nvic;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);	//使能GPIOD时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);	//使能CAN1时钟

    GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_CAN1); //PD0是CAN1_RX
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_CAN1); //PD1是CAN1_TX


    gpio.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_0;;
    gpio.GPIO_Mode = GPIO_Mode_AF;			//复用
    GPIO_Init(GPIOD, &gpio);

    /**********NVIC中断初始化***********/
    nvic.NVIC_IRQChannel = CAN1_RX0_IRQn;				//设置中断通道
    nvic.NVIC_IRQChannelPreemptionPriority = 1;	//设置中断的抢占优先级为
    nvic.NVIC_IRQChannelSubPriority = 0;				//设置中断的相应优先级为
    nvic.NVIC_IRQChannelCmd = ENABLE;						//IRQ通道使能    IRQ——中断请求
    NVIC_Init(&nvic);				//根据上面的参数初始化NVIC寄存器

    CAN_StructInit(&can);

    /******CAN1单元设置******/
    can.CAN_TTCM = DISABLE;						//非时间触发通信模式
    can.CAN_ABOM = ENABLE;						//软件自动离线管理
    can.CAN_AWUM = DISABLE;						//睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)
    can.CAN_NART = DISABLE;						//禁止报文自动传送
    can.CAN_RFLM = DISABLE;						//报文不锁定,新的覆盖旧的
    can.CAN_TXFP = DISABLE;						//优先级由报文标识符决定
    can.CAN_Mode = CAN_Mode_Normal;		//模式设置： mode:0,普通模式;1,回环模式；

    /******CAN1波特率设置******/
    can.CAN_SJW  = CAN_SJW_1tq;				//重新同步跳跃宽度(Tsjw)为tsjw+1个时间单位
    can.CAN_BS1 = CAN_BS1_6tq;				//Tbs1=tbs1+1个时间单位CAN_BS1_1tq ~CAN_BS1_16tq
    can.CAN_BS2 = CAN_BS2_2tq;				//Tbs2=tbs2+1个时间单位CAN_BS2_1tq ~	CAN_BS2_8tq
    can.CAN_Prescaler = 5;  					//分频系数(Fdiv)为brp+1
    CAN_Init(CAN1, &can);							//初始化CAN1

    can_filter.CAN_FilterNumber = 0;											//过滤器0
    can_filter.CAN_FilterMode = CAN_FilterMode_IdMask;
    can_filter.CAN_FilterScale = CAN_FilterScale_32bit;		//32位标识符屏蔽位模式
    can_filter.CAN_FilterIdHigh = 0x0000;									//32位标识符ID
    can_filter.CAN_FilterIdLow = 0x0000;
    can_filter.CAN_FilterMaskIdHigh = 0x0000;							//32位的MASK屏蔽位
    can_filter.CAN_FilterMaskIdLow = 0x0000;
    can_filter.CAN_FilterFIFOAssignment = 0;							//过滤器0关联到FIFO0
    can_filter.CAN_FilterActivation = ENABLE;								//激活过滤器0
    CAN_FilterInit(&can_filter); 				//过滤器初始化

    CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);		//开启FMP0中断
    CAN_ITConfig(CAN1, CAN_IT_TME, ENABLE);			//开启TME邮箱中断 TME=0邮箱挂号，TME=1邮箱空置
}


void CAN2_Configuration()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    CAN_InitTypeDef CAN_InitStructure;
    CAN_FilterInitTypeDef CAN_FilterInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);////////
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);

    RCC_APB1PeriphResetCmd(RCC_APB1Periph_CAN2, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_CAN2, DISABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource12, GPIO_AF_CAN2);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_CAN2);

    CAN_InitStructure.CAN_TTCM = DISABLE;
    CAN_InitStructure.CAN_ABOM = ENABLE;
    CAN_InitStructure.CAN_AWUM = DISABLE;
    CAN_InitStructure.CAN_NART = DISABLE;
    CAN_InitStructure.CAN_RFLM = DISABLE;
    CAN_InitStructure.CAN_TXFP = DISABLE;
    CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
    CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
    CAN_InitStructure.CAN_BS1 = CAN_BS1_7tq;
    CAN_InitStructure.CAN_BS2 = CAN_BS2_7tq;
    CAN_InitStructure.CAN_Prescaler = 3;
    CAN_Init(CAN2, &CAN_InitStructure);

    CAN_FilterInitStructure.CAN_FilterNumber = 14;
    CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
    CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
    CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
    CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;
    CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
    CAN_FilterInit(&CAN_FilterInitStructure);

    CAN_ITConfig(CAN2, CAN_IT_FMP0, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


/*****************控制电机函数*****************/
/****************CAN1****************/
static int16_t  Motor[4];
static int16_t Gimbal[2];
static int16_t UpStir;
static uint16_t Power;
int16_t IDen;
void CAN1_TX(int16_t Motor_LEFT_FRON_201,
             int16_t Motor_RIGH_FRON_202,
             int16_t Motor_LEFT_BACK_203,
             int16_t Motor_RIGH_BACK_204,
						 int16_t Motor_Gimbal_Pitch,
						 int16_t Motor_Gimbal_Yaw,
						 int16_t Motor_UpStir,
						uint16_t SuperCap_Power,	
						 int16_t Identify,
						 int16_t STDID)  //I = -32768~327678
{
    CanTxMsg tx_message;
    tx_message.DLC = 8;
    tx_message.RTR = CAN_RTR_DATA;
    tx_message.IDE = CAN_ID_STD;
    tx_message.StdId = STDID;
		if(STDID == 0x200)
		{
		Motor[0] = Motor_LEFT_FRON_201;
		Motor[1] = Motor_RIGH_FRON_202;
		Motor[2] = Motor_LEFT_BACK_203;
		Motor[3] = Motor_RIGH_BACK_204;	
    tx_message.Data[0] = Motor[0] >> 8;     //0x201
    tx_message.Data[1] = Motor[0];
    tx_message.Data[2] = Motor[1] >> 8;     //0x202
    tx_message.Data[3] = Motor[1];
    tx_message.Data[4] = Motor[2] >> 8;     //0x203
    tx_message.Data[5] = Motor[2];
    tx_message.Data[6] = Motor[3] >> 8;     //0x204
    tx_message.Data[7] = Motor[3];
		}
		if(STDID == 0x1FF)
		{
		IDen = Identify; 
	if(IDen == GimbalID)
		{
		Gimbal[0] = Motor_Gimbal_Yaw;
		Gimbal[1] = Motor_Gimbal_Pitch;
		}
		
	if(IDen == UpstirID)
	  {
		UpStir = Motor_UpStir;
		}
    tx_message.Data[0] = Gimbal[0] >> 8;
    tx_message.Data[1] = Gimbal[0];
	  tx_message.Data[2] = Gimbal[1] >> 8;
    tx_message.Data[3] = Gimbal[1];		
		tx_message.Data[4] = UpStir >> 8;
    tx_message.Data[5] = UpStir;
    tx_message.Data[6] = 0;
    tx_message.Data[7] = 0;	
		}
		if(STDID == 0x210)
		{
		SuperCap_Power = Power;	
		tx_message.Data[6] = SuperCap_Power >> 8;
    tx_message.Data[7] = SuperCap_Power;	
		}
    CAN_Transmit(CAN1, &tx_message);
}

//void Gimbal_Upstir_Run(int16_t Motor_Gimbal_Pitch,
//											 int16_t Motor_Gimbal_Yaw,
//											 int16_t Motor_UpStir,
//											 int16_t Identify)
//{
//		CanTxMsg tx_message;
//    tx_message.DLC = 8;
//    tx_message.RTR = CAN_RTR_DATA;
//    tx_message.IDE = CAN_ID_STD;
//    tx_message.StdId = 0x1ff;
//	if(Identify == GimbalID)
//		{
//		Gimbal[0] = Motor_Gimbal_Yaw;
//		Gimbal[1] = Motor_Gimbal_Pitch;
//		}
//    tx_message.Data[0] = Gimbal[0] >> 8;
//    tx_message.Data[1] = Gimbal[0];
//	  tx_message.Data[2] = Gimbal[1] >> 8;
//    tx_message.Data[3] = Gimbal[1];
//		
//	if(Identify == UpstirID)
//	  {
//		UpStir = Motor_UpStir;
//		}		
//		tx_message.Data[4] = UpStir >> 8;
//    tx_message.Data[5] = UpStir;

//    tx_message.Data[6] = 0;
//    tx_message.Data[7] = 0;			
//		
//    CAN_Transmit(CAN1, &tx_message);
//}

//void Upstir_Run(int16_t Motor_UpStir)  //I = -32768~327678
//{
//    CanTxMsg tx_message;
//    tx_message.DLC = 8;
//    tx_message.RTR = CAN_RTR_DATA;
//    tx_message.IDE = CAN_ID_STD;
//    tx_message.StdId = 0x1ff;
////    tx_message.Data[0] = 0;
////    tx_message.Data[1] = 0;
////    tx_message.Data[2] = 0;
////    tx_message.Data[3] = 0;		


//    CAN_Transmit(CAN1, &tx_message);
//}

/****************CAN2****************/

static int16_t Fricmotor[2];
static int16_t DownStir;
void Fric_DownStir_Run(int16_t Motor_FricL,
											 int16_t Motor_FricR,
											 int16_t Motor_DownStir,
											 int16_t Identify)
{
    CanTxMsg tx_message;
    tx_message.DLC = 8;
    tx_message.RTR = CAN_RTR_DATA;
    tx_message.IDE = CAN_ID_STD;
    tx_message.StdId = 0x200;
	
	if(Identify == FricID)
	  {
		Fricmotor[0] = Motor_FricL;
		Fricmotor[1] = Motor_FricR;
		}
	  
	if(Identify == DownstirID)
	  {
		DownStir = Motor_DownStir;
		}
		
		tx_message.Data[0] = Fricmotor[0] >> 8;
    tx_message.Data[1] = Fricmotor[0];
    tx_message.Data[2] = Fricmotor[1] >> 8;
    tx_message.Data[3] = Fricmotor[1];
    tx_message.Data[4] = DownStir >> 8;
    tx_message.Data[5] = DownStir;			
    tx_message.Data[6] = 0;
    tx_message.Data[7] = 0;		
		
    CAN_Transmit(CAN2, &tx_message);	
}

//void DownStir_Run(int16_t Motor_DownStir)
//{
//    CanTxMsg tx_message;
//    tx_message.DLC = 8;
//    tx_message.RTR = CAN_RTR_DATA;
//    tx_message.IDE = CAN_ID_STD;
//    tx_message.StdId = 0x200;

////	  tx_message.Data[0] = 0;
////    tx_message.Data[1] = 0;
////    tx_message.Data[2] = 0;
////    tx_message.Data[3] = 0;		
//    tx_message.Data[4] = Motor_DownStir >> 8;
//    tx_message.Data[5] = Motor_DownStir;
//	
//    CAN_Transmit(CAN2, &tx_message);	
//}

/*****************CAN1的接收中断函数*****************/
void CAN1_RX0_IRQHandler(void)
{
    CanRxMsg can_receive_message;
    if (CAN_GetITStatus(CAN1, CAN_IT_FMP0) != RESET)	//判断是否接收结束
    {
        CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);		//清除FIFO0消息挂号中断标志位
        CAN_Receive(CAN1, CAN_FIFO0, &can_receive_message);	//获取接收数据

        Can1ReceiveMsgProcess(&can_receive_message);       		//将数据通过函数处理
    }
}

void CAN2_RX0_IRQHandler(void)
{

	CanRxMsg Can2RxMessage;
    if (CAN_GetITStatus(CAN2,CAN_IT_FMP0)!= RESET)
	{
	    CAN_ClearITPendingBit(CAN2, CAN_IT_FMP0);//清除中断挂起
		CAN_Receive(CAN2, CAN_FIFO0, &Can2RxMessage);

	    Can2ReceiveMsgProcess(&Can2RxMessage);
	}
}

void Can1ReceiveMsgProcess(CanRxMsg *can_receive_message) 	//传入接收到数据的指针
{
		float angle;
//	CanRxMsg rx_message;
	switch(can_receive_message->StdId)	//判断标识符（根据电调说明书）
	{
		case 0x201:				
					Msg[Motor3508_0].Angle=(can_receive_message->Data[0] << 8) + (can_receive_message->Data[1]);//电机1的机械角度值 
					Msg[Motor3508_0].Speed=(can_receive_message->Data[2] << 8) + (can_receive_message->Data[3]);//电机1的速度值
					Msg[Motor3508_0].Current=(can_receive_message->Data[4] << 8) + (can_receive_message->Data[5]);
			break;
		case 0x202:				
					Msg[Motor3508_1].Angle=(can_receive_message->Data[0] << 8) + (can_receive_message->Data[1]);//电机1的机械角度值 
					Msg[Motor3508_1].Speed=(can_receive_message->Data[2] << 8) + (can_receive_message->Data[3]);//电机1的速度值
					Msg[Motor3508_1].Current=(can_receive_message->Data[4] << 8) + (can_receive_message->Data[5]);	
			break;	
		case 0x203:				
					Msg[Motor3508_2].Angle=(can_receive_message->Data[0] << 8) + (can_receive_message->Data[1]);//电机1的机械角度值 
					Msg[Motor3508_2].Speed=(can_receive_message->Data[2] << 8) + (can_receive_message->Data[3]);//电机1的速度值
					Msg[Motor3508_2].Current=(can_receive_message->Data[4] << 8) + (can_receive_message->Data[5]);	
			break;	
		case 0x204:				
					Msg[Motor3508_3].Angle=(can_receive_message->Data[0] << 8) + (can_receive_message->Data[1]);//电机1的机械角度值 
					Msg[Motor3508_3].Speed=(can_receive_message->Data[2] << 8) + (can_receive_message->Data[3]);//电机1的速度值
					Msg[Motor3508_3].Current=(can_receive_message->Data[4] << 8) + (can_receive_message->Data[5]);	
			break;
		
		case 0x205:	
					Msg[Motor6020_Yaw].Angle=(can_receive_message->Data[0] << 8) + (can_receive_message->Data[1]);//电机1的机械角度值 
					Msg[Motor6020_Yaw].Speed=(can_receive_message->Data[2] << 8) + (can_receive_message->Data[3]);//电机1的速度值
					Msg[Motor6020_Yaw].Current=(can_receive_message->Data[4] << 8) + (can_receive_message->Data[5]);	
			break;	
		case 0x206:			
					Msg[Motor6020_Pitch].Angle=(can_receive_message->Data[0] << 8) + (can_receive_message->Data[1]);//电机1的机械角度值 
					Msg[Motor6020_Pitch].Speed=(can_receive_message->Data[2] << 8) + (can_receive_message->Data[3]);//电机1的速度值
					Msg[Motor6020_Pitch].Current=(can_receive_message->Data[4] << 8) + (can_receive_message->Data[5]);	
			break;
		
		case 0x207:				
					Msg[Motor3508_Magazine].Angle=(can_receive_message->Data[0] << 8) + (can_receive_message->Data[1]);//电机1的机械角度值 
					Msg[Motor3508_Magazine].Speed=(can_receive_message->Data[2] << 8) + (can_receive_message->Data[3]);//电机1的速度值
					Msg[Motor3508_Magazine].Current=(can_receive_message->Data[4] << 8) + (can_receive_message->Data[5]);	
			break;
		
		//NULL		
		case 0x208:				
					Msg[7].Angle=(can_receive_message->Data[0] << 8) + (can_receive_message->Data[1]);//电机1的机械角度值 
					Msg[7].Speed=(can_receive_message->Data[2] << 8) + (can_receive_message->Data[3]);//电机1的速度值
					Msg[7].Current=(can_receive_message->Data[4] << 8) + (can_receive_message->Data[5]);	
			break;
		case 0x209:				
					Msg[16].Angle=(can_receive_message->Data[0] << 8) + (can_receive_message->Data[1]);//电机1的机械角度值 
					Msg[16].Speed=(can_receive_message->Data[2] << 8) + (can_receive_message->Data[3]);//电机1的速度值
					Msg[16].Current=(can_receive_message->Data[4] << 8) + (can_receive_message->Data[5]);	
			break;
		case 0x20A:				
					Msg[17].Angle=(can_receive_message->Data[0] << 8) + (can_receive_message->Data[1]);//电机1的机械角度值 
					Msg[17].Speed=(can_receive_message->Data[2] << 8) + (can_receive_message->Data[3]);//电机1的速度值
					Msg[17].Current=(can_receive_message->Data[4] << 8) + (can_receive_message->Data[5]);	
			break;
		//NULL
		
		case 0x211:				
					MsgCap.Input_Voltage = (uint16_t)((can_receive_message->Data[1] << 8) + (can_receive_message->Data[0]));//输入电压
					MsgCap.Cap_Voltage =   (uint16_t)((can_receive_message->Data[3] << 8) + (can_receive_message->Data[2]));//电容电压
					MsgCap.Input_Current=  (uint16_t)((can_receive_message->Data[5] << 8) + (can_receive_message->Data[4]));//输入电流	
					MsgCap.Output_Power=   (uint16_t)((can_receive_message->Data[7] << 8) + (can_receive_message->Data[6]));//目标功率	
			break;
		
		}
}

void Can2ReceiveMsgProcess(CanRxMsg *can_receive_message) 	//传入接收到数据的指针
{
		float angle;
//	CanRxMsg rx_message;
	switch(can_receive_message->StdId)	//判断标识符（根据电调说明书）
	{
		case 0x201:				
					Msg[Motor3508_FricL].Angle=(can_receive_message->Data[0] << 8) + (can_receive_message->Data[1]);//电机1的机械角度值 
					Msg[Motor3508_FricL].Speed=(can_receive_message->Data[2] << 8) + (can_receive_message->Data[3]);//电机1的速度值
					Msg[Motor3508_FricL].Current=(can_receive_message->Data[4] << 8) + (can_receive_message->Data[5]);
			break;
		case 0x202:				
					Msg[Motor3508_FricR].Angle=(can_receive_message->Data[0] << 8) + (can_receive_message->Data[1]);//电机1的机械角度值 
					Msg[Motor3508_FricR].Speed=(can_receive_message->Data[2] << 8) + (can_receive_message->Data[3]);//电机1的速度值
					Msg[Motor3508_FricR].Current=(can_receive_message->Data[4] << 8) + (can_receive_message->Data[5]);	
			break;
		
		case 0x203:				
					Msg[Motor2006_BD].Angle=(can_receive_message->Data[0] << 8) + (can_receive_message->Data[1]);//电机1的机械角度值 
					Msg[Motor2006_BD].Speed=(can_receive_message->Data[2] << 8) + (can_receive_message->Data[3]);//电机1的速度值
					Msg[Motor2006_BD].Current=(can_receive_message->Data[4] << 8) + (can_receive_message->Data[5]);	
			break;
		
		case 0x204:				
					Msg[11].Angle=(can_receive_message->Data[0] << 8) + (can_receive_message->Data[1]);//电机1的机械角度值 
					Msg[11].Speed=(can_receive_message->Data[2] << 8) + (can_receive_message->Data[3]);//电机1的速度值
					Msg[11].Current=(can_receive_message->Data[4] << 8) + (can_receive_message->Data[5]);	
			break;	
		case 0x205:	
					Msg[12].Angle=(can_receive_message->Data[0] << 8) + (can_receive_message->Data[1]);//电机1的机械角度值 
					Msg[12].Speed=(can_receive_message->Data[2] << 8) + (can_receive_message->Data[3]);//电机1的速度值
					Msg[12].Current=(can_receive_message->Data[4] << 8) + (can_receive_message->Data[5]);	
			break;	
		case 0x206:			
					Msg[13].Angle=(can_receive_message->Data[0] << 8) + (can_receive_message->Data[1]);//电机1的机械角度值 
					Msg[13].Speed=(can_receive_message->Data[2] << 8) + (can_receive_message->Data[3]);//电机1的速度值
					Msg[13].Current=(can_receive_message->Data[4] << 8) + (can_receive_message->Data[5]);	
			break;	
		case 0x207:				
					Msg[14].Angle=(can_receive_message->Data[0] << 8) + (can_receive_message->Data[1]);//电机1的机械角度值 
					Msg[14].Speed=(can_receive_message->Data[2] << 8) + (can_receive_message->Data[3]);//电机1的速度值
					Msg[14].Current=(can_receive_message->Data[4] << 8) + (can_receive_message->Data[5]);	
			break;	
		case 0x208:				
					Msg[15].Angle=(can_receive_message->Data[0] << 8) + (can_receive_message->Data[1]);//电机1的机械角度值 
					Msg[15].Speed=(can_receive_message->Data[2] << 8) + (can_receive_message->Data[3]);//电机1的速度值
					Msg[15].Current=(can_receive_message->Data[4] << 8) + (can_receive_message->Data[5]);	
			break;	
		
		}
}



/*****************CAN1的数据处理函数*****************/
//void Can1ReceiveMsgProcess(CanRxMsg *can_receive_message)
//{
//    switch(can_receive_message->StdId)	//判断标识符发送频率：1KHz 机械角度值范围：0~8191对应0~360  标识符 0x204+电调ID
//    {
//    /*
//    Chassis data process
//    */
//    case 0x201:
//        Chassis_Rx.Motor_LEFT_FRON_201_Speed_RX = (can_receive_message->Data[2] << 8) | (can_receive_message->Data[3]);
//        //CHASSIS_Update_MotorSpeed(LEFT_FRON_201, speed_measure);
//        break;
//    case 0x202:
//        Chassis_Rx.Motor_RIGH_FRON_202_Speed_RX = (can_receive_message->Data[2] << 8) | (can_receive_message->Data[3]);
//        //CHASSIS_Update_MotorSpeed(RIGH_FRON_202, speed_measure);
//        break;
//    case 0x203:
//        Chassis_Rx.Motor_LEFT_BACK_203_Speed_RX = (can_receive_message->Data[2] << 8) | (can_receive_message->Data[3]);
//        //CHASSIS_Update_MotorSpeed(LEFT_BACK_203, speed_measure);
//        break;
//    case 0x204:
//        Chassis_Rx.Motor_RIGH_BACK_204_Speed_RX = (can_receive_message->Data[2] << 8) | (can_receive_message->Data[3]);
//        //CHASSIS_Update_MotorSpeed(RIGH_BACK_204, speed_measure);
//        break;

//    //Gimbal Data Process
//    case 0x205:
//        Gimbal_RX[Y].Gimbal_Speed_RX = (can_receive_message->Data[2] << 8) | (can_receive_message->Data[3]);
//        Gimbal_RX[Y].Gimbal_Angle_RX = (can_receive_message->Data[0] << 8) | (can_receive_message->Data[1]);				
//        break;
//    case 0x206:
//        Gimbal_RX[P].Gimbal_Speed_RX = (can_receive_message->Data[2] << 8) | (can_receive_message->Data[3]);
//        Gimbal_RX[P].Gimbal_Angle_RX = (can_receive_message->Data[0] << 8) | (can_receive_message->Data[1]);				
//        break;
//		//Upstir data process
//    case 0x207:
//        Shoot_RX.UpStir_Motor_Speed_RX = (can_receive_message->Data[2] << 8 ) | (can_receive_message->Data[3]);
//        Shoot_RX.UpStir_Motor_Angle_RX = (can_receive_message->Data[0] << 8 ) | (can_receive_message->Data[1]);
//        break;
//		//SuperCap data process
//		case 0x211:
//				CanRX_Power.InputVoltage     = (float)can_receive_message->Data[0] / 100.0f;
//				CanRX_Power.CapacitorVoltage = (float)can_receive_message->Data[1] / 100.0f;
//				CanRX_Power.InputCurrent     = (float)can_receive_message->Data[2] / 100.0f;
//				CanRX_Power.TargetPower      = (float)can_receive_message->Data[3] / 100.0f;
//				break;
//    }
//}


//void Can2ReceiveMsgProcess(CanRxMsg *can_receive_message)
//{
//switch(can_receive_message->StdId)	//判断标识符发送频率：1KHz 机械角度值范围：0~8191对应0~360  标识符 0x204+电调ID
//    {
//		case 0x201:
//        Shoot_RX.FricL_Motor_Speed_RX = (can_receive_message->Data[2] << 8 ) | (can_receive_message->Data[3]);
//				break;
//		case 0x202:
//				Shoot_RX.FricR_Motor_Speed_RX = (can_receive_message->Data[2] << 8 ) | (can_receive_message->Data[3]);
//			  break;
//		case 0x203:
//        Shoot_RX.DownStir_Motor_Speed_RX = (can_receive_message->Data[2] << 8 ) | (can_receive_message->Data[3]);
//        Shoot_RX.DownStir_Motor_Angle_RX = (can_receive_message->Data[0] << 8 ) | (can_receive_message->Data[1]);			
//				break;
//		}
//}

CanRXMsg_st* CanRecvMsg_CallBack(CanDev_en CanDev)
{
	if(CanDev == Motor3508_0)
		return &Msg[Motor3508_0];
	
	if(CanDev == Motor3508_1)
		return &Msg[Motor3508_1];	
	
	if(CanDev == Motor3508_2)
		return &Msg[Motor3508_2];
	
	if(CanDev == Motor3508_3)
		return &Msg[Motor3508_3];
	
	if(CanDev == Motor6020_Yaw)
		return &Msg[Motor6020_Yaw];
	
	if(CanDev == Motor6020_Pitch)
		return &Msg[Motor6020_Pitch];
	
	if(CanDev == Motor3508_FricL)
		return &Msg[Motor3508_FricL];
	
	if(CanDev == Motor3508_FricR)
		return &Msg[Motor3508_FricR];
	
	if(CanDev == Motor2006_BD)
		return &Msg[Motor2006_BD];
	
	if(CanDev == Motor3508_Magazine)
		return &Msg[Motor3508_Magazine];
	
	if(CanDev == SuperCap_)
		return &Msg[SuperCap_];
//	if(CanDev == )
//		return &Msg[];	
}


