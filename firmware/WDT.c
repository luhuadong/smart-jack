//微信公众号：物联网零妖
//看门狗代码
#include<WDT.h>

//初始化WDT  分频64 22.1184MHZ 大约1S
void WDT_Init(void)
{
	WDT_CONTR = 0x07;       //看门狗定时器溢出时间计算公式: (12 * 32768 * PS) / FOSC (秒)
	WDT_CONTR |= 0x20;      //启动看门狗
}
//喂狗
void WDT_Clear(void)
{
	WDT_CONTR |= 0x10;
}
//

