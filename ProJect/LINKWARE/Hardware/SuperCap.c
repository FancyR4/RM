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
	//���µ�ǰ�ٶ�
	*vx = (chassis[0].Speed - chassis[1].Speed + chassis[2].Speed - chassis[3].Speed);
	*vy = (chassis[0].Speed + chassis[1].Speed - chassis[2].Speed - chassis[3].Speed);
	*vz = (chassis[0].Speed - chassis[1].Speed + chassis[2].Speed - chassis[3].Speed);
}

void SuperCapCtrl()
{
	InvMecanumCal(&Vx, &Vy, &Vz, CanRXMsg);	
  if (IF_KEY_PRESSED_SHIFT)  //��סSHIFT���Լ���
  {
		//�������ݿ�ʼ�ŵ�
  }
  else 
  {
	  //��ȡ����ϵͳ����ʣ������
	  if(DJI_Judge.DJI_Judge_Mes.Judge_power_heat_data.chassis_power_buffer > 45)
	  {
			 //������������������б�ǰ�ٶ� 
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
	//������ݵ�ѹС�����ֵ
	if(MsgCap.Cap_Voltage < MAXCapVoltage)
	{
		//����ͨ������������
		SetCapPower();
	}
}

void CapPowerIN()
{
	//��ǰ����
	PowerIN = MsgCap.Input_Voltage * MsgCap.Input_Current;
}


void CapOUT()
{
	if(MsgCap.Cap_Voltage > 1200) //�ŵ粻��ȫ������
	{
		SetCapPower();
	}
	else SetCapPower(10);
}

void SuperCapCtrl()
{
	if(IF_KEY_PRESSED_SHIFT)
	{
		//�趨һ�����������ʳ���������Ǳ����빦�ʿ϶�С
		SetCapPower();
	}
	else
	{
		//ֻҪ�������湻,�������˶���һֱ����
		if(DJI_Judge.DJI_Judge_Mes.Judge_power_heat_data.chassis_power_buffer > 45)
		{
			if()
			{
				//��ʱ���ڳ���ֻҪ���̲�������
				//CapIN();
			}
			else
			{
				
			}		
		}
	}
}

