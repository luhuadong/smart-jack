//微信公众号：物联网零妖
//单路18B20驱动
#include<DS18B20_T1.h>
#include<Uart_1.h>
#include<intrins.h>

#define	Debug_SendData	Send_Data1
#define	T_IO	P45		//T1  注意对应IO口设置强推挽，否则距离过长导致没有输出

static unsigned char DS18B20_RAM[10];//第九个元素是DS18B20CRC，第10个元素是单片机计算的CRC
//一微秒延时
static void Delay1us(unsigned int a)		//@22.1184MHz
{
	unsigned char i;
	unsigned int b=0;
	for(b=0;b<a;b++)
	{
		i = 3;
		while (--i);
	}
}
//复位总线
static void Reset_IO(void)
{
	unsigned char a;
	T_IO = 0;
	Delay1us(700);//拉低总线；复位脉冲
	T_IO = 1;
	Delay1us(35);//18B20等待
	a = T_IO; //读取IO口
	Delay1us(160);//18B20拉低总线，表示自己存在
	Delay1us(480);//结束复位
	//Debug_SendData(a);
}
//发送一个字节 低位在前
static void Send_Byte_IO(unsigned char Dat)
{
	unsigned char i=0;
	for(i=0;i<8;i++)
	{
		T_IO = 0;
		Delay1us(5);
		T_IO = Dat & 0x01;
		Delay1us(30);//主机拉低30us
		T_IO = 1;
		Delay1us(5);
		Dat >>= 1;
	}
	Delay1us(10);
}
//读取一个字节 低位在前
static unsigned char Read_Byte_IO(void)
{
	unsigned char i=0;
	unsigned char Dat=0;
	for(i=0;i<8;i++)
	{
		Dat >>= 1;
		T_IO = 0;
		Delay1us(5);//主机拉低5us
		T_IO = 1;
		Delay1us(10);//拉低后15us左右读状态
		if(T_IO)
			Dat |= 0x80;
		else
			Dat |= 0x00;
		Delay1us(5);
	}
	return Dat;
}
//CRC一个字节
static unsigned char CRC_One_Byte(unsigned char Dat)
{
	unsigned char i=0,crc_1byte=0;     
  for(i=0;i<8;i++)
  {
		if(((crc_1byte^Dat)&0x01))
    {
			crc_1byte^=0x18;
      crc_1byte>>=1;
      crc_1byte|=0x80;
    }
    else
			crc_1byte>>=1;
    Dat>>=1;
  }
  return crc_1byte;
}
//计算一串数据的CRC结果
static unsigned char CRC_Bytes(unsigned char *s,unsigned char Len)
{
	unsigned char CRC=0;
	while(Len--)
	{
		CRC = CRC_One_Byte(CRC^*s++);
	}
	return CRC;
}

//循环读取温度数据
void Read_DS18B20_T1_Loop(void)
{
	unsigned char i=0;
	
	Reset_IO();
	Send_Byte_IO(0XCC);//Skip ROM
	Send_Byte_IO(0XBE);//读取内部寄存器

	for(i=0;i<9;i++)//读取9个字节
	{
		DS18B20_RAM[i] = Read_Byte_IO();
	}
	
	Reset_IO();
	Send_Byte_IO(0XCC);//Skip ROM
	Send_Byte_IO(0X44);//打开转换
	
	DS18B20_RAM[9]= CRC_Bytes(DS18B20_RAM,8);//单片机计算CRC
	
}
//返回温度数据
float Read_T1(void)
{
	float FloatTemperature;
	int Temp=0;
	if(DS18B20_RAM[8] == DS18B20_RAM[9])
	{
		Temp = DS18B20_RAM[1];//MSB
		Temp <<= 8;
		Temp += DS18B20_RAM[0];//LSB
		FloatTemperature = Temp;
		FloatTemperature *= 0.0625;
	}
	
	return FloatTemperature;
}




