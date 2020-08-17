#ifndef __JSON_H__
#define __JSON_H__

unsigned char Compare_str(unsigned char *str1,unsigned char *str2);
long int Get_Squ(long int a,unsigned char b);
float Str_2_Float(unsigned char *str);
void Float_To_Str(float num1,unsigned char *s);
unsigned char Get_Key_Value(unsigned char *JSON,unsigned char *Key,float *Num_Res);
unsigned char JSON_Join_Key(unsigned char *JSON,unsigned char *Key,float Num);
void Sum_Str_Tail(char *str1,char *str2);
unsigned char JSON_Join_StrKey(unsigned char *JSON,unsigned char *Key,unsigned char *Str);
void Get_Key_Str(unsigned char *JSON,unsigned char *Key,unsigned char *s);
void JsonTest(void);

#endif 