#ifndef __UARTDEAL_H
#define __UARTDEAL_H
#include "main.h"
 
 
#define RECEIVE_SIZE 	50			//���ջ���


#define CMD_BEGIN     1 		//��ʼ
#define CMD_OVER      2			//����
#define CMD_PACK 			3			//���Ϳγ̰�
#define CMD_SCHEDULE  4			//��ѯ�γ̽���
#define CMD_STATE 		5			//��ѯ�豸״̬
#define CMD_UPDATA 		6			//��״̬�ϴ�
#define CMD_CONTROL   7			//ֱ�ӿ�����
#define CMD_CLOSEALL   8			 
#define CMD_SETSENSOR   9			 

typedef struct
{
	  unsigned int  writeIndex;		//ͷ
		unsigned int  readIndex;		//β
		unsigned char Buf[RECEIVE_SIZE];
}ReceiverBuf_t;

extern ReceiverBuf_t ReceiverBuf;

void ReceiverBuf_Write(unsigned char data );
void UartBufDeal(void);
void UartReturn_Updata(u8 flood);

#endif

