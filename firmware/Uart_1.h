
//微信公众号：物联网零妖
#ifndef __Uart_1_H__
#define __Uart_1_H__

#include<STC15W.h>

void Init_Uart1(void);// 描述: 串口初始化程序.
void Send_Data1(unsigned char dat);//COM1发送一个字节
void Send_Str1(unsigned char *s);
void Uart1_SetMessageFun(void (*Fun)(void));
void Uart1_CheckMessageLoop(void);
unsigned char Get_Byte_Uart1(unsigned char *dat);
void Clear_Uart1(void);
unsigned int Get_Byte_Count_Uart1(void);

#endif