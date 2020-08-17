//微信公众号：物联网零妖
//Timer0 22.1184MHZ
#ifndef __TIMER0_H__
#define __TIMER0_H__
#include "STC15W.h"

//初始化定时器0
void Timer0_Init(void);
//系统定时器中断里面执行的函数
void Timer0_SYS_APP_LOOP(void);
void Timer0_SYS_APP_LOOP_Message(void);
void Timer0_SYS_APP_LOOP_Once(void);

//添加一个函数到定时器中
unsigned char Timer_0_Add_Fun(unsigned long Time,void (*Fun)(void));
unsigned char Timer_0_Add_Fun_Once(unsigned long Time,void (*Fun)(void));
//添加一个消息和函数的映射关系
unsigned char Timer0_Add_MessageFun(unsigned char Message,void (*Fun)(void));
//给系统发射一个信号
unsigned char Timer0_SendMessage(unsigned char Message);

#endif