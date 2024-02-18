#include "pid.h"
//#include "LunchTask.h"
#include "GimbalTask.h"
#include "user_lib.h"


//<3200rpm


ChassisPidInformation Chassis_Pid_Motor[4];


/*限幅函数*/
void PID_limit(float *a, float PID_MAX)
{
    if(*a > PID_MAX)
        *a = PID_MAX;
    if(*a < -PID_MAX)
        *a = -PID_MAX;
}
/*单边限幅*/
void PID_limit_(float *a, float PID_MAX)
{
    if(*a > PID_MAX)
        *a = PID_MAX;
    if(*a < 0)
        *a = 0;
}
/*绝对值函数abs*/
void PID_juedui(float *a)
{
    if(*a > 0)
        *a = *a;
    if(*a < 0)
        *a = -*a;
}

/*引入变速积分*/
float VariableSpeedIntegral(float A, float B, float error)
{
	 float factor;
	 B = abs(B);
	 A = abs(A);
	 if(abs(error) <= B)
	 {
	    factor = 1;
			return factor;
	 }
		else if((abs(error) >= B) && (abs(error) < (A+B)))
		{
			factor = (A+B-abs(error))/A;
			return factor;
		}
		else 
		{
			factor = 0;
			return factor;
		}
}

/********************************Chassis********************************/

void CHASSIS_PID_Parameter(ChassisPidInformation *PID)
{

    PID[0].Kp = 4;
    PID[0].Ki = 1;
    PID[0].Kd = 0;

    PID[1].Kp = 4;
    PID[1].Ki = 1;
    PID[1].Kd = 0;

    PID[2].Kp = 4;
    PID[2].Ki = 1;
    PID[2].Kd = 0;

    PID[3].Kp = 4;
    PID[3].Ki = 1;
    PID[3].Kd = 0;
	
    PID[0].Now_Error = 0;
    PID[1].Now_Error = 0;
    PID[2].Now_Error = 0;	
    PID[3].Now_Error = 0;
		PID[0].Last_Error = 0;
    PID[1].Last_Error = 0;
    PID[2].Last_Error = 0;	
    PID[3].Last_Error = 0;
		PID[0].Last_Last_Error = 0;
    PID[1].Last_Last_Error = 0;
    PID[2].Last_Last_Error = 0;	
    PID[3].Last_Last_Error = 0;	
}


float PID_Integral(ChassisPidInformation *PID, emTYPE_ChassisWheel Wheel, int umax, int umin, int deadzone)
{
	int index;
	if(PID[Wheel].Now>umax) //抗积分饱和的实现：只累加负偏差
{
   if(abs(PID[Wheel].Now_Error)>deadzone) //积分分离过程
   { 
      index=0;
    }
   else{ 
      index=1; 
      if(PID[Wheel].Now_Error<0) 
     { 
        PID[Wheel].Iout += PID[Wheel].Ki * PID[Wheel].Now_Error;; 
      } 
   } 
}else if(PID[Wheel].Now<umin){  //抗积分饱和的实现：只累加正偏差

       if(abs(PID[Wheel].Now_Error)>deadzone) //积分分离过程 
       { 
         index=0; 
       }
       else{ 
         index=1; 
         if(PID[Wheel].Now_Error>0) 
          { 
            PID[Wheel].Iout += PID[Wheel].Ki * PID[Wheel].Now_Error;; 
          } 
} 
}else{ //抗积分饱和的实现 
   
    if(abs(PID[Wheel].Now_Error)>deadzone) //积分分离过程 
     { 
       index=0; 
     }
    else{ 
       index=1; 
       PID[Wheel].Iout += PID[Wheel].Ki * PID[Wheel].Now_Error;
     } 
}
	return PID[Wheel].Iout;
}

void RPM_Limit()
{
	
}

float CHASSIS_PID_Calc(ChassisPidInformation *PID, float ref, float set, emTYPE_ChassisWheel Wheel)
{

    int i;
    //if ()
    PID[Wheel].Set = set;
    PID[Wheel].Now = ref;
    PID[Wheel].Now_Error = set - ref;
	if((PID[Wheel].Now_Error<20)&&(PID[Wheel].Now_Error>-20))
		PID[Wheel].Now_Error = 0;

    for(i = 1; i <= 0; i--)
    PID[Wheel].Error_Buf[i+1] = PID[Wheel].Error_Buf[i];

    PID[Wheel].Pout = PID[Wheel].Kp * (PID[Wheel].Now_Error - PID[Wheel].Last_Error);

    //PID[Wheel].Iout += PID[Wheel].Ki * PID[Wheel].Now_Error;
	PID[Wheel].Iout = PID_Integral(PID, Wheel, 7000, -7000, 8000);
	PID_limit(&PID[Wheel].Iout, 3000);

    PID[Wheel].Dout = PID[Wheel].Kd * (PID[Wheel].Now_Error
                                    -2*PID[Wheel].Last_Error + PID[Wheel].Last_Last_Error);
	
	PID[Wheel].Error_Buf[0] = PID[Wheel].Now_Error;
	PID[Wheel].Last_Error = PID[Wheel].Error_Buf[1];
	PID[Wheel].Last_Last_Error = PID[Wheel].Error_Buf[2];
    
    PID[Wheel].Out = PID[Wheel].Pout + PID[Wheel].Iout + PID[Wheel].Dout;

    PID_limit(&PID[Wheel].Out, 25000);

    return PID[Wheel].Out;

}

/********************************Chassis********************************/

/*********************************Lunch*********************************/

UpstirPIDinformation Upstir;

void Upstir_PID_Parameter(UpstirPIDinformation *PID)
{
    PID->Kp = 1.00;
    PID->Ki = 0.2;
    PID->Kd = 0.3;	

	PID->Now_Error = 0;
	PID->Last_Error = 0;
    PID->Last_Last_Error = 0;	  
}

float Upstir_PID_Calc(UpstirPIDinformation *PID, float ref, float set)
{

    int i;
    //if ()
    PID->Now_Error = set - ref;
    PID->Last_Error = PID->Error_buf[0];

    for(i = 1; i <= 0; i--)
        PID->Error_buf[i + 1] = PID->Error_buf[i];
    //if ()

    PID->Pout = PID->Kp * (PID->Now_Error - PID->Last_Error);

    PID->Iout += PID->Ki * PID->Now_Error;


    PID->Dout = PID->Kd * (PID->Now_Error - 2 * PID->Now_Error + PID->Last_Last_Error);

    PID->Error_buf[0] = PID->Now_Error;
    PID->Last_Error = PID->Error_buf[1];
    PID->Last_Last_Error = PID->Error_buf[2];

    PID_limit(&PID->Iout, 5000);
    PID->Current_Out = PID->Pout + PID->Iout + PID->Dout;

    PID_limit_(&PID->Current_Out, 12000);  //2500

    return PID->Current_Out;
}


DownstirPIDinformation DownstirPID;

void Downstir_PID_Parameter(DownstirPIDinformation *PID)
{
	PID->Now_Error = 0;
	PID->Last_Error = 0;
  PID->Last_Last_Error = 0;	 
}

//3 -1 0 10 1 5
//float p,i,d,p_,i_,d_;
float Downstir_PID_Calc(DownstirPIDinformation *PID, float ref, float set, DownstirMode_en mode)
{

	//PID RUN/HALT
	float P_I_D[3][3] = {{1.0, 0.03, 0.0},{1, 0, 0},{1.0, 0.2, 0.3}};
	
	if(mode == downstirpidrun){
	   PID->Kp = P_I_D[0][0];
     PID->Ki = P_I_D[0][1];
     PID->Kd = P_I_D[0][2];
	   PID->Last_Error = 0;
     PID->Last_Last_Error = 0;
	   PID->Error_buf[0] = 0;
	   PID->Error_buf[1] = 0;
	   PID->Error_buf[2] = 0;		
		 set = -850;
	}
	if(mode == downstirpidstop){
	   PID->Kp = P_I_D[1][0];
     PID->Ki = P_I_D[1][1];
     PID->Kd = P_I_D[1][2];
	   PID->Last_Error = 0;
     PID->Last_Last_Error = 0;
	   PID->Error_buf[0] = 0;
	   PID->Error_buf[1] = 0;
	   PID->Error_buf[2] = 0;
		 set = 0;		
	}
	if(mode == downstirslide){
		 PID->Kp = P_I_D[2][0];
     PID->Ki = P_I_D[2][1];
     PID->Kd = P_I_D[2][2];
	   PID->Last_Error = 0;
     PID->Last_Last_Error = 0;
	   PID->Error_buf[0] = 0;
	   PID->Error_buf[1] = 0;
	   PID->Error_buf[2] = 0;
		 set = -150;	
	}		
	
    int i;
    //if ()
    PID->Now_Error = set - ref;
    PID->Last_Error = PID->Error_buf[0];

    for(i = 1; i <= 0; i--)
        PID->Error_buf[i + 1] = PID->Error_buf[i];
    //if ()

    PID->Pout = PID->Kp * (PID->Now_Error - PID->Last_Error);

    PID->Iout += PID->Ki * PID->Now_Error;

    PID->Dout = PID->Kd * (PID->Now_Error - 2 * PID->Now_Error + PID->Last_Last_Error);

    PID->Error_buf[0] = PID->Now_Error;
    PID->Last_Error = PID->Error_buf[1];
    PID->Last_Last_Error = PID->Error_buf[2];

    PID_limit(&PID->Iout, 20000);
    PID->Current_Out = PID->Pout + PID->Iout + PID->Dout;

    PID_limit(&PID->Current_Out, 20000);

    return PID->Current_Out;

}	


FricPIDinformation Fric[2];
ramp_function_source_t KP;

void Fric_PID_Parameter(FricPIDinformation *PID)
{
    PID[0].Kp = 5.0;
    PID[0].Ki = 0.2;
    PID[0].Kd = 0.7;//0.3
    
		PID[1].Kp = 5.0;
    PID[1].Ki = 0.3;
    PID[1].Kd = 1.0;//0.3	

		PID[0].Now_Error = 0;
    PID[1].Now_Error = 0;
		PID[0].Last_Error = 0;
    PID[1].Last_Error = 0;
		PID[0].Last_Last_Error = 0;
    PID[1].Last_Last_Error = 0;
		ramp_init(&KP, 0.05, 2000, 5); //1000
}

float Fric_PID_Calc(FricPIDinformation *PID, float ref, float set, FricSelect_en Select, int16_t Hlat)
{
	
		int i;
		if(Hlat == ENABLE)
		{
		 set = 0;
		 PID[Select].Kp = 10.0;
		 PID[Select].Ki = 1;
		 PID[Select].Kd = 0;
		 PID[Select].Pout = 0;
		 PID[Select].Iout = 0;
		 PID[Select].Dout = 0;
     PID[Select].Now_Error = 0;
     PID[Select].Last_Error = 0;
     PID[Select].Last_Last_Error = 0;
		}
		else
		{
		 if (Select == FRICL)
			 {
				PID[Select].Kp = 5.0;
				PID[Select].Ki = 0.2;
				PID[Select].Kd = 0.7;
			 }
		 if (Select == FRICR)
			 {
				PID[Select].Kp = 5.0;
				PID[Select].Ki = 0.3;
				PID[Select].Kd = 1.0;			 
		   }
		}
		PID[Select].measure = ref;
		PID[Select].give = set;
    PID[Select].Now_Error = set - ref;
    PID[Select].Last_Error = PID[Select].Error_buf[0];

    for(i = 1; i <= 0; i--)
        PID[Select].Error_buf[i + 1] = PID[Select].Error_buf[i];
    //if ()

		if ((abs(PID[Select].Now_Error) > 120) && (Hlat != ENABLE))
		{
			//PID[Select].Kp = 200;
			ramp_calc(&KP, PID[Select].Kp);
			PID[Select].Kp = KP.out;
		}
		else PID[Select].Kp = 5.0;
    
		PID[Select].Pout = PID[Select].Kp * (PID[Select].Now_Error - PID[Select].Last_Error);

    PID[Select].Iout += PID[Select].Ki * PID[Select].Now_Error;

    PID[Select].Dout = PID[Select].Kd * (PID[Select].Now_Error - 2 * PID[Select].Now_Error + PID[Select].Last_Last_Error);

    PID[Select].Error_buf[0] = PID[Select].Now_Error;
    PID[Select].Last_Error = PID[Select].Error_buf[1];
    PID[Select].Last_Last_Error = PID[Select].Error_buf[2];

    PID_limit(&PID[Select].Iout, 1000);
    PID[Select].Current_Out = PID[Select].Pout + PID[Select].Iout + PID[Select].Dout;

    PID_limit(&PID[Select].Current_Out, 30000);      //18000

    return PID[Select].Current_Out;

}

/*********************************Lunch*********************************/

/*********************************Gimbal*********************************/

 GimbalPidInformation HardPitch;
 GimbalPidInformation Yaw;

//10 1 0.1 0 0 15
void GIMBALPitch_PID_Parameter(GimbalPidInformation *PID)
{
 PID->Kp[0] = 10000;
 PID->Ki[0] = 500;//5
 PID->Kd[0] = 8000;
 PID->Kp[1] = 50;
 PID->Ki[1] = 0;
 PID->Kd[1] = 0;//12 0.5
 PID->Error_Buf[0] = 0;
 PID->Error_Buf[1] = 0;
 PID->Error_Buf[2] = 0;	
 PID->Error_Buf_1[0] = 0;
 PID->Error_Buf_1[1] = 0;
 PID->Error_Buf_1[2] = 0;
 PID->Error[0].Now = 0;
 PID->Error[0].Last = 0;
 PID->Error[0].Last_Last = 0;	
 PID->Error[1].Now = 0;
 PID->Error[1].Last = 0;
 PID->Error[1].Last_Last = 0;
 PID->loopK = 1;
 PID->loopF = 2;
 PID->loop_1F = 1;
 PID->loopCnt = PID->loopF;
 PID->loop_1Cnt = PID->loop_1F;
 PID->add = 0.4;
 PID->base = -0.4;
 float filternum = 0.5;
first_order_filter_type_t pitch;
first_order_filter_init(&pitch, 0.1, &filternum);
 
}

void FeatureFilter(float CharacteristicWave)
{
	static float FeatureHighPoint;
	static float FeatureHighPointBuf[2];
	if(CharacteristicWave > FeatureHighPoint){
	   FeatureHighPointBuf[0] = CharacteristicWave;
	   FeatureHighPointBuf[1] = FeatureHighPointBuf[0];
	}
	//if();
}

float ZoneTest;
float ZoneTest1;

float filternum = 1;

first_order_filter_type_t pitch;

float GimbalPitch_PID_Calc(GimbalPidInformation *PID, float ref, float set, float get)
{
 //防止角度超过死区
 //set = float_deadline( , , set);
 int i,j;
// while(PID->loopCnt > 0)
//	 {
 PID->Set.Angle = set;
 PID->Now.Angle = ref;
 PID->Error[0].Now = PID->Set.Angle - PID->Now.Angle;
 //first_order_filter_cali(&pitch, PID->Error[0].Now);
 //PID->Error[0].Now = pitch.out;	
 
 PID->Error[0].Now = float_deadline(PID->Error[0].Now, -0.05, 0.2);
	
 for(i = 1; i <= 0; i--)
     PID->Error_Buf[i+1] = PID->Error_Buf[i];
 
 PID->Out.P = PID->Kp[0] * PID->Error[0].Now;
 //PID->Out.I += PID->Ki[0] * PID->Error[0].Now;
 //PID->Out.I += PID->Ki[0] * VariableSpeedIntegral(PID->add, PID->base, PID->Error[0].Now, PID->Ki[0]) * PID->Error[0].Now;	
 
// if(PID->Out.I >5000)
//	{
//		while(PID->Error[0].Now < 0)
//		{
//			PID->Out.I += PID->Ki[0] * PID->Error[0].Now;
//		}
//  }
// else if(PID->Out.I <-13000)
//	{
//		while(PID->Error[0].Now > 0)
//		{
//			PID->Out.I += PID->Ki[0] * PID->Error[0].Now;
//		}
//	}	
//	else 
		PID->Out.I += PID->Ki[0] * PID->Error[0].Now;
		
 PID->Out.D = PID->Kd[0] * (PID->Error[0].Now - PID->Error[0].Last);
 
 PID->Out.D = float_constrain(PID->Out.D, -15000, 15000);	
	
 PID->Error_Buf[0] = PID->Error[0].Now;
 PID->Error[0].Last = PID->Error_Buf[1];
 PID->Error[0].Last_Last = PID->Error_Buf[2]; 
 
 PID->Out.Loop_1 = PID->loopK *(PID->Out.P + PID->Out.I + PID->Out.D);
//		 PID->loopCnt--;
//		 PID->LoopSit = Trap;
//	 }
// PID->LoopSit = Continue; 
	 
 //PID->Out.Loop_1 = LoopOuter_HighPassFilter(PID->Out.Loop_1, ZoneTest1);	
 
 //PID->Out.Loop_1 = fp32_constrain(PID->Out.Loop_1, 1950, 3100);   //角度限幅
	
// if (PID->LoopSit == Continue)
// {
// PID->Now.Speed = get;
// PID->Set.Speed = PID->Out.Loop_1;	
// PID->Error[1].Now = PID->Set.Speed - PID->Now.Speed;
// for(j = 1; j <= 0; j--)	
// 	  PID->Error_Buf_1[i+1] = PID->Error_Buf_1[i];
// 
// PID->Out.P_1 = PID->Kp[1] * (PID->Error[1].Now - PID->Error[1].Last); 
// // PID->Ki[1] = VariableIntegral(PID->Error[1].Now, 0, PID->Ki[1]);	
// PID->Out.I_1 += PID->Ki[1] * PID->Error[1].Now;
//	//PID->Out.I_1 += VariableSpeedIntegral(PID->add, PID->base, PID->Error[1].Now, PID->Ki[1]) * PID->Error[1].Now;
//	
// PID->Out.D_1 = PID->Kd[1] * (PID->Error[1].Now - 2*PID->Error[1].Last + PID->Error[1].Last_Last);
// PID->Out.D_1 = float_constrain(PID->Out.D_1, -5000, 5000);	
// //PID->Out.D_1 = LoopInner_HighPassFilter((PID->Error[1].Now - 2*PID->Error[1].Last + PID->Error[1].Last_Last), PID->Out.D_1, ZoneTest);
// //PID->Out.D_1 = AbsAttenuation(PID->Out.D_1, 0.001);
// 
// PID->Error_Buf_1[0] = PID->Error[1].Now;
// PID->Error[1].Last = PID->Error_Buf_1[1];
// PID->Error[1].Last_Last = PID->Error_Buf_1[2]; 
 
 PID->Out.final = PID->Out.Loop_1;//PID->Out.P_1 + PID->Out.I_1 + PID->Out.D_1;
	
// PID->loopCnt = PID->loopF;   //自动重装
// }
 
 PID->Out.final = float_constrain(PID->Out.final, -20000, 20000);
 
 return PID->Out.final;
 
}

void GIMBALYaw_PID_Parameter(GimbalPidInformation *PID)
{
 PID->Kp[0] = 200;
 PID->Ki[0] = 0;
 PID->Kd[0] = 150;
 PID->Kp[1] = 50;
 PID->Ki[1] = 0;
 PID->Kd[1] = 100;
 PID->Error_Buf[0] = 0;
 PID->Error_Buf[1] = 0;
 PID->Error_Buf[2] = 0;	
 PID->Error_Buf_1[0] = 0;
 PID->Error_Buf_1[1] = 0;
 PID->Error_Buf_1[2] = 0;
 PID->Error[0].Now = 0;
 PID->Error[0].Last = 0;
 PID->Error[0].Last_Last = 0;	
 PID->Error[1].Now = 0;
 PID->Error[1].Last = 0;
 PID->Error[1].Last_Last = 0;
 PID->loopK = 1;
 PID->loopF = 1;
 PID->loop_1F =1;	
 PID->loopCnt = PID->loopF;
 PID->loop_1Cnt = PID->loop_1F;	
}

float GimbalYaw_PID_Calc(GimbalPidInformation *PID, float ref, float set, float get)
{
 //防止角度超过死区
 //set = float_constrain(set, 1000, 7000 );
	
 int i,j;
 PID->Set.Angle = set;
 PID->Now.Angle = ref;	
 PID->Error[0].Now = PID->Set.Angle - PID->Now.Angle;
	
 //PID->Error[0].Now = float_deadline(PID->Error[0].Now, PID->Deadlinemin[0], PID->Deadlinemax[0]);	
 
 for(i = 1; i <= 0; i--)
     PID->Error_Buf[i+1] = PID->Error_Buf[i];
 
 PID->Out.P = PID->Kp[0] * PID->Error[0].Now;
 	 
 PID->Out.I += PID->Ki[0] * PID->Error[0].Now;
	
 PID->Out.D = PID->Kd[0] * (PID->Error[0].Now - PID->Error[0].Last); 
	
 PID->Error_Buf[0] = PID->Error[0].Now;
 PID->Error[0].Last = PID->Error_Buf[1];
 PID->Error[0].Last_Last = PID->Error_Buf[2]; 
 
 PID->Out.Loop_1 =  PID->loopK *(PID->Out.P + PID->Out.I + PID->Out.D);
 //PID->Out.Loop_1 = LoopOuter_HighPassFilter(PID->Out.Loop_1, ZoneTest1);	
 
 //速度达到的最大值	
 //PID->Out.Loop_1 = float_constrain(PID->Out.Loop_1, 1950, 3100);   //速度限幅
	
 PID->Now.Speed = get;
 PID->Set.Speed = PID->Out.Loop_1;	
 PID->Error[1].Now = PID->Set.Speed - PID->Now.Speed;
 
 //PID->Error[1].Now = float_deadline(PID->Error[1].Now, PID->Deadlinemin[1], PID->Deadlinemax[1]);
 
 for(j = 1; j <= 0; j--)	
 	  PID->Error_Buf_1[i+1] = PID->Error_Buf_1[i];
 
 PID->Out.P_1 = PID->Kp[1] * (PID->Error[1].Now - PID->Error[1].Last);
 
 if(PID->Out.I_1 >20000)
	{
		while(PID->Error[1].Now < 0)
		{
			PID->Out.I_1 += PID->Ki[1] * PID->Error[1].Now;
		}
  }
 else if(PID->Out.I_1 <-20000)
	{
		while(PID->Error[1].Now > 0)
		{
			PID->Out.I_1 += PID->Ki[1] * PID->Error[1].Now;
		}
	}	
	else PID->Out.I_1 += PID->Ki[1] * PID->Error[1].Now;
 //PID->Out.I_1 += IntegralIndex(PID->Error[1].Now, 180, 200, 20)*PID->Ki[1] * PID->Error[1].Now;
 
 PID->Out.D_1 = PID->Kd[1] * (PID->Error[1].Now - 2*PID->Error[1].Last + PID->Error[1].Last_Last);
 //PID->Out.D_1 = AbsAttenuation(PID->Out.D_1, 0.001);
 
 PID->Error_Buf_1[0] = PID->Error[1].Now;
 PID->Error[1].Last = PID->Error_Buf_1[1];
 PID->Error[1].Last_Last = PID->Error_Buf_1[2]; 
 
 PID->Out.final = PID->Out.P_1 + PID->Out.I_1 + PID->Out.D_1;
 
 PID->Out.final = float_constrain(PID->Out.final, -20000, 20000);
 
 return PID->Out.final;

//IMU模式	
if(0)	
{
 
}	 
}


/*********************************Gimbal*********************************/


/**********************************IMU YAW**********************************/

GimbalPidInformation IMUYaw;

/**
  * @brief  
  * @param  
  * @note   内环位置环P（PD） 外环速度环PI
  * @retval 
  */
void GIMBALIMUYaw_PID_Parameter(GimbalPidInformation *PID)
{
 PID->Kp[0] = 0;
 PID->Ki[0] = 0;
 PID->Kd[0] = 0;
 PID->Kp[1] = 0;
 PID->Ki[1] = 0;
 PID->Kd[1] = 0;
 PID->Error_Buf[0] = 0;
 PID->Error_Buf[1] = 0;
 PID->Error_Buf[2] = 0;	
 PID->Error_Buf_1[0] = 0;
 PID->Error_Buf_1[1] = 0;
 PID->Error_Buf_1[2] = 0;
 PID->Error[0].Now = 0;
 PID->Error[0].Last = 0;
 PID->Error[0].Last_Last = 0;	
 PID->Error[1].Now = 0;
 PID->Error[1].Last = 0;
 PID->Error[1].Last_Last = 0;
 PID->loopK = 1;
 PID->loopF = 1;
 PID->loop_1F =1;	
 PID->loopCnt = PID->loopF;
 PID->loop_1Cnt = PID->loop_1F;	
}

/**
  * @brief  变速积分
  * @param  PID_EXTEND
  * @note   
  * @retval 指针传递
  */
static void f_Changing_Integral_Rate(PID_EXTEND *PID)
{
    if (PID->_errorIN * PID->_Iout > 0)
    {
        if (abs(PID->_errorIN) <= PID->_ScalarB)
            PID->_I_Multiply =1; //完整积分
        if (abs(PID->_errorIN) <= (PID->_ScalarA + PID->_ScalarB))
            //使用线性函数过渡
            PID->_I_Multiply = (PID->_ScalarA - abs(PID->_errorIN) + PID->_ScalarB) / PID->_ScalarA;
        else
            PID->_I_Multiply = 0;//取消积分环节
    }
}

float GimbalIMUYaw_PID_Calc(GimbalPidInformation *PID, float IMUA, float set, float IMUS)
{
 //防止角度超过死区
 //set = float_constrain(set, 1000, 7000 );
	
 int i,j;
 PID->Set.Angle = set;
 PID->Now.Angle = IMUA;	
 PID->Error[0].Now = PID->Set.Angle - PID->Now.Angle;
	
 //PID->Error[0].Now = float_deadline(PID->Error[0].Now, PID->Deadlinemin[0], PID->Deadlinemax[0]);	
 
 for(i = 1; i <= 0; i--)
     PID->Error_Buf[i+1] = PID->Error_Buf[i];
 
 PID->Out.P = PID->Kp[0] * PID->Error[0].Now;
 	 
 PID->Out.I += PID->Ki[0] * PID->Error[0].Now;
	
 PID->Out.D = PID->Kd[0] * (PID->Error[0].Now - PID->Error[0].Last); 
	
 PID->Error_Buf[0] = PID->Error[0].Now;
 PID->Error[0].Last = PID->Error_Buf[1];
 PID->Error[0].Last_Last = PID->Error_Buf[2]; 
 
 PID->Out.Loop_1 =  PID->loopK *(PID->Out.P + PID->Out.I + PID->Out.D);
 //PID->Out.Loop_1 = LoopOuter_HighPassFilter(PID->Out.Loop_1, ZoneTest1);	
 
 //速度达到的最大值	
 //PID->Out.Loop_1 = float_constrain(PID->Out.Loop_1, 1950, 3100);   //速度限幅
	
 PID->Now.Speed = IMUS;
 PID->Set.Speed = PID->Out.Loop_1;	
 PID->Error[1].Now = PID->Set.Speed - PID->Now.Speed;
 
 //PID->Error[1].Now = float_deadline(PID->Error[1].Now, PID->Deadlinemin[1], PID->Deadlinemax[1]);
 
 for(j = 1; j <= 0; j--)	
 	  PID->Error_Buf_1[i+1] = PID->Error_Buf_1[i];
 
 PID->Out.P_1 = PID->Kp[1] * (PID->Error[1].Now - PID->Error[1].Last);
 
 if(PID->Out.I_1 >20000)
	{
		while(PID->Error[1].Now < 0)
		{
			PID->Out.I_1 += PID->Ki[1] * PID->Error[1].Now;
		}
  }
 else if(PID->Out.I_1 <-20000)
	{
		while(PID->Error[1].Now > 0)
		{
			PID->Out.I_1 += PID->Ki[1] * PID->Error[1].Now;
		}
	}	
	else PID->Out.I_1 += PID->Ki[1] * PID->Error[1].Now;
 //PID->Out.I_1 += IntegralIndex(PID->Error[1].Now, 180, 200, 20)*PID->Ki[1] * PID->Error[1].Now;
 
 PID->Out.D_1 = PID->Kd[1] * (PID->Error[1].Now - 2*PID->Error[1].Last + PID->Error[1].Last_Last);
 //PID->Out.D_1 = AbsAttenuation(PID->Out.D_1, 0.001);
 
 PID->Error_Buf_1[0] = PID->Error[1].Now;
 PID->Error[1].Last = PID->Error_Buf_1[1];
 PID->Error[1].Last_Last = PID->Error_Buf_1[2]; 
 
 PID->Out.final = PID->Out.P_1 + PID->Out.I_1 + PID->Out.D_1;
 
 PID->Out.final = float_constrain(PID->Out.final, -20000, 20000);
 
 return PID->Out.final;	
}

/**********************************IMU YAW**********************************/


void PID_Init(void)
{
 CHASSIS_PID_Parameter(Chassis_Pid_Motor);
 Upstir_PID_Parameter(&Upstir);
 Downstir_PID_Parameter(&DownstirPID);
 Fric_PID_Parameter(Fric);
 GIMBALPitch_PID_Parameter(&HardPitch);	
 GIMBALYaw_PID_Parameter(&Yaw);
 GIMBALIMUYaw_PID_Parameter(&IMUYaw);	
}

