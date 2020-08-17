//微信公众号：物联网零妖
//按键扫描
#include<Button.h>
#include<Uart_1.h>
#include<intrins.h>
#include<Timer0.h>

#define	Debug_SendData	Send_Data1

#define	Button_IO_0	P12		//程序预留，有需要可以硬件上画一个按键，检测两个按键用
#define	Button_IO_1	P16   //按键



#define Button_ListCount		2  //按键的数量
#define	Button_G_Time		15	 //干扰敏感时间,*10ms
#define	Button_D_Time		80 //点动敏感时间，*10ms
#define	Button_L_Time		200 //长按敏感时间,*10ms

static unsigned int Button_Timer[Button_ListCount];
static void (*DianDong_Fun_Point_List[Button_ListCount])(void);
static void (*ChangAn_Fun_Point_List[Button_ListCount])(void);

//读取某IO口状态
static unsigned char Button_ReadIO(unsigned char CH)
{
	if(CH == 0)
		return Button_IO_0;
	if(CH == 1)
		return Button_IO_1;
	return 0;
}
//根据检测到的点动或者长按关联函数  Type D为点动，L为长按
static void Button_RunFun(unsigned char CH,unsigned char Type)
{
	if(Type == 'D')
	{
		if(DianDong_Fun_Point_List[CH] == 0)
			return;
		DianDong_Fun_Point_List[CH]();
	}
	if(Type == 'L')
	{
		if(ChangAn_Fun_Point_List[CH] == 0)
			return;
		ChangAn_Fun_Point_List[CH]();
	}
}
//初始化按钮相关的变量
void Button_Init(void)
{
	unsigned char i=0;
	Button_IO_0 = 1;
	Button_IO_1 = 1;
	for(i=0;i<Button_ListCount;i++)
	{
		DianDong_Fun_Point_List[i] = 0;
		ChangAn_Fun_Point_List[i] = 0;
		Button_Timer[i] = 0;
	}
}

//检测按键业务逻辑，放到10ms循环中不断重复
void Button_Loop(void)
{
	unsigned char i;
	for(i=0;i<Button_ListCount;i++)
	{
		if(Button_ReadIO(i) == 0)
		{
			Button_Timer[i] ++ ;
			if(Button_Timer[i] == 65531)
			{
				Button_Timer[i] =  65530;
				continue;
			}
			if(Button_Timer[i] > Button_L_Time) //5秒之上是长按
			{
				Button_Timer[i] =  65530;
				Button_RunFun(i,'L');
				continue;
			}
		}
		else
		{
			if(Button_Timer[i] != 0)
			{
				if(Button_Timer[i] < Button_G_Time) //抖动
				{
					Button_Timer[i] = 0;
					continue;
				}
				if(Button_Timer[i] < Button_D_Time) //点动
				{
					Button_Timer[i] = 0;
					Button_RunFun(i,'D');
					continue;
				}
				if(Button_Timer[i] >= 65530) //已经处理过长按了
				{
					Button_Timer[i] = 0;
					continue;
				}
				if(Button_Timer[i] > Button_L_Time) //长按
				{
					Button_Timer[i] = 0;
					Button_RunFun(i,'L');
					continue;
				}
			}
			Button_Timer[i] = 0;
		}
	}
}
//给某个IO口添加短按或者长按的处理函数 Type D为点动，C为长按
void Button_SetFun(unsigned char CH,unsigned char Type,void (*Fun)(void))
{
	if(Type == 'D')
	{
		DianDong_Fun_Point_List[CH] = Fun;
	}
	if(Type == 'C')
	{
		ChangAn_Fun_Point_List[CH] = Fun;
	}
}
//获取某个IO口当前状态
unsigned char Button_GetState(unsigned char CH)
{
	if(CH == 0)
	{
		return ! Button_IO_0;
	}
	
	return 0;
}
//



//

