#ifndef __24L01BUF_H
#define __24L01BUF_H
 #include "sys.h"   
 #include "main.h"

#define RF_RECEIVE_SIZE 10			//½ÓÊÕ»º´æ
#define RF_SEND_SIZE    30    //·¢ËÍ»º´æ

typedef struct
{
	  unsigned char head;		//ͷ
		unsigned char tail;		//β
		unsigned char Buf[RF_RECEIVE_SIZE][4];
}RF_ReceiverBuf_t;

typedef struct
{
	  unsigned char head;
		unsigned char tail;
		unsigned char Buf[RF_SEND_SIZE][4];
	  unsigned char txID[RF_SEND_SIZE];
}RF_SendBuf_t;


void   Mode1_ReceiverBuf_Write(unsigned char data[4] );
unsigned char   Mode1_ReceiverBuf_Read(unsigned char *pData);
void   Mode1_ReceiverBufClear(void );

unsigned char   SI24R1_SendBuf_Read(unsigned char *pData , unsigned  char *txid);
void   SI24R1_SendBuf_Write(unsigned char *data ,unsigned char txid);
void   SI24R1_SendBufClear(void);

void   SI24R1_task(void );
void   IRQ_intr_init(void)	;
static void SI24R1_IRQ_Intr_Read(void *arg);	//2.4G??????
 void SI24R1_RecDeal(u8 * buf);
void Ball_OnlineDeal(void);
void IRQ_intr_init(void);  //Íâ²¿ÖÐ¶Ï³õÊ¼»¯
#endif

