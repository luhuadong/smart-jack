//微信公众号：物联网零妖
//串口2
#ifndef __Uart_2_H__
#define __Uart_2_H__
#include<STC15W.h>


void Init_Uart2(void);
void Send_Data2(unsigned char dat);
void Send_Str2(unsigned char *s);
void Uart2_CheckMessageLoop(void);
void Uart2_SetMessageFun(void (*Fun)(void));

unsigned char Get_Byte_WIFI(unsigned char *dat);
void Clear_WIFI(void);
unsigned int Get_Byte_Count_WIFI(void);

#endif