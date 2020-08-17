//队列的实现，用于各种通信场合处理数据。比如SPI,IIC,UART等
//微信公众号：物联网零妖
#include "circle_queue.h"

//初始化队列。
unsigned char Queue_Init(CircleQueue_t *pQue)  
{
	uint16_t i=0;
	for(i=0;i<QUEUESIZE;i++)
		pQue->data1[i] = 0;
	pQue->front = 0; //队头索引是0
	pQue->rear = 0;  //队尾索引是0
	pQue->count = 0;  //队列计数是0
	return 1;  
}  

//判断队列是否是空的。
unsigned char Queue_IfEmpty(CircleQueue_t *pQue)  
{  
	
	if(pQue->count == 0)  
		return 1;  
	else  
		return 0;  
}  
//判断队列是否是满的  
unsigned char Queue_IfFull(CircleQueue_t *pQue)  
{  
	
	if(pQue->count == QUEUESIZE)  
		return 1;  
	else  
		return 0;  
}  
//入队一个字节。放到接收中断里，接收一个新的字节立刻入队。
unsigned char Queue_Put(CircleQueue_t *pQue, QueueElem_t e)  
{  
	
	if(pQue->count == QUEUESIZE)   //如果队列是满的，直接返回错误
	{   
		return 0;  
	}  

	pQue->data1[pQue->rear] = e;  //把新数据放入队尾
	pQue->rear = (pQue->rear + 1) % QUEUESIZE;  //循环队列，防止溢出
	pQue->count++;  //每加入一个数据，队列计数加1
	return 1;  
}  
//从队列获取一个字节。传参以地址的形式 
unsigned char Queue_Get(CircleQueue_t *pQue, QueueElem_t *e)  
{  
	
	if(pQue->count == 0)  //如果队列是空的，则直接出错
	{  
		return 0;  
	}  

	*e = pQue->data1[pQue->front];  //取出队列头的一个元素
	pQue->front = (pQue->front + 1) % QUEUESIZE;  //循环队列，防止溢出
	pQue->count--;  //队列计数减1
	return 1;  
}  

//获取队列头的位置
unsigned char Queue_QueryHead(CircleQueue_t *pQue, QueueElem_t *e)  
{  
	
	if(pQue->count == 0)  
	{  
		return 0;  
	}  

	*e = pQue->data1[pQue->front];
	return 1;  
}  
//清空队列所有数据
unsigned char Queue_Clear(CircleQueue_t *pQue )  
{  
	
	pQue->front = pQue->rear = 0;  
	pQue->count = 0;  
	return 1;   
}  
//获取队列当前的长度 
uint16_t Queue_Length(CircleQueue_t *pQue)  
{  
	return pQue->count;  
}


//
