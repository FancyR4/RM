#include "Judge_Data.h"
#include "Judge_CRC.h"
#include "usart_judge.h"

#define    FALSE    0
#define    TRUE     1

DJI_Judge_t DJI_Judge; 

uint8_t Judge_Send_Dataa[200];

uint8_t Judge_Student_Communicate_Dataa[200];
	int Judge_Send_Data_Long,Judge_Send_Data_Long_1,i;



/*****************************
****裁判系统客户端绘制直线****
输入：
		Graphic_width		线条宽度
		Graphic_start_x	起点x坐标
		Graphic_start_y	起点y坐标
		Graphic_end_x		终点x坐标
		Graphic_end_y		终点Y坐标
******************************/	
void Judge_Straight_Line_Write(graphic_data_struct_t* Line_data,int Graphic_start_x,int Graphic_start_y,int Graphic_end_x,int Graphic_end_y)
{
	Line_data->start_x = (uint32_t)Graphic_start_x;
	Line_data->start_y = (uint32_t)Graphic_start_y;
	Line_data->end_x = (uint32_t)Graphic_end_x;
	Line_data->end_y = (uint32_t)Graphic_end_y;	
}

/*****************************
****裁判系统客户端绘制矩形****
输入：
		Graphic_width		线条宽度
		Graphic_start_x	起点x坐标
		Graphic_start_y	起点y坐标
		Graphic_end_x		对角顶点x坐标
		Graphic_end_y		对角顶点Y坐标
******************************/	
void Judge_Rectangle_Write(graphic_data_struct_t* Rectangle_data,int Graphic_start_x,int Graphic_start_y,int Graphic_end_x,int Graphic_end_y)
{
	Rectangle_data->start_x = (uint32_t)Graphic_start_x;
	Rectangle_data->start_y = (uint32_t)Graphic_start_y;
	Rectangle_data->end_x = (uint32_t)Graphic_end_x;
	Rectangle_data->end_y = (uint32_t)Graphic_end_y;	
}

/*****************************
****裁判系统客户端绘制正圆****
输入：
		Graphic_width		线条宽度
		Graphic_start_x	圆心x坐标
		Graphic_start_y	圆心y坐标
		Graphic_radius	半径
******************************/	
void Judge_All_Circle_Write(graphic_data_struct_t* All_Circle_data,int Graphic_start_x,int Graphic_start_y,int Graphic_radius)
{
	All_Circle_data->start_x = (uint32_t)Graphic_start_x;
	All_Circle_data->start_y = (uint32_t)Graphic_start_y;
	All_Circle_data->radius = (uint32_t)Graphic_radius;
}

/******************************
****裁判系统客户端绘制椭圆****
输入：
		Graphic_width		线条宽度
		Graphic_start_x	圆心x坐标
		Graphic_start_y	圆心y坐标
		Graphic_end_x		x半轴长度
		Graphic_end_y		y半轴长度
******************************/	
void Judge_Oval_Circle_Write(graphic_data_struct_t* Oval_Circl_data,int Graphic_start_x,int Graphic_start_y,int Graphic_end_x,int Graphic_end_y)
{
	Oval_Circl_data->start_x = (uint32_t)Graphic_start_x;
	Oval_Circl_data->start_y = (uint32_t)Graphic_start_y;
	Oval_Circl_data->end_x = (uint32_t)Graphic_end_x;
	Oval_Circl_data->end_y = (uint32_t)Graphic_end_y;	
}

/******************************
****裁判系统客户端绘制圆弧****
输入：
		Graphic_start_angle	起始角度
		Graphic_end_angle		终止角度
		Graphic_width		线条宽度
		Graphic_start_x	圆心x坐标
		Graphic_start_y	圆心y坐标
		Graphic_end_x		x半轴长度
		Graphic_end_y		y半轴长度
******************************/	
void Judge_Arc_Circle_Write(graphic_data_struct_t* Arc_Circle_data,int Graphic_start_angle,int Graphic_end_angle,int Graphic_start_x,int Graphic_start_y,int Graphic_end_x,int Graphic_end_y)
{
	Arc_Circle_data->start_angle = (uint32_t)Graphic_start_angle;
	Arc_Circle_data->end_angle = (uint32_t)Graphic_end_angle;
	Arc_Circle_data->start_x = (uint32_t)Graphic_start_x;
	Arc_Circle_data->start_y = (uint32_t)Graphic_start_y;
	Arc_Circle_data->end_x = (uint32_t)Graphic_end_x;
	Arc_Circle_data->end_y = (uint32_t)Graphic_end_y;	
}

/******************************
****裁判系统客户端绘制浮点数****
输入：
		Word_width	字体大小
		Float_Small_Num	小数位有效个数
		Graphic_width		线条宽度
		Graphic_start_x	起点x坐标
		Graphic_start_y	起点y坐标
		Floating_Num		32位浮点数，float
******************************/	
void Judge_Floating_Write(union Judge_Graphic_Client_Single_t* Judge_Graphic_Client_Single_Floating,int Word_width,int Float_Small_Num,int Graphic_start_x,int Graphic_start_y,float Floatint_Data)
{
	Judge_Graphic_Client_Single_Floating->grapic_data_struct.start_angle = (uint32_t)Word_width;
	Judge_Graphic_Client_Single_Floating->grapic_data_struct.end_angle = (uint32_t)Float_Small_Num;	
	Judge_Graphic_Client_Single_Floating->grapic_data_struct.start_x = (uint32_t)Graphic_start_x;
	Judge_Graphic_Client_Single_Floating->grapic_data_struct.start_y = (uint32_t)Graphic_start_y;
	memcpy(Judge_Graphic_Client_Single_Floating->Judge_client_custom_graphic_single_Data+11,(uint8_t *)&Floatint_Data,4); 	
}

/******************************
****裁判系统客户端绘制整型数****
输入：
		Word_width	字体大小
		Float_Small_Num	小数位有效个数
		Graphic_width		线条宽度
		Graphic_start_x	起点x坐标
		Graphic_start_y	起点y坐标
		Floating_Num		32位整型数，int32_t
******************************/	
void Judge_Integer_Write(union Judge_Graphic_Client_Single_t* Judge_Graphic_Client_Single_Integer,int Word_width,int Graphic_start_x,int Graphic_start_y,int Int_Num)
{
	Judge_Graphic_Client_Single_Integer->grapic_data_struct.start_angle = (uint32_t)Word_width;
	Judge_Graphic_Client_Single_Integer->grapic_data_struct.start_x = (uint32_t)Graphic_start_x;
	Judge_Graphic_Client_Single_Integer->grapic_data_struct.start_y = (uint32_t)Graphic_start_y;
	memcpy(Judge_Graphic_Client_Single_Integer->Judge_client_custom_graphic_single_Data+11,(uint8_t *)&Int_Num,4); 
}

/******************************
****裁判系统客户端绘制字符****
输入：
		Word_width	字体大小
		Word_Lenth	字符长度
		Graphic_width		线条宽度
		Graphic_start_x	起点x坐标
		Graphic_start_y	起点y坐标
******************************/	
void Judge_Character_Write(graphic_data_struct_t* Character_data,int Word_width,int Word_Lenth,int Graphic_start_x,int Graphic_start_y)
{
	Character_data->start_angle = (uint32_t)Word_width;
	Character_data->end_angle = (uint32_t)Word_Lenth;		
	Character_data->start_x = (uint32_t)Graphic_start_x;
	Character_data->start_y = (uint32_t)Graphic_start_y;
}


//void Judge_Graphic_Data_Calculate(uint32_t dataaa,graphic_data_struct_t* graphic_data_size_test)
//{
//	dataaa = (graphic_data_size_test->graphic_tpye <<29) || 
//}
uint32_t dataaa[3];
/***************************************
****裁判系统客户端自定义图形数据设置****
输入：(所有参数)
		Graphic_Name	图形名
		Judge_Graphic_Type	图形操作
		Judge_Graphic_Control	图形类型
		Grap_Board_Num	图层数
		Grap_Colour	颜色
		Grap_start_angle	起始角度
		Grap_end_angle		终止角度
		Grap_width		线条宽度
		Grap_start_x	起点x坐标
		Grap_start_y	起点y坐标
		Grap_size			字体大小或者半径
		Grap_end_x		终点x坐标
		Grap_end_y		终点y坐标
*****************************************/	
void Judge_Graphic_Data_Set(graphic_data_struct_t* graphic_data,uint8_t* Graphic_Name,Judge_Graphic_Type_t Judge_Graphic_Type,Judge_Graphic_Control_t Judge_Graphic_Control,\
int Grap_Board_Num,int Grap_Colour,int Grap_start_angle,int Grap_end_angle,int Grap_width,int Grap_start_x,int Grap_start_y,int Grap_size,int Grap_end_x,int Grap_end_y)
{

	graphic_data->graphic_name[0] = *Graphic_Name;
	graphic_data->graphic_name[1] = *Graphic_Name+1;
	graphic_data->graphic_name[2] = *Graphic_Name+2;
	
//	dataaa = (Judge_Graphic_Control << 29)|(Judge_Graphic_Type << 29)|(Grap_Board_Num << 28)|(Grap_Colour << 28)|(Grap_start_angle << 21)|(Grap_end_angle << 21);
	
	graphic_data->operate_tpye = Judge_Graphic_Control;
	graphic_data->graphic_tpye = Judge_Graphic_Type;
	graphic_data->layer = Grap_Board_Num;
	graphic_data->color = Grap_Colour;
	graphic_data->width = Grap_width;
//	memcpy(dataaa,graphic_data+6,12);
	switch(Judge_Graphic_Type)
	{
		case Straight_Line:
		{
			Judge_Straight_Line_Write(graphic_data,Grap_start_x,Grap_start_y,Grap_end_x,Grap_end_y);
			break;
		}
		case Rectangle:
		{
			Judge_Rectangle_Write(graphic_data,Grap_start_x,Grap_start_y,Grap_end_x,Grap_end_y);
			break;
		}
		case All_Circle:
		{
			Judge_All_Circle_Write(graphic_data,Grap_start_x,Grap_start_y,Grap_size);
			break;
		}
		case Oval_Circle:
		{
			Judge_Oval_Circle_Write(graphic_data,Grap_start_x,Grap_start_y,Grap_end_x,Grap_end_y);
			break;
		}
		case Arc_Circle:
		{
			Judge_Arc_Circle_Write(graphic_data,Grap_start_angle,Grap_end_angle,Grap_start_x,Grap_start_y,Grap_end_x,Grap_end_y);
			break;
		}
		case Floating:
//			Judge_Floating_Write(graphic_data,Grap_start_angle,Grap_end_angle,Grap_start_x,Grap_start_y);
		break;
		case Integer:
	//		Judge_Integer_Write(graphic_data,Grap_start_angle,Grap_start_x,Grap_start_y);
		break;
		case Character:
//			Judge_Character_Write(graphic_data,Grap_start_angle,Grap_end_angle,Grap_start_x,Grap_start_y);
		break;
		
	
	}

}

/***************************************
****裁判系统客户端自定义字数据设置****
输入：(所有参数)
		Graphic_Name	图形名
		Judge_Graphic_Type	图形操作
		Judge_Graphic_Control	图形类型
		Grap_Board_Num	图层数
		Grap_Colour	颜色
		Grap_Word_Size	字体大小
		Grap_Word_Lenth		小数位有效个数/字符长度
		Grap_width		线条宽度
		Grap_start_x	起点x坐标
		Grap_start_y	起点y坐标
		Flost_32_Data		浮点型32位数据		
		Int_32_Data			整型32位数据
		Send_Character	字符串地址
*****************************************/	
void Judge_Word_Data_Set(union Judge_Graphic_Client_Single_t* Judge_Graphic_Client_Single,uint8_t* Graphic_Name,Judge_Graphic_Type_t Judge_Graphic_Type,Judge_Graphic_Control_t Judge_Graphic_Control,\
int Grap_Board_Num,int Grap_Colour,int Grap_Word_Size,int Grap_Word_Lenth,int Grap_width,int Grap_start_x,int Grap_start_y,float Flost_32_Data,int32_t Int_32_Data)
{

	Judge_Graphic_Client_Single->grapic_data_struct.graphic_name[0] = *Graphic_Name;
	Judge_Graphic_Client_Single->grapic_data_struct.graphic_name[1] = *Graphic_Name+1;
	Judge_Graphic_Client_Single->grapic_data_struct.graphic_name[2] = *Graphic_Name+2;
		
	Judge_Graphic_Client_Single->grapic_data_struct.operate_tpye = Judge_Graphic_Control;
	Judge_Graphic_Client_Single->grapic_data_struct.graphic_tpye = Judge_Graphic_Type;
	Judge_Graphic_Client_Single->grapic_data_struct.layer = Grap_Board_Num;
	Judge_Graphic_Client_Single->grapic_data_struct.color = Grap_Colour;
	Judge_Graphic_Client_Single->grapic_data_struct.width = Grap_width;
	switch(Judge_Graphic_Type)
	{
		case Floating:
		{
			Judge_Floating_Write(Judge_Graphic_Client_Single,Grap_Word_Size,Grap_Word_Lenth,Grap_start_x,Grap_start_y,Flost_32_Data);
			break;
		}
		case Integer:
		{
			Judge_Integer_Write(Judge_Graphic_Client_Single,Grap_Word_Size,Grap_start_x,Grap_start_y,Int_32_Data);
			break;
		}
		case Character:
		{
//			Judge_Character_Write(graphic_data,Grap_Word_Size,Grap_Word_Lenth,Grap_start_x,Grap_start_y);
			break;
		}
		case Straight_Line:break;
		case Rectangle:break;
		case All_Circle:break;
		case Oval_Circle:break;
		case Arc_Circle:break;	
	
	}
	
}
extern uint8_t	Judge_Test_Graphic_Data[];
/***************************************
****裁判系统客户端自定义字符数据设置****
输入：(所有参数)
		Graphic_Name	图形名
		Judge_Graphic_Type	图形操作
		Judge_Graphic_Control	图形类型
		Grap_Board_Num	图层数
		Grap_Colour	颜色
		Grap_Word_Size	字体大小
		Grap_Word_Lenth		小数位有效个数/字符长度
		Grap_width		线条宽度
		Grap_start_x	起点x坐标
		Grap_start_y	起点y坐标
		Send_Character	字符串地址
*****************************************/	
void Judge_Character_Data_Set(Judge_client_custom_character_t* Judge_client_custom_character,uint8_t* Graphic_Name,Judge_Graphic_Type_t Judge_Graphic_Type,Judge_Graphic_Control_t Judge_Graphic_Control,\
int Grap_Board_Num,int Grap_Colour,int Grap_Word_Size,int Grap_Word_Lenth,int Grap_width,int Grap_start_x,int Grap_start_y,uint8_t* Send_Character)
{

	Judge_client_custom_character->grapic_data_struct.graphic_name[0] = *Graphic_Name;
	Judge_client_custom_character->grapic_data_struct.graphic_name[1] = *Graphic_Name+1;
	Judge_client_custom_character->grapic_data_struct.graphic_name[2] = *Graphic_Name+2;
		
	Judge_client_custom_character->grapic_data_struct.operate_tpye = Judge_Graphic_Control;
	Judge_client_custom_character->grapic_data_struct.graphic_tpye = Judge_Graphic_Type;
	Judge_client_custom_character->grapic_data_struct.layer = Grap_Board_Num;
	Judge_client_custom_character->grapic_data_struct.color = Grap_Colour;
	Judge_client_custom_character->grapic_data_struct.width = Grap_width;
	switch(Judge_Graphic_Type)
	{
		case Floating:break;
		case Integer:break;
		case Character:
			Judge_Character_Write(&Judge_client_custom_character->grapic_data_struct,Grap_Word_Size,Grap_Word_Lenth,Grap_start_x,Grap_start_y);
			memcpy(Judge_client_custom_character->data,Send_Character,30);			
			break;
		case Straight_Line:break;
		case Rectangle:break;
		case All_Circle:break;
		case Oval_Circle:break;
		case Arc_Circle:break;	
	
	}
	
}

void Judge_Communication_To_Client_Custom(DJI_Judge_t*DJI_Judge_Clint_Communication)
{
	
}


/***************************************
****裁判系统客户端自定义字符数据发送****
输入：(所有参数)
		Graphic_Name	图形名
		Judge_Graphic_Type	图形操作
		Judge_Graphic_Control	图形类型
		Grap_Board_Num	图层数
		Grap_Colour	颜色
		Grap_Word_Size	字体大小
		Grap_Word_Lenth		小数位有效个数/字符长度
		Grap_width		线条宽度
		Grap_start_x	起点x坐标
		Grap_start_y	起点y坐标
		Send_Character	字符串地址
*****************************************/	
void Judge_Data_Send(DJI_Judge_t* DJI_Judge_Send_Data,uint16_t Stu_data_cmd_id,uint16_t Stu_Get_id, uint8_t* p_data , int Data_long)
{
	static uint8_t Judge_Send_Seq = 0;

//	Judge_Send_Data_Long_1 = sizeof(&p_data);
	
	DJI_Judge_Send_Data->Judge_Send_Data.frame_header.sof = DJI_Judge_Header_SOF;
	DJI_Judge_Send_Data->Judge_Send_Data.frame_header.data_length = sizeof(DJI_Judge_Send_Data->Judge_Send_Data.Judge_student_interactive_header_data) + Data_long;
	DJI_Judge_Send_Data->Judge_Send_Data.frame_header.seq = Judge_Send_Seq;
	

	
	memcpy(Judge_Send_Dataa, &DJI_Judge_Send_Data->Judge_Send_Data.frame_header, sizeof(DJI_Judge_Send_Data->Judge_Send_Data.frame_header));
	append_crc8_check_sum(Judge_Send_Dataa,sizeof(DJI_Judge_Send_Data->Judge_Send_Data.frame_header));
	
	DJI_Judge_Send_Data->Judge_Send_Data.cmdid = DJI_Judge_Student_CmdID;
	DJI_Judge_Send_Data->Judge_Send_Data.Judge_student_interactive_header_data.data_cmd_id = Stu_data_cmd_id;
	DJI_Judge_Send_Data->Judge_Send_Data.Judge_student_interactive_header_data.sender_ID = \
	(uint16_t)DJI_Judge_Send_Data->DJI_Judge_Mes.Judge_game_robot_status.robot_id;
	DJI_Judge_Send_Data->Judge_Send_Data.Judge_student_interactive_header_data.receiver_ID = Stu_Get_id;
	
	
	
	memcpy(Judge_Send_Dataa+sizeof(DJI_Judge_Send_Data->Judge_Send_Data.frame_header),(uint8_t *)&DJI_Judge_Send_Data->Judge_Send_Data.cmdid,sizeof(DJI_Judge_Send_Data->Judge_Send_Data.cmdid));
	
	memcpy(Judge_Send_Dataa+sizeof(DJI_Judge_Send_Data->Judge_Send_Data.frame_header)+sizeof(DJI_Judge_Send_Data->Judge_Send_Data.cmdid),(uint8_t*)&DJI_Judge_Send_Data->Judge_Send_Data.Judge_student_interactive_header_data,sizeof(DJI_Judge_Send_Data->Judge_Send_Data.Judge_student_interactive_header_data)); 	

/*
	if((Stu_data_cmd_id >= 0x0200) && (Stu_data_cmd_id <= 0x02FF))
	{
		memcpy(Judge_Send_Dataa+sizeof(DJI_Judge_Send_Data->Judge_Send_Data.frame_header)+sizeof(DJI_Judge_Send_Data->Judge_Send_Data.cmdid) + sizeof(DJI_Judge_Send_Data->Judge_Send_Data.Judge_student_interactive_header_data),p_data,Data_long); 		
	}
	else
	{
		switch(Stu_data_cmd_id)
		{
			case 0x0100: //客户端删除图形
			{
//				memcpy(Judge_Send_Dataa+sizeof(DJI_Judge_Send_Data->Judge_Send_Data.frame_header)+sizeof(DJI_Judge_Send_Data->Judge_Send_Data.cmdid) + sizeof(DJI_Judge_Send_Data->Judge_Send_Data.Judge_student_interactive_header_data),\
//					p_data,Data_long); 		
				
				break;
			}
			case 0x0101: //客户端绘制一个图形
			{
			
				break;
			}
			case 0x0102: //客户端绘制二个图形
			{
			
				break;
			}
			case 0x0103: //客户端绘制五个图形
			{
			
				break;
			}
			case 0x0104: //客户端绘制七个图形
			{
			
				break;
			}
			case 0x0110: //客户端绘制字符
			{
			
				break;
			}

		}	
	}
*/
		memcpy(Judge_Send_Dataa+sizeof(DJI_Judge_Send_Data->Judge_Send_Data.frame_header)+sizeof(DJI_Judge_Send_Data->Judge_Send_Data.cmdid) + sizeof(DJI_Judge_Send_Data->Judge_Send_Data.Judge_student_interactive_header_data),p_data,Data_long); 		


//	memcpy(Judge_Send_Dataa + sizeof(DJI_Judge_Send_Data->Judge_Send_Data.frame_header) + sizeof(DJI_Judge_Send_Data->Judge_Send_Data.Judge_student_interactive_header_data), p_data, sizeof(&p_data));
	append_crc16_check_sum(Judge_Send_Dataa,sizeof(DJI_Judge_Send_Data->Judge_Send_Data) + Data_long);
	Judge_Send_Data_Long = sizeof(DJI_Judge_Send_Data->Judge_Send_Data) + Data_long;

		for(i = 0;i < Judge_Send_Data_Long;i++)
		{
			USART6_SendData(Judge_Send_Dataa[i]);
//			while(USART_GetFlagStatus(USART6,USART_FLAG_TC)==RESET);
//			USART_SendData(USART6,(uint8_t)Judge_Send_Dataa[i]);			
		}	
	
	Judge_Send_Seq++;
	if(Judge_Send_Seq == 0xFF)
	{
		Judge_Send_Seq = 0;
	}
}



int Judge_Data_Update(uint8_t* Judge_Data_Get,DJI_Judge_t* DJI_Judge_Update)
{
		int Judge_Data_Len;
		//校验头帧SOF
		if(Judge_Data_Get[0] == DJI_Judge_Header_SOF)
		{
			//将数组数据存入结构体
			DJI_Judge_Update->Judge_Data.frame_header.sof = DJI_Judge_Header_SOF;
			DJI_Judge_Update->Judge_Data.frame_header.data_length = (uint16_t)(Judge_Data_Get[1] | (Judge_Data_Get[2] << 8));
			DJI_Judge_Update->Judge_Data.frame_header.seq = Judge_Data_Get[3];
			//头帧CRC8校验判断
			if(verify_crc8_check_sum( Judge_Data_Get, Judge_Header_LEN ) == TRUE)
			{
				Judge_Data_Len = DJI_Judge_Update->Judge_Data.frame_header.data_length + Judge_Header_LEN + Judge_Cmd_id_LEN + Judge_CRC16_LEN;
				DJI_Judge_Update->Judge_Data.cmdid = (uint16_t)(Judge_Data_Get[5] | (Judge_Data_Get[6] << 8));
				//尾帧CRC16校验判断
				if(verify_crc16_check_sum( Judge_Data_Get, Judge_Data_Len ) == TRUE)
				{			
					switch(DJI_Judge_Update->Judge_Data.cmdid)
					{
						case Judge_CmdID_Game_State:
						{
							memcpy(&DJI_Judge_Update->DJI_Judge_Mes.Judge_game_status, (Judge_Data_Get + Judge_Header_LEN + Judge_Cmd_id_LEN), Judge_Len_Game_State);
							break;
						}
						case Judge_CmdID_Game_Result:
						{
							memcpy(&DJI_Judge_Update->DJI_Judge_Mes.Judge_game_result, (Judge_Data_Get + Judge_Header_LEN + Judge_Cmd_id_LEN), Judge_Len_Game_Result);
							break;
						}
						case Judge_CmdID_Robot_Blood:
						{
							memcpy(&DJI_Judge_Update->DJI_Judge_Mes.Judge_game_robot_HP, (Judge_Data_Get + Judge_Header_LEN + Judge_Cmd_id_LEN), Judge_Len_Robot_Blood);
							break;
						}
						case Judge_CmdID_Missile_State:
						{
							memcpy(&DJI_Judge_Update->DJI_Judge_Mes.Judge_dart_status, (Judge_Data_Get + Judge_Header_LEN + Judge_Cmd_id_LEN), Judge_Len_Missile_State);
							break;
						}
						case Judge_CmdID_Area_Event:
						{
							memcpy(&DJI_Judge_Update->DJI_Judge_Mes.Judge_event_data, (Judge_Data_Get + Judge_Header_LEN + Judge_Cmd_id_LEN), Judge_Len_Area_Event);
							break;
						}
						case Judge_CmdID_Area_Supply_Station_Action_Data:
						{
							memcpy(&DJI_Judge_Update->DJI_Judge_Mes.Judge_supply_projectile_action, (Judge_Data_Get + Judge_Header_LEN + Judge_Cmd_id_LEN), Judge_Len_Area_Supply_Station_Action_Data);
							break;
						}
						case Judge_CmdID_Waening_Data:
						{
							memcpy(&DJI_Judge_Update->DJI_Judge_Mes.Judge_referee_warning, (Judge_Data_Get + Judge_Header_LEN + Judge_Cmd_id_LEN), Judge_Len_Waening_Data);
							break;
						}
						case Judge_CmdID_Missile_Time_Limit_Count:
						{
							memcpy(&DJI_Judge_Update->DJI_Judge_Mes.Judge_dart_remaining_time, (Judge_Data_Get + Judge_Header_LEN + Judge_Cmd_id_LEN), Judge_Len_Missile_Time_Limit_Count);
							break;
						}
						case Judge_CmdID_Robot_State:
						{
							memcpy(&DJI_Judge_Update->DJI_Judge_Mes.Judge_game_robot_status, (Judge_Data_Get + Judge_Header_LEN + Judge_Cmd_id_LEN), Judge_Len_Robot_State);
							break;
						}
						case Judge_CmdID_Real_Time_Power_Heat:
						{
							memcpy(&DJI_Judge_Update->DJI_Judge_Mes.Judge_power_heat_data, (Judge_Data_Get + Judge_Header_LEN + Judge_Cmd_id_LEN), Judge_Len_Real_Time_Power_Heat);
							break;
						}
						case Judge_CmdID_Robot_Position_Data:
						{
							memcpy(&DJI_Judge_Update->DJI_Judge_Mes.Judge_game_robot_pos, (Judge_Data_Get + Judge_Header_LEN + Judge_Cmd_id_LEN), Judge_Len_Robot_Position_Data);
							break;
						}
						case Judge_CmdID_Robot_Gain_Data:
						{
							memcpy(&DJI_Judge_Update->DJI_Judge_Mes.Judge_buff, (Judge_Data_Get + Judge_Header_LEN + Judge_Cmd_id_LEN), Judge_Len_Robot_Gain_Data);
							break;
						}
						case Judge_CmdID_Aerial_Robot_Power_Data:
						{
							memcpy(&DJI_Judge_Update->DJI_Judge_Mes.Judge_aerial_robot_energy, (Judge_Data_Get + Judge_Header_LEN + Judge_Cmd_id_LEN), Judge_Len_Aerial_Robot_Power_Data);
							break;
						}
						case Judge_CmdID_Heat_Data:
						{
							memcpy(&DJI_Judge_Update->DJI_Judge_Mes.Judge_robot_hurt, (Judge_Data_Get + Judge_Header_LEN + Judge_Cmd_id_LEN), Judge_Len_Heat_Data);
							break;
						}
						case Judge_CmdID_Real_Time_Shoot_Data:
						{
							memcpy(&DJI_Judge_Update->DJI_Judge_Mes.Judge_shoot_data, (Judge_Data_Get + Judge_Header_LEN + Judge_Cmd_id_LEN), Judge_Len_Real_Time_Shoot_Data);
							break;
						}
						case Judge_CmdID_Bullet_Shoot_Limit:
						{
							memcpy(&DJI_Judge_Update->DJI_Judge_Mes.Judge_bullet_remaining, (Judge_Data_Get + Judge_Header_LEN + Judge_Cmd_id_LEN), Judge_Len_Bullet_Shoot_Limit);
							break;
						}
						case Judge_CmdID_Robot_RFID_State:
						{
							memcpy(&DJI_Judge_Update->DJI_Judge_Mes.Judge_RFID_status, (Judge_Data_Get + Judge_Header_LEN + Judge_Cmd_id_LEN), Judge_Len_Robot_RFID_State);
							break;
						}
						case Judge_CmdID_Missile_Client_Control_Data:
						{
							memcpy(&DJI_Judge_Update->DJI_Judge_Mes.Judge_dart_client_cmd, (Judge_Data_Get + Judge_Header_LEN + Judge_Cmd_id_LEN), Judge_Len_Missile_Client_Control_Data);
							break;
						}
						case Judge_CmdID_Robot_Communication_Data:
						{
							DJI_Judge_Update->DJI_Judge_Mes.Judge_Student_Data.Student_Data = Judge_Student_Communicate_Dataa;
							memcpy(&DJI_Judge_Update->DJI_Judge_Mes.Judge_Student_Data, (Judge_Data_Get + Judge_Header_LEN + Judge_Cmd_id_LEN), DJI_Judge_Update->Judge_Data.frame_header.data_length);
							break;
						}	
					return Judge_Data_Len;						
					
					}

				}
				return 0;
			}			
			return 0;
		}
		return 0;
}	



void Judge_Data_check(uint8_t* Judge_Data)
{
	static int Judge_Len;
	int i;
		for(i = 0;i<DJI_Judge_Buf_Len_Max;i++)
		{		
			Judge_Len = Judge_Data_Update(Judge_Data+i,&DJI_Judge);	
			if(Judge_Len!= 0)
			{
				i = i+ Judge_Len - 1 ;
			}		
		}
}


DJI_Judge_Mes_t* get_Judge_Mes_Add(void)
{
	return &DJI_Judge.DJI_Judge_Mes;
}
