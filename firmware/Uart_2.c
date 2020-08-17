//微信公众号：物联网零妖
//串口2  115200bps 22.1184MHZ
#include<Uart_2.h>
#include "circle_queue.h"

#define S2RI  0x01              //S2CON.0
#define S2TI  0x02              //S2CON.1

static bit busy2=0;//COM2用到的发送忙标志
static CircleQueue_t WIFI_Read_Buf;//WIFI接收缓冲区
static unsigned char ReadingByteCount=0;
static void (*MessageFun)(void); //断帧后处理函数

//串口2中断处理程序
void INT_Uart2(void) interrupt 8
{
 	if (S2CON & S2RI)
  {
    S2CON &= ~S2RI;         //清除S2RI位
		ReadingByteCount++;
		Queue_Put(&WIFI_Read_Buf,S2BUF);//数据进入队列
  }
   if (S2CON & S2TI)
   {
     S2CON &= ~S2TI;         //清除S2TI位
     busy2 = 0;               //清忙标志
   }
}
// 描述: 串口初始化程序.
void Init_Uart2(void)
{
	S2CON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x04;		//定时器2时钟为Fosc,即1T
	T2L = 0xD0;		//设定定时初值
	T2H = 0xFF;		//设定定时初值 115200    22.1184MHZ
	AUXR |= 0x10;	//启动定时器2
	IE2 = 0x01;    //使能串口2中断
	
	Queue_Init(&WIFI_Read_Buf);
	MessageFun = 0;
}
//COM2发送一个字节
void Send_Data2(unsigned char dat)
{
	busy2 = 1;
	S2BUF = dat;
	
	//SBUF = dat;
	
	while(busy2);
	busy2 = 1;
}
//COM2发送一个字符串
void Send_Str2(unsigned char *s)
{
	do
		Send_Data2(*s++);
	while(*s);
}
//2ms循环调用，断帧用
void Uart2_CheckMessageLoop(void)
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
void Uart2_SetMessageFun(void (*Fun)(void))
{
	MessageFun = Fun;
}
//从接收缓冲区取出一个字节消息
unsigned char Get_Byte_WIFI(unsigned char *dat)
{
	return(Queue_Get(&WIFI_Read_Buf,dat));
}
//清空接收缓冲区的队列
void Clear_WIFI(void)
{
	Queue_Init(&WIFI_Read_Buf);
}
//
unsigned int Get_Byte_Count_WIFI(void)
{
	return (Queue_Length(&WIFI_Read_Buf));
}
//



