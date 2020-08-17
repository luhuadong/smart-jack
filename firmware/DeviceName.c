//微信公众号：物联网零妖
//串口1 三元组交互逻辑
#include<DeviceName.h>
#include<Uart_1.h>
#include<Timer0.h>
#include<JSON.h>
#include<IAP_EEPROM.h>

#define	Debug_SendByte	Send_Data1
#define	Debug_SendStr	  Send_Str1


/*

{"CMD":"Read"}  //读取三元组

//写入新三元组。因为未知Bug，顺序必须按照下述顺序，TAB不能为空格。
{
	"CMD":"Write",
	"ProductKey":"XXXXX",
	"DeviceSecret":"XXXXX",
	"DeviceName":"XXXXX",
	"ProductSecret":"XXXXX"
}

*/



//帧处理函数
void Uart1_MessageFun(void)
{
	unsigned int MessageCount=0;
	unsigned int i;
	unsigned char Dat;
	unsigned char ReadBuf[180];
	float LinFloat=0;
	
	EEPROMSecret Secret;
	
	EEPROM_ReadSecret(&Secret); //读取三元组信息
	
	MessageCount = Get_Byte_Count_Uart1();
	for(i=0;i<MessageCount;i++)
	{
		Get_Byte_Uart1(&Dat);
		ReadBuf[i] = Dat;
	}
	ReadBuf[i] = 0;
	
	if(Compare_str(ReadBuf,"Read") != 255)
	{
		Send_Str1("{\"ProductKey\":\"");
		Send_Str1(Secret.EEPROM_ProductKey);
		Send_Str1("\",\"ProductSecret\":\"");
		Send_Str1(Secret.EEPROM_ProductSecret);
		Send_Str1("\",\"DeviceSecret\":\"");
		Send_Str1(Secret.EEPROM_DeviceSecret);
		Send_Str1("\",\"DeviceName\":\"");
		Send_Str1(Secret.EEPROM_DeviceName);
		Send_Str1("\"}");
		return;
	}
	
	if(Compare_str(ReadBuf,"Write") != 255)
	{
		Get_Key_Str(ReadBuf,"ProductKey",Secret.EEPROM_ProductKey);
		Get_Key_Str(ReadBuf,"ProductSecret",Secret.EEPROM_ProductSecret);
		Get_Key_Str(ReadBuf,"DeviceSecret",Secret.EEPROM_DeviceSecret);
		Get_Key_Str(ReadBuf,"DeviceName",Secret.EEPROM_DeviceName);
		
		EEPROM_SaveSecret(&Secret);
		
		Send_Str1("{\"CMD\":\"OK\"}");
		
		IAP_CONTR = 0x60;//复位
		
		return;
	}
	
}

//



//



