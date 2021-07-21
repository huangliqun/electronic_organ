#ifndef __UARTDEAL_H
#define __UARTDEAL_H
#include "main.h"
 
 
#define RECEIVE_SIZE 	50			//接收缓存


#define CMD_BEGIN     1 		//开始
#define CMD_OVER      2			//结束
#define CMD_PACK 			3			//发送课程包
#define CMD_SCHEDULE  4			//查询课程进度
#define CMD_STATE 		5			//查询设备状态
#define CMD_UPDATA 		6			//球状态上传
#define CMD_CONTROL   7			//直接控制球
#define CMD_CLOSEALL   8			 
#define CMD_SETSENSOR   9			 

typedef struct
{
	  unsigned int  writeIndex;		//头
		unsigned int  readIndex;		//尾
		unsigned char Buf[RECEIVE_SIZE];
}ReceiverBuf_t;

extern ReceiverBuf_t ReceiverBuf;

void ReceiverBuf_Write(unsigned char data );
void UartBufDeal(void);
void UartReturn_Updata(u8 flood);

#endif

