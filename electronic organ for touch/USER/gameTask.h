#ifndef __GAMETASK_H
#define __GAMETASK_H	 
#include "sys.h"	    


typedef struct gamePackAction_TypeDef {  
	u8 floorID;
	u8 color;
	u8 type;	 
	u16 time;	 
	struct  gamePackAction_TypeDef *next;
} gamePackAction_TypeDef;

typedef struct gamePackData_TypeDef { 
	u8 Mode;
	u8 Beat; //���� 
	u8 Group; //����
	
	u16 Timer;			//ʹ��ʱ��
	u16 FinGroup;  // �������
	u16 RightCount;  
	u16 ErrCount;  
	u8  NowStep;  //��ǰ���е��ڼ�����
	u8  IsUsed;
	
	u8 nowKey;
	u8 nowFloorID;
	u8 beforeFloorID;
	u8 beforeColor;
	u16  noTouchTime;
	struct  gamePackAction_TypeDef *head;
	//struct  gamePackData_TypeDef *next;
}gamePackData_TypeDef;

 
  

extern gamePackData_TypeDef gamePack1 ; // ��Ϸ�� //��Ϸ������
 
void    gamePack_Clear(gamePackData_TypeDef *head);
gamePackAction_TypeDef    *gamePackAction_Creatlink(void);
int    gamePackAction_InsertNode(gamePackAction_TypeDef *head, gamePackAction_TypeDef *padd);

//void  ApiPacket( u8 * receverData , u8 dataLen);

#endif


