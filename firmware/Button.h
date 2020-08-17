//微信公众号：物联网零妖
//按键扫描
#ifndef __BUTTON_H__
#define __BUTTON_H__

#include<STC15W.h>

void Button_Loop(void);

void Button_SetFun(unsigned char CH,unsigned char Type,void (*Fun)(void));//Type D为点动，C为长按

void Button_Init(void);


unsigned char Button_GetState(unsigned char CH);

#endif