//微信公众号：物联网零妖
//继电器控制
#ifndef __RELAY_H__
#define __RELAY_H__

#include<STC15W.h>

void OpenRelay(unsigned char CH);
void CloseRelay(unsigned char CH);
unsigned char GetRelay(unsigned char CH);

void Relay_Turn(void);


#endif