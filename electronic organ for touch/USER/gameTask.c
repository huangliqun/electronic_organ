#include "main.h"
 
#include "string.h "
#include "stdlib.h"	
 

gamePackData_TypeDef gamePack1,gamePack2,gamePack3; // 游戏包

//控制所有的球亮一个色						
void ctrlAllBall(u8 color)
{
		u8 canbuf[4];
		canbuf[0]=FLOOD_LED;
		canbuf[1]	=color;
		canbuf[2]	=40;		
		SI24R1_SendBuf_Write(canbuf,CAN_BASE_ID);
}	

//控制某个球亮色
void ctrlOneBall(u8 id,u8 color)
{
		u8 canbuf[4];
	
		canbuf[0]=FLOOD_LED;
		canbuf[1]	=color;
		canbuf[2]	=40;		
		SI24R1_SendBuf_Write(canbuf,id);
}




 

 
void  gamePack_Clear(gamePackData_TypeDef *temp)
{
		gamePackAction_TypeDef *gamePcakActionTemp = temp->head;
		while (gamePcakActionTemp)
		{
			gamePackAction_TypeDef* tmp = gamePcakActionTemp;
			gamePcakActionTemp = gamePcakActionTemp->next;
			my_free(tmp);
		}	
		memset(temp,0, sizeof(gamePackData_TypeDef));	
}

 
gamePackAction_TypeDef   *gamePackAction_Creatlink(void)
{
	gamePackAction_TypeDef *head = NULL;
	head = (gamePackAction_TypeDef *) my_malloc( sizeof(gamePackAction_TypeDef));
	if (!head)
	{
			return NULL;
	}
	memset(head,0, sizeof(gamePackAction_TypeDef));
	head->next = NULL;
	return head;
}

int   gamePackAction_InsertNode(gamePackAction_TypeDef *head, gamePackAction_TypeDef *padd)
{
	if(head== NULL)
	{
		head=padd;
		return 0;
	}	
	while (head->next != NULL)
	{
		head = head->next;
	}
	head->next = padd;
	return 0;
}
 
//void  ApiPacket( u8 * receverData , u8 dataLen)
//{
//	u8 i   ;
//	
//	gamePackData_TypeDef *gamePack;
//	
//	if(receverData[0]==1)
//	{
//		gamePack=&gamePack1;
//	}
//	else if(receverData[0]==2)
//	{
//		gamePack=&gamePack2;
//	}
//	else
//	{
//		gamePack=&gamePack3;
//	}
//	gamePack_Clear(gamePack);
//	gamePack->number=	receverData[1]*256+receverData[2];
// 
//	for(i=3;i<dataLen;i+=3)
//	{
//			gamePackAction_TypeDef *p;
//			p=gamePackAction_Creatlink();
//			p->floorID=receverData[i];	
//  		p->type=receverData[i];	
//			p->color=receverData[i+1];		
//			p->time=receverData[i+2];
//			p->next=NULL;
//		  if(i==3)
//			{
//				gamePack->head=p;				
//			}
//			else
//			{
//				gamePackAction_InsertNode(gamePack->head,p);			 		
//			}
//	}
//	
//}

 

