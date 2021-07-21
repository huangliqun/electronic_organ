#ifndef __CANTYPE_H
#define __CANTYPE_H	 
#include "sys.h"	   


 
typedef enum FLOOD_REC_CMD
{
	FLOOD_SETID_BEGIN =0x00,
	FLOOD_SETID_OVER=1,
	FLOOD_LED   =0x02	,		//����led
	FLOOD_BREATH =0x03 ,	// ����  
	FLOOD_ALWAYS =0x04, 		// ����
	FLOOD_LIUSHUI=0x05,
  FLOOD_RADIO1=0x06,    //�㲥���� �� 1-10
	FLOOD_RADIO2=0x07	,		//�㲥���� �� 11-20
  FLOOD_RADIONO1=0x08,    //�㲥���� �� ���ܲ��� ipadֱ�ӿ���ģʽ��ʹ��
	FLOOD_RADIONO2=0x09	,   	//�㲥���� �� ���ܲ���
	FLOOD_SHUTDOWN=0x5a
}FLOOD_REC_CMD;

#endif
