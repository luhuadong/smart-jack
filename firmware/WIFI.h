//微信公众号：物联网零妖
//EMW3080 ILOP 固件驱动程序
#ifndef __WIFI_H__
#define __WIFI_H__

#include<STC15W.h>

void WIFI_Init(void);
void WIFI_LED_Loop(void);


void WIFI_ResetAuthor(void);//解绑用户关系
void WIFI_StartAWS(void);//启动配网

void WIFI_SubStation(void);


#endif