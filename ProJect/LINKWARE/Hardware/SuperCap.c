#include "SuperCap.h"
#include "RemoteTask.h"
#include "Judge_Data.h"
#include "can.h"

extern volatile  RC_Ctl_t RC_Ctl;
extern DJI_Judge_t DJI_Judge;
extern CanRXMsgCAP_st MsgCap;
extern CanRXMsg_st CanRXMsg[20];

float Vx=0.0,Vy=0.0,Vz=0.0;

int16_t PowerIN;

void InvMecanumCal(float *vx,float *vy,float *vz, CanRXMsg_st *chassis)
{
	//更新当前速度
	*vx = (chassis[0].Speed - chassis[1].Speed + chassis[2].Speed - chassis[3].Speed);
	*vy = (chassis[0].Speed + chassis[1].Speed - chassis[2].Speed - chassis[3].Speed);
	*vz = (chassis[0].Speed - chassis[1].Speed + chassis[2].Speed - chassis[3].Speed);
}

void SuperCapCtrl()
{
	InvMecanumCal(&Vx, &Vy, &Vz, CanRXMsg);	
  if (IF_KEY_PRESSED_SHIFT)  //按住SHIFT可以加速
  {
		//超级电容开始放电
  }
  else 
  {
	  //读取裁判系统底盘剩余热量
	  if(DJI_Judge.DJI_Judge_Mes.Judge_power_heat_data.chassis_power_buffer > 45)
	  {
			 //在热量不超的情况下判别当前速度 
			 if((abs(Vx) > ) &&
				  (abs(Vy) > ) &&
					(abs(Vz) > )   )
			 {
		
			 }
			 else
			 {
		
			 } 
	  }
	  else
	  {
	 
	  }		 
	
  }
}

void SetCapPower(float SetPower)
{
	
}

void CapIN()
{
	//如果电容电压小于最大值
	if(MsgCap.Cap_Voltage < MAXCapVoltage)
	{
		//可以通过车跑慢储能
		SetCapPower();
	}
}

void CapPowerIN()
{
	//当前功率
	PowerIN = MsgCap.Input_Voltage * MsgCap.Input_Current;
}


void CapOUT()
{
	if(MsgCap.Cap_Voltage > 1200) //放电不能全部放完
	{
		SetCapPower();
	}
	else SetCapPower(10);
}

void SuperCapCtrl()
{
	if(IF_KEY_PRESSED_SHIFT)
	{
		//设定一个大的输出功率持续输出但是比输入功率肯定小
		SetCapPower();
	}
	else
	{
		//只要热量缓存够,能正常运动就一直充能
		if(DJI_Judge.DJI_Judge_Mes.Judge_power_heat_data.chassis_power_buffer > 45)
		{
			if()
			{
				//随时都在充能只要底盘不超功率
				//CapIN();
			}
			else
			{
				
			}		
		}
	}
}

