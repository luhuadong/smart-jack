//微信公众号：物联网零妖
//使用单片机内部的Flash，模拟EEPROM使用
//STC15W4K48S4 单片机，可以使用IAP命令擦写内部Flash
//起始地址 0X0000 结束 0X27FF  其中，512Byte一个扇区
//一共20个扇区  一个扇区可以存储一组参数，这样就可以直接擦出整个扇区修改内容

#ifndef	__IAP_EEPROM__H_
#define	__IAP_EEPROM__H_
#include "STC15W.h"
#include "intrins.h"

//本地配置  29个字节
typedef struct _Config
{
	unsigned char 	a;
	unsigned int 		b;
	unsigned char 	c;
	unsigned char   d;
	unsigned int 		e;
	unsigned int 		f;
	unsigned int 		g;
	unsigned int 		h;
	unsigned int 		i;
	unsigned int 		j;
	unsigned int 		k;
	unsigned int 		l;
	unsigned int 		m;
	unsigned int 		n;
	unsigned int 		o;
	unsigned int 		p;
}EEPROMConfig;

//三元组配置 130个字节
typedef struct _ConfigSecret
{
	unsigned char 	EEPROM_ProductKey[20];
	unsigned char 	EEPROM_ProductSecret[30];
	unsigned char 	EEPROM_DeviceSecret[50];
	unsigned char 	EEPROM_DeviceName[30];
}EEPROMSecret;

//
void EEPROM_SaveSecret(EEPROMSecret *Secret);
void EEPROM_ReadSecret(EEPROMSecret *Secret);
void EEPROM_SaveConfig(EEPROMConfig *Config);
void EEPROM_ReadConfig(EEPROMConfig *Config);
void EEPROM_EnterFactoryMode(void);
void EEPROM_TestWriteSecret(void);
void EEPROM_FistStart(void);

#endif