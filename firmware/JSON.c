//微信公众号：物联网零妖
#include<JSON.h>
#include "Uart_1.h"

//判断str1里面是否包含str2，返回位置(注意，长度不超过255) 返回255表示匹配失败
unsigned char Compare_str(unsigned char *str1,unsigned char *str2)
{
	unsigned char *str2_copy;
	unsigned char str1_long=0,str2_long=0;
	str2_copy = str2;//保留str2的字符串地址
	do
	{
		if(str1[str1_long] == str2_copy[0])
		{
			str2_long = str1_long;
			do
			{
				if(str1[str2_long++] != *str2_copy++)//如果有一个字符串不匹配，就跳出这个循环
				{
					str2_long = 0;
					break;
				}
			}while(*str2_copy);
			
			if(str2_long != 0)
				return str1_long;//如果可以执行到这里，表示有匹配的字符串
		}
		str2_copy = str2;
		if(str1_long >= 254)
			return 255;
	}while(str1[str1_long++]);
	
	return 255;//如果上述循环自动退出了，那么匹配失败
}
//返回某个数的几次方。
long int Get_Squ(long int a,unsigned char b)
{
	float num;
	unsigned char i;
	num = 1;
	
	if(b == 0)
		return 1;
	
	for(i=0;i<b;i++)
		num = num*a;
	return num;
}
//将一个字符串转换为整型数据
float Str_2_Float(unsigned char *str)
{
	float num=0,dot_b=0,num_3=0;
	unsigned char count=0,dot_count=0;//一共有几位，小数点在第几位
	unsigned char dat[20];
	unsigned char i;
	unsigned char Fu_Shu_Flag=0;
	
	count=0;
	do
	{
		dat[count] = *str;
		count++;
		if(*str == '.')
			dot_count = count;
	}while(*str++);
	
	if(dat[0] == '-')
	{
		for(i=0;i<count;i++)
			dat[i] = dat[i+1];
		Fu_Shu_Flag=1;
		
		if(count >= 1)
			count--;
		if(dot_count >= 1)
			dot_count--;
	}
	else
	{
		Fu_Shu_Flag = 0;
	}
	
	if(count>0)
		count--;
	if(dot_count>0)
	{
		dot_count--;
		count--;
	}
	
	if(dot_count == 0) //如果没有小数
	{
		for(i=0;i<count;i++)
		{
			num_3 = (dat[i]-0x30);
			num += (num_3*Get_Squ(10,count-i-1));
		}
	}
	else
	{
		for(i=0;i<dot_count;i++)
		{
			num_3 = (dat[i]-0x30);
			num += (num_3*Get_Squ(10,dot_count-i-1));
		}
		if((count-dot_count)>=2)//后面有2位小数
		{
			dot_b = (dat[dot_count+1]-0x30);
			num += 0.1*dot_b;
			dot_b = (dat[dot_count+2]-0x30);
			num += 0.01*dot_b;
		}
		else if((count-dot_count)>=1)//后面有一位小数
		{
			dot_b = (dat[dot_count+1]-0x30);
			num += 0.1*dot_b;
		}
	}
	
	if(Fu_Shu_Flag)
		num *= (-1.00);
	
	return num;
}
//将一个小数转换为字符串，精确到小数点后两位
void Float_To_Str(float num1,unsigned char *s)
{
	float Float_Num;
	long int num,num2;
	long int a;
	unsigned char Data1[25];//用来存放转换好的字符串
	unsigned char i=0,j=0;
	unsigned char Len=0;
	
	Float_Num = num1;//
	if(num1 < 0 )
	{
		Float_Num *= (-1.00);
	}
	num = Float_Num;
	
	if(num == 0)
		Float_Num += 1;
	
	if(Float_Num > num) //有小数
	{
		num = Float_Num * 100;//放大两倍，这样可以保留小数点后两位
		num2 = num;
		for(i=0;i<20;i++)
		{
			a = Get_Squ(10,i);
			if(num/a > 0)//表示数字还没有结束
			{
				Data1[i] = num2 % 10 + 0x30;
				if(num2/10 > 0)
					num2 /= 10;
			}
			else
			{
				Data1[i] = 0;//字符串结尾
				j = i;
				i = 25;
			}
		}
		for(i=0;i<j;i++)
			s[i] = Data1[j-i-1];
		j = s[i-2];
		s[i-2] = '.';
		s[i] = s[i-1];
		s[i-1] = j;
		s[i+1] = 0;
		
		num = Float_Num-1;
		if(num == 0)
			s[0] = 0x30;
	}
	else//没有小数点
	{
		num2 = num;
		for(i=0;i<20;i++)
		{
			a = Get_Squ(10,i);
			if(num/a > 0)//表示数字还没有结束
			{
				Data1[i] = num2 % 10 + 0x30;
				if(num2/10 > 0)
					num2 /= 10;
			}
			else
			{
				Data1[i] = 0;//字符串结尾
				j = i;
				i = 25;
			}
		}
		for(i=0;i<j;i++)
			s[i] = Data1[j-i-1];
		s[i] = 0;
		
		num = Float_Num-1; //处理输入为0的情况
		if((num == 0)&(num1 != 1))
			s[0] = 0x30;
		
	}
	
	Len = 0;
	while(s[Len++]);
	
	if(num1 <0)
	{
		Data1[0] = '-';
		for(i=0;i<Len;i++)
			Data1[1+i] = s[i];
		Len += 1;
		for(i=0;i<Len;i++)
			s[i] = Data1[i];
		s[i] = 0;
	}
	
	if((s[0]=='-')&&(s[1]=='0')&&(s[2]=='.')&&(s[3]=='0')&&(s[4]=='0'))
	{
		s[0]='0';
		s[1]=0;
		s[2]=0;
		s[3]=0;
		s[4]=0;
	}
	if((s[0]=='0')&&(s[1]=='.')&&(s[2]=='0')&&(s[3]=='0'))
	{
		s[0]='0';
		s[1]=0;
		s[2]=0;
		s[3]=0;
		s[4]=0;
	}
}
//获取某个键的值 返回0表示没有匹配到
unsigned char Get_Key_Value(unsigned char *JSON,unsigned char *Key,float *Num_Res)
{
	unsigned char Count=0;//键的位置
	unsigned char Key_Size=0;//键的长度
	unsigned char *str;
	unsigned char Data1[20];//读取的值存放到这里面
	float num=0;//要返回的数值
	
	for(Key_Size=0;Key_Size<20;Key_Size++)
		Data1[Key_Size]=0;
	
	Key_Size=0;
	str = Key;
	while(*str++)
		Key_Size++;
	Count = Compare_str(JSON,Key);
	if(Count == 255)//没有匹配到字符串
	{
		return 0;
	}
	else
	{
		Count += Key_Size;
		Count += 2;
		Key_Size = 0;
		while((JSON[Count] != ',')&(JSON[Count] != '}'))
		{
			Data1[Key_Size++] = JSON[Count++];
		}
		
		num = Str_2_Float(Data1);
		
	}
	*Num_Res = num;
	return 1;
}
//给JSON字符串中添加数字字段，返回整体JSON串的长度
unsigned char JSON_Join_Key(unsigned char *JSON,unsigned char *Key,float Num)
{
	unsigned char JSON_Len=0,i=0,j=0,Counter=0;
	unsigned char Join_Str[64]=",\"";
	unsigned char Num_Str[25]="";
	unsigned char Key_Str[25]="";
	
	for(JSON_Len=0;JSON_Len<254;JSON_Len++)
	{
		if(!JSON[JSON_Len])
			break;
	}
	for(i=0;i<JSON_Len;i++)
	{
		if(JSON[i] == '{')
		{
			Counter++;
		}
		if(JSON[i] == '}')
		{
			Counter--;
		}
		if(JSON[i] == '"')
		{
			j++;
		}
	}
	if(Counter) //通过{}和“”成对出现确认JSON是否完整
		JSON_Len = 0;
	if(j%2)
		JSON_Len = 0;
	
	if(JSON_Len>4)
	{
		for(i=0;i<JSON_Len;i++)
		{
			if(JSON[i] == '}')
			{
				Counter = i;
			}
		}
		JSON_Len = Counter+1;
		JSON[JSON_Len]=0;
	}
	
	
	for(i=0;i<25;i++)
	{
		Key_Str[i]=Key[i];
		if(!Key[i])
			break;
	}
	
	Float_To_Str(Num,Num_Str);
	Sum_Str_Tail(Key_Str,"\":");
	Sum_Str_Tail(Join_Str,Key_Str);
	Sum_Str_Tail(Join_Str,Num_Str);
	
	if(JSON_Len)
	{
		JSON[0]='{';
		JSON[JSON_Len-1]=0;
	}
	else
	{
		JSON[0] = '{';
		JSON[1] = 0;
	}
	
	Sum_Str_Tail(JSON,Join_Str);
	Sum_Str_Tail(JSON,"}");
	
	if(!JSON_Len)
	{
		JSON[1] = ' ';
	}
	
	for(JSON_Len=0;JSON_Len<254;JSON_Len++)
	{
		if(!JSON[JSON_Len])
			break;
	}
	
	return JSON_Len;
	
}
//给JSON添加字符串字段的Key 返回整体JSON串的长度
unsigned char JSON_Join_StrKey(unsigned char *JSON,unsigned char *Key,unsigned char *Str)
{
	unsigned char JSON_Len=0,i=0,j=0,Counter=0;
	unsigned char Join_Str[64]=",\"";
	unsigned char Key_Str[25]="";
	
	for(JSON_Len=0;JSON_Len<254;JSON_Len++)
	{
		if(!JSON[JSON_Len])
			break;
	}
	for(i=0;i<JSON_Len;i++)
	{
		if(JSON[i] == '{')
		{
			Counter++;
		}
		if(JSON[i] == '}')
		{
			Counter--;
		}
		if(JSON[i] == '"')
		{
			j++;
		}
	}
	if(Counter) //通过{}和“”成对出现确认JSON是否完整
		JSON_Len = 0;
	if(j%2)
		JSON_Len = 0;
	
	if(JSON_Len>4)
	{
		for(i=0;i<JSON_Len;i++)
		{
			if(JSON[i] == '}')
			{
				Counter = i;
			}
		}
		JSON_Len = Counter+1;
		JSON[JSON_Len]=0;
	}
	
	
	for(i=0;i<25;i++)
	{
		Key_Str[i]=Key[i];
		if(!Key[i])
			break;
	}
	
	Sum_Str_Tail(Key_Str,"\":\"");
	Sum_Str_Tail(Join_Str,Key_Str);
	Sum_Str_Tail(Join_Str,Str);
	Sum_Str_Tail(Join_Str,"\"");
	
	if(JSON_Len)
	{
		JSON[0]='{';
		JSON[JSON_Len-1]=0;
	}
	else
	{
		JSON[0] = '{';
		JSON[1] = 0;
	}
	
	Sum_Str_Tail(JSON,Join_Str);
	Sum_Str_Tail(JSON,"}");
	
	if(!JSON_Len)
	{
		JSON[1] = ' ';
	}
	
	for(JSON_Len=0;JSON_Len<254;JSON_Len++)
	{
		if(!JSON[JSON_Len])
			break;
	}
	
	return JSON_Len;
	
}
//拼接两条字符串，把2添加到1的后面
void Sum_Str_Tail(char *str1,char *str2)
{
	unsigned int Len1=0;
	unsigned int Len2=0;
	unsigned int i=0;
	
	do
		Len1 ++;
	while(str1[Len1]);//计算str1的长度
	
	do
		Len2 ++;
	while(str2[Len2]);//计算str2的长度
	
	for(i=0;i<Len2;i++)
	{
		str1[Len1+i] = str2[i];
	}
	str1[Len1+i] = 0;
}

//获取某个键后面的字符串
void Get_Key_Str(unsigned char *JSON,unsigned char *Key,unsigned char *s)
{
	
	unsigned char a,i;
	unsigned char Len_Key;
	
	a = Compare_str(JSON,Key);
	if(a == 255)
	{
		s[0] = 0;
		
		return;
	}
	
	Len_Key=0;
	do
	{
		Len_Key++;
		if(Len_Key >= 250)
			return;
	}while(Key[Len_Key]);
	
	for(i=0;i<250;i++)
	{
		s[i] = JSON[a+i+Len_Key+3];
		if(s[i] == '"')
		{
			s[i] = 0;
			return;
		}
	}
	
}

//测试区域
void JsonTest(void)
{
	unsigned char NUMStr[20];
	
	Float_To_Str(-9,NUMStr);
	
	Send_Str1(NUMStr);
	
}


