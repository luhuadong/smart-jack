//测试串口1，与USB转串口直接连接   115200/22.1184MHZ
//微信公众号：物联网零妖
#include<Uart_1.h>
#include<Timer0.h>
#include "circle_queue.h"

static bit busy1=0;//COM1用到的发送忙标志

static CircleQueue_t WIFI_Read_Buf;//WIFI接收缓冲区
static unsigned char ReadingByteCount=0;
static void (*MessageFun)(void); //断帧后处理函数


//串口1中断处理程序
void INT_Uart1(void) interrupt 4
{
	if(RI)//清空读中断标志位
	{
		RI = 0;
		ReadingByteCount++;
		Queue_Put(&WIFI_Read_Buf,SBUF);//数据进入队列
		if(SBUF == 0XFA)//收到 0XFA ，单片机复位到烧写区
			IAP_CONTR = 0x60;
	}
	if(TI)//清空发送中断标志位
	{
		TI = 0;
		busy1 = 0;
	}
}
// 描述: 串口初始化程序.
void Init_Uart1(void)
{
	SCON = 0x50;//
	AUXR |= 0x01;//定时器2，1T
	AUXR |= 0x04;
	T2L = 0xD0;		//设定定时初值
	T2H = 0xFF;		//设定定时初值  115200    22.1184MHZ
	AUXR |= 0x10;		//启动定时器2
	ES = 1;//允许COM1串行口中断
	TI = 1;	
	
	Queue_Init(&WIFI_Read_Buf);
	MessageFun = 0;
}
//COM1发送一个字节
void Send_Data1(unsigned char dat)
{
	busy1 = 1;
	SBUF = dat;
	while(busy1);
	busy1 = 1;
}
//发送一个字符串
void Send_Str1(unsigned char *s)
{
	while(*s)
	{
		Send_Data1(*s++);
	}
}
//2ms循环调用，断帧用
void Uart1_CheckMessageLoop(void)
{
	static unsigned char ReadingByteCount2=0;
	static unsigned char NoByteCount=0;
	
	if(ReadingByteCount > ReadingByteCount2)
	{
		NoByteCount=0;
		ReadingByteCount2 = ReadingByteCount;
	}
	else
	{
		NoByteCount ++;
	}
	
	if(NoByteCount > 5) //大概10ms没有收到数据，就执行帧处理函数
	{
		NoByteCount = 0;
		ReadingByteCount = 0;
		ReadingByteCount2 = 0;
		if(MessageFun != 0)
			MessageFun();
	}
}
//设置收到一帧数据后执行的代码
void Uart1_SetMessageFun(void (*Fun)(void))
{
	MessageFun = Fun;
}
//从接收缓冲区取出一个字节消息
unsigned char Get_Byte_Uart1(unsigned char *dat)
{
	return(Queue_Get(&WIFI_Read_Buf,dat));
}
//清空接收缓冲区的队列
void Clear_Uart1(void)
{
	Queue_Init(&WIFI_Read_Buf);
}
//
unsigned int Get_Byte_Count_Uart1(void)
{
	return (Queue_Length(&WIFI_Read_Buf));
}
//









