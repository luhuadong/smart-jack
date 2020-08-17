//微信公众号：物联网零妖
//继电器控制
#include<Relay.h>
#include<Uart_1.h>
#include<intrins.h>
#include<WIFI.h>
#include<Timer0.h>

#define	Relay1_IO	P26		//继电器1

//反转继电器状态
void Relay_Turn(void)
{
	Relay1_IO = ~Relay1_IO;
}

void OpenRelay(unsigned char CH)
{
	
	if(CH==1)
	{
		Relay1_IO = 0;
	}
	
	
}
void CloseRelay(unsigned char CH)
{
	
	if(CH==1)
	{
		Relay1_IO = 1;
	}
	
	
}
unsigned char GetRelay(unsigned char CH)
{
	if(CH==1)
	{
		return ! Relay1_IO;
	}
	
	
	return 0;
}





