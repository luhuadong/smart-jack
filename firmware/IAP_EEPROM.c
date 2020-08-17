//微信公众号：物联网零妖
#include "IAP_EEPROM.h"
#include "Uart_1.h"
//STC15W4K 单片机，可以使用IAP命令擦写内部Flash  起始地址 0X0000 ，512Byte一个扇区
//0000   0200  0400  0600   这是扇区首地址。 STC15W4K56S4只有4个扇区。


#define		Test_ProductKey				"AAA"
#define		Test_DeviceName				"BBB"
#define		Test_DeviceSecret			"CCC"
#define		Test_ProductSecret		"DDD"



#define	LED1	P07
#define	LED2	P06
#define	LED3	P05

static void Close_IAP(void)
{
	IAP_CONTR = 0;
	IAP_CMD = 0;
	IAP_TRIG = 0;
	IAP_ADDRH = 0;
	IAP_ADDRL = 0;
}

static void Erase_IAP(unsigned int addr)
{
		IAP_CONTR = 0x82;//允许IAP操作，设置IAP操作速度         
    IAP_CMD = 0x03;//扇区擦除命令            
    IAP_ADDRL = addr;              
    IAP_ADDRH = addr >> 8;//地址赋值          
    IAP_TRIG = 0x5a;                
    IAP_TRIG = 0xa5;//触发命令               
    _nop_();_nop_();_nop_();            
    Close_IAP();//清空IAP操作寄存器等
}

static void Write_One_Byte(unsigned int addr,unsigned char dat)
{
		IAP_CONTR = 0x82;//允许IAP操作，设置IAP操作速度   
    IAP_CMD = 0x02;//IAP写命令
    IAP_ADDRL = addr;
    IAP_ADDRH = addr >> 8;//地址赋值
    IAP_DATA = dat;//更新写寄存器
    IAP_TRIG = 0x5a;
    IAP_TRIG = 0xa5;//触发
    _nop_();
    Close_IAP();//清空IAP操作寄存器等
}

static unsigned char Read_One_Byte(unsigned int addr)
{
		unsigned char dat=0;//定义一个待返回的数据
    IAP_CONTR = 0x82;//允许IAP
    IAP_CMD = 0x01;//IAP读命令
    IAP_ADDRL = addr;
    IAP_ADDRH = addr >> 8;//地址赋值
    IAP_TRIG = 0x5a;
    IAP_TRIG = 0xa5;//触发
    _nop_();_nop_();_nop_();_nop_();
    dat = IAP_DATA;
		Close_IAP();//清空IAP操作寄存器等
    return dat;
}


//保存三元组到Flash
void EEPROM_SaveSecret(EEPROMSecret *Secret)
{
	unsigned char *p;
	unsigned int i;
	p = Secret;
	Erase_IAP(0x0000);
	for(i=0;i<130;i++)
	{
		Write_One_Byte(0x0000+i,p[i]);
	}
}
//从Flash读取三元组
void EEPROM_ReadSecret(EEPROMSecret *Secret)
{
	unsigned char *p;
	unsigned int i;
	p = Secret;
	for(i=0;i<130;i++)
	{
		p[i] = Read_One_Byte(0x0000+i);
	}
}
//保存配置到Flash
void EEPROM_SaveConfig(EEPROMConfig *Config)
{
	unsigned char *p;
	unsigned int i;
	p = Config;
	Erase_IAP(0x0200);
	for(i=0;i<29;i++)
	{
		Write_One_Byte(0x0200+i,p[i]);
	}
}
//读取Flash配置
void EEPROM_ReadConfig(EEPROMConfig *Config)
{
	unsigned char *p;
	unsigned int i;
	p = Config;
	for(i=0;i<29;i++)
	{
		p[i] = Read_One_Byte(0x0200+i);
	}
}
//进入工厂模式 不改变三元组信息
void EEPROM_EnterFactoryMode(void)
{
	EEPROMConfig Config;
	
	Config.a = 3;
	Config.b = 60;
	Config.c = 25;
	Config.d = 1;
	Config.e = 5;
	Config.f = 10;
	Config.g = 15;
	Config.h = 25;
	Config.i = 35;
	Config.j = 45;
	Config.k = 55;
	Config.l = 65;
	Config.m = 75;
	Config.n = 85;
	Config.o = 95;
	Config.p = 105;
	
	EEPROM_SaveConfig(&Config);
}
//测试模式，烧录三元组信息
void EEPROM_TestWriteSecret(void)
{
	
	char *ProductKey = Test_ProductKey;
	char *ProductSecret = Test_ProductSecret;
	char *DeviceSecret = Test_DeviceSecret;
	char *DeviceName = Test_DeviceName;
	
	
	unsigned char *p;
	EEPROMSecret Secret;
	
	p = &Secret.EEPROM_ProductKey[0];
	while(*ProductKey)
	{
		*p++ = *ProductKey++;
	}
	*p++ = 0;
	
	p = &Secret.EEPROM_ProductSecret[0];
	while(*ProductSecret)
	{
		*p++ = *ProductSecret++;
	}
	*p++ = 0;
	
	p = &Secret.EEPROM_DeviceSecret[0];
	while(*DeviceSecret)
	{
		*p++ = *DeviceSecret++;
	}
	*p++ = 0;
	
	p = &Secret.EEPROM_DeviceName[0];
	while(*DeviceName)
	{
		*p++ = *DeviceName++;
	}
	*p++ = 0;
	
	EEPROM_SaveSecret(&Secret);
	
}
//首次开机使用
void EEPROM_FistStart(void)
{
	unsigned char Flag=0;
	Flag = Read_One_Byte(0x0400);
	if(Flag != 0x88)
	{
		EEPROM_EnterFactoryMode();//配置写入出厂家模式
		EEPROM_TestWriteSecret();//三元组信息写入
		Erase_IAP(0x0400);
		Write_One_Byte(0x0400,0X88);
	}
}
//



