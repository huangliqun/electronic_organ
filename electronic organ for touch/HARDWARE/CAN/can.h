#ifndef __CAN_H
#define __CAN_H	 
#include "sys.h"	    

#define CAN_BASE_ID 100
#define MAIN_SERVER_ID  101
//CAN����RX0�ж�ʹ��
#define CAN_RX0_INT_ENABLE	0		//0,��ʹ��;1,ʹ��.								    
										 							 				    
u8 CAN_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode,u32 can_id);//CAN��ʼ��
 
u8 Can_Send_Msg(u16 TxId,u8* msg,u8 len);				  //��������

u8 Can_Receive_Msg(u8 *buf);							  //��������
#endif

















