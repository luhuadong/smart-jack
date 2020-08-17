#ifndef __CIRCLE_QUEUE_H__
#define __CIRCLE_QUEUE_H__
//队列的实现，用于各种通信场合处理数据。比如SPI,IIC,UART等
//博客:www.hongchangzidonghua.com 微信公众号:0零妖1    零妖制作

//定义队列的长度
#define QUEUESIZE   256
  
typedef unsigned char QueueElem_t ;
typedef unsigned short int uint16_t ;
  
typedef struct _CircleQueue  {  
    QueueElem_t data1[QUEUESIZE];
    uint16_t front;
    uint16_t rear;
    uint16_t count;
}CircleQueue_t;   
//
//初始化队列。
unsigned char Queue_Init(CircleQueue_t *pQue);
//判断队列是否是空的。
unsigned char Queue_IfEmpty(CircleQueue_t *pQue);
//判断队列是否是满的  
unsigned char Queue_IfFull(CircleQueue_t *pQue);
//入队一个字节。放到接收中断里，接收一个新的字节立刻入队。
unsigned char Queue_Put(CircleQueue_t *pQue, QueueElem_t e);
//从队列获取一个字节。传参以地址的形式 
unsigned char Queue_Get(CircleQueue_t *pQue, QueueElem_t *e);
//获取队列当前的长度 
uint16_t Queue_Length(CircleQueue_t *pQue) ;
//清空队列所有数据
unsigned char Queue_Clear(CircleQueue_t *pQue )  ;


#endif	//__CIRCLE_QUEUE_H__

