//微信公众号：物联网零妖
//工作模式和业务逻辑
#include<Mode.h>
#include<IAP_EEPROM.h>
#include<Relay.h>
#include<Timer0.h>
#include<WIFI.h>
#include<Button.h>


#define	LED1	P07
#define	LED2	P06
#define	LED3	P05

void ModeCloseRelay(void)
{
	CloseRelay(1);
}
//打开继电器，带延时。
void Mode_OpenRelay(void)
{
	OpenRelay(1);
}
//三个LED一起闪烁，表示正在配网
void Mode_3LED_Loop(void)
{
	LED1 = ~LED3;
	LED2 = ~LED3;
	LED3 = LED1;
}
//按键点动逻辑
void Mode_DianDong(void)
{
	Relay_Turn();
	WIFI_SubStation();
}



//
