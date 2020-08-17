//Timer0    22.1184MHZ
//微信公众号：物联网零妖
#include "Timer0.h"
#include "Uart_1.h"

#define	DebugSendByte	Send_Data1

#define	FOSC	22118400
#define	Timer_0_List_Count	20

struct 
{
	unsigned char Flag[Timer_0_List_Count];
	void (*Fun_Point_List[Timer_0_List_Count])(void);
	unsigned long Counter[Timer_0_List_Count];
	unsigned long Timer[Timer_0_List_Count];
}Timer0_Struct;

struct 
{
	unsigned char Flag[Timer_0_List_Count];
	void (*Fun_Point_List[Timer_0_List_Count])(void);
	unsigned long Counter[Timer_0_List_Count];
	unsigned long Timer[Timer_0_List_Count];
}Timer0_Struct_Once;

struct
{
	unsigned char MessageQueue[Timer_0_List_Count];
	unsigned char MessageList[Timer_0_List_Count];
	void (*MessageFun_Point_List[Timer_0_List_Count])(void);
	
}Timer0_Message_Struct;

unsigned char Timer0_Handler_Flag=0;
unsigned char Timer0_Handler_Flag_Message=0;
unsigned char Timer0_Handler_Flag_Once=0;
//初始化定时器0
void Timer0_Init(void)
{
	unsigned char i=0;
	
	for(i=0;i<Timer_0_List_Count;i++)
	{
		Timer0_Struct.Flag[i] = 0;
		Timer0_Struct.Counter[i] = 0;
		
		Timer0_Struct_Once.Flag[i] = 0;
		Timer0_Struct_Once.Counter[i] = 0;
		
		Timer0_Message_Struct.MessageQueue[i] = 0;
		Timer0_Message_Struct.MessageList[i] = 0;
	}
	
	AUXR |= 0x80;//定时器时钟1T模式
	TMOD &= 0xF0;//设置定时器模式
    TL0 = 0x9A;//设置定时初值
	TH0 = 0xA9;//设置定时初值
	TF0 = 0;//清除TF0标志
    TR0 = 1;//启动定时器
    ET0 = 1;//使能定时器中断
	
}
//添加一个函数到定时器中
unsigned char Timer_0_Add_Fun(unsigned long Time,void (*Fun)(void))
{
	unsigned char i=0;
	for(i=0;i<Timer_0_List_Count;i++)
	{
		if(Timer0_Struct.Flag[i] == 0)
		{
			Timer0_Struct.Flag[i] = 1;
			Timer0_Struct.Counter[i] = 0;
			Timer0_Struct.Fun_Point_List[i] = Fun;
			Timer0_Struct.Timer[i] = Time-1;
			return 1;
		}
	}
	return 0;
}
//添加一个一次性执行函数到定时器中
unsigned char Timer_0_Add_Fun_Once(unsigned long Time,void (*Fun)(void))
{
	unsigned char i=0;
	for(i=0;i<Timer_0_List_Count;i++)
	{
		if(Timer0_Struct_Once.Flag[i] == 0)
		{
			Timer0_Struct_Once.Flag[i] = 1;
			Timer0_Struct_Once.Counter[i] = 0;
			Timer0_Struct_Once.Fun_Point_List[i] = Fun;
			Timer0_Struct_Once.Timer[i] = Time-1;
			return 1;
		}
	}
	return 0;
}


//添加一个消息和函数的映射关系
unsigned char Timer0_Add_MessageFun(unsigned char Message,void (*Fun)(void))
{
	unsigned char i;
	for(i=0;i<Timer_0_List_Count;i++)
	{
		if(!Timer0_Message_Struct.MessageList[i])
		{
			Timer0_Message_Struct.MessageList[i] = Message;
			Timer0_Message_Struct.MessageFun_Point_List[i] = Fun;
			return 1;
		}
	}
	return 0;
}
//给系统发射一个信号
unsigned char Timer0_SendMessage(unsigned char Message)
{
	unsigned char i;
	for(i=0;i<Timer_0_List_Count;i++)
	{
		if(!Timer0_Message_Struct.MessageQueue[i])
		{
			Timer0_Message_Struct.MessageQueue[i] = Message;
			return 1;
		}
	}
	return 0;
}
//系统定时器中断里面执行的函数
void Timer0_SYS_APP_LOOP(void)
{
	unsigned char i=0,j=0;
	if(Timer0_Handler_Flag == 0)
		return;
	Timer0_Handler_Flag = 0;
	for(i=0;i<Timer_0_List_Count;i++) //循环定时的函数
	{
		if(Timer0_Struct.Flag[i])
		{
			Timer0_Struct.Counter[i] ++ ;
			if(Timer0_Struct.Counter[i] > Timer0_Struct.Timer[i])
			{
				Timer0_Struct.Counter[i] = 0;
				Timer0_Struct.Fun_Point_List[i]();
			}
		}
	}
	
	
}
//系统循环执行-邮箱处理
void Timer0_SYS_APP_LOOP_Message(void)
{
	unsigned char i=0,j=0;
	if(Timer0_Handler_Flag_Message == 0)
		return;
	Timer0_Handler_Flag_Message = 0;
	
	for(i=0;i<Timer_0_List_Count;i++) //调用消息队列中的函数
	{
		if(Timer0_Message_Struct.MessageQueue[i])
		{
			for(j=0;j<Timer_0_List_Count;j++)
			{
				if(Timer0_Message_Struct.MessageQueue[i] == Timer0_Message_Struct.MessageList[j])
				{
					//DebugSendByte(Timer0_Message_Struct.MessageQueue[i]);
					Timer0_Message_Struct.MessageFun_Point_List[j]();
					j = Timer_0_List_Count + 10;
				}
			}
			Timer0_Message_Struct.MessageQueue[i] = 0;
		}
	}
}
//系统循环，单次执行函数处理
void Timer0_SYS_APP_LOOP_Once(void)
{
	unsigned char i=0,j=0;
	if(Timer0_Handler_Flag_Once == 0)
		return;
	Timer0_Handler_Flag_Once = 0;
	
	for(i=0;i<Timer_0_List_Count;i++) //一次性定时函数
	{
		if(Timer0_Struct_Once.Flag[i])
		{
			Timer0_Struct_Once.Counter[i] ++ ;
			if(Timer0_Struct_Once.Counter[i] > Timer0_Struct_Once.Timer[i])
			{
				Timer0_Struct_Once.Counter[i] = 0;
				Timer0_Struct_Once.Flag[i] = 0;
				Timer0_Struct_Once.Fun_Point_List[i]();
				Timer0_Struct_Once.Flag[i] = 0;
			}
		}
	}
	
}
//被链接到了定时器0.
void Timer0_Interrupt(void) interrupt 1
{
	Timer0_Handler_Flag = 1;
	Timer0_Handler_Flag_Message = 1;
	Timer0_Handler_Flag_Once = 1;
}

//

//


