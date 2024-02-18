#include "SysCtrl.h"
#include "RemoteTask.h"

//#include "LunchTask.h"
#include "can.h"

#define OFFSETOF(structure, member) ((int) &((structure *)0)->member)

u8 InitFlag;
//u8 InitFlag = FirstInit;
extern volatile  RC_Ctl_t RC_Ctl;

void AllInit()
{
//设备初始化

//参数初始化
  //FricInit();
//电机都输入归零
	
//检查控制模式
	
}

/**
  * @brief  Check Everything is ok
  * @param  
  * @note   reset InitFlag
  * @retval 
  */
void InitCallback()
{
  //InitFlag = AlreadyInit;
	//while (InitFlag != AlreadyInit)
	//{
	 
	//}
}

ControlState CheckControlOnline(ControlWay_en SelectControlWay)
{

	switch(SelectControlWay)
{
	 case RC:
	 {
		 if ((RC_Ctl.rc.ch1 != 0x00) &&
		 (RC_Ctl.rc.ch2 != 0x00) &&
		 (RC_Ctl.rc.ch3 != 0x00) &&
		 (RC_Ctl.rc.ch4 != 0x00) &&
     (RC_Ctl.rc.ch0 != 0x00)   )
				return Online;
			else
				return Offline;
	 }
	 break;
	 case PC:
	 {
		 if ((RC_Ctl.mouse.x != 0x00) &&
		 (RC_Ctl.mouse.x != 0x00) &&
		 (RC_Ctl.mouse.y != 0x00) &&
		 (RC_Ctl.mouse.z != 0x00)   )
				return Online;
			else
				return Offline;	 
	 }
	 break;
	 default:
		 
	 break;
}
}


//#define DEBUGLEVEL1	(1<<0)
//#define DEBUGLEVEL2	(1<<1)
//#define DEBUGLEVEL3	(1<<2)
//extern unsigned short debug_level;
//void Send2Uart ( char *buffer );

//int FunctionDebug( int value1,int value2,int value3 )
//{     	
//if( debug_level )	
//{
//char buffer[256];
//if( debug_level&DEBUGLEVEL1 )
//{			
//sprintf( buffer,"%s,%d, value=%d\r\n", _FILE_, _LINE_, value1 );
//Send2Uart( buffer );		
//}		
//if( debug_level&DEBUGLEVEL2 )		
//{			
//sprintf( buffer,"%s,%d, value=%d\r\n", _FILE_, _LINE_, value2 );
//Send2Uart( buffer );		
//}		
//if( debug_level&DEBUGLEVEL3 )		
//{			
//sprintf( buffer,"%s,%d, value=%d\r\n", _FILE_, _LINE_, value3 );			
//Send2Uart( buffer );		
//}	
//}
//return 0;
//}


float Q_rsqrt( float number )
{
	long i;
	float x2, y;
	const float threehalfs = 1.5F;
	x2 = number * 0.5F;
	y  = number;
	i  = * ( long * ) &y;                       // evil floating point bit level hacking（对浮点数的邪恶位级hack）
	i  = 0x5f3759df - ( i >> 1 );               // what the fuck?（这他妈的是怎么回事？
	y  = * ( float * ) &i;
	y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration （第一次牛顿迭代
//      y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed（第二次迭代，可以删除）
	return y;
}




