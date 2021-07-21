#include "gamePackDeal.h"

#include "gameTask.h"

BallSta_Type ballSta;
//u8 floodSta[MAX_BALL+1];
//u8 GameBallOnline[MAX_BALL+1];
u8 Mode1canbufTemp[8];
gamePackAction_TypeDef *Mode1_game_temp;
u8 Mode1_liangdu_temp;
u16 Mode1noRecDataTime_10ms=0;

//10ms ????
u8 GameRunFlag1=0 ;
u8 GameRunDelay10ms1=0;
static u16 Step_Time_10ms,FinGroup=0,count10ms;
	
static gamePackData_TypeDef *gamePcakTemp;

void	gameStart(void)
{

}
void	gameOver(void)
{

}
 

/*
void Mode1_floodCon(gamePackAction_TypeDef *game_temp,u8 liangdu)
{
			static u8 temp,i;

      Mode1noRecDataTime_10ms=0;
 
		//	LED595SendData(0xff);
	    delay_ms(100);
	
			temp=0xff^(0x80>>(game_temp->floorID-1));
	   // LED595SendData( temp);
	
			gamePack1.nowFloorID=game_temp->floorID;
			gamePack1.nowKey=0;
			gamePack1.noTouchTime=15;  //0.4s
	
	   LED0=~LED0;
}*/
void Mode1_floodCon(gamePackAction_TypeDef *game_temp,u8 liangdu)
{		 
   	u8 i;
 		for(i=0;i<LED_LONG;i++)
		{
				led_table[i]=color_no;
		}	

		gamePack1.nowFloorID=game_temp->floorID;
		gamePack1.nowKey=0;
		
	  if(gamePack1.beforeFloorID == gamePack1.nowFloorID)
		{
			if(gamePack1.beforeColor == color_red)				//�ظ���ʱ���л���ɫ
				led_table[game_temp->floorID]=color_blue;	
			else led_table[game_temp->floorID]=color_red;
		}
		else
		{
			led_table[game_temp->floorID]=color_green;
		}
		
		gamePack1.beforeColor=led_table[game_temp->floorID];
		gamePack1.beforeFloorID=gamePack1.nowFloorID;		
		
		

		
		TIM_Cmd(TIM3, DISABLE);  //��TIMx		
		LED_display_temp(led_table, 255);
		TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx
 

	//   LED0=~LED0;
}

/*
 * ???????????
 *  floorID ��ǰ���µ����Ƿ���ȷ
 *  game_temp ��ǰ���еĲ���
*/
u8 Mode1_checkFloodStaOnlyOne(u8 floorID,gamePackAction_TypeDef *game_temp)
{
		u8 sta=0; //0 �ذ�û�����   1�ذ尴��OK
	  if(game_temp->floorID & (0x01<<floorID))
		{
			sta=1;
		}
		return sta;
}

/*
 *  ��ǰ�����Ƿ����
 *  floodSta ��ǰ�ذ�״̬
 *  game_temp ��ǰ���еĲ���
 *  ����ֵ0 û�����  1����� 
*/

u8 Mode1_checkFloodSta(u8 *floodSta,gamePackAction_TypeDef *game_temp)
{
		u8 i=0,sta=1; //0 �ذ�û�����   1�ذ尴��OK

		for(i=1;i<=6;i++)
		{
			 if(game_temp->floorID & (0x01<<i))
			 {
				  if(floodSta[i])  //��Ӧ����û�а���
					{ 
						sta=0;
					}	 
			 }
		}		
		return sta;
}
 
void overGame()
{
  cJSON *returnDataJson = cJSON_CreateObject();
  char * WifiDataStr;
	
	gamePackData_TypeDef *gamePack;
  gamePack=&gamePack1;
	gamePack->IsUsed=0;
	
  cJSON_AddStringToObject(returnDataJson, "Api", 	"StopMusic");
  cJSON_AddStringToObject(returnDataJson, "Flag", 	"true");
	cJSON_AddStringToObject(returnDataJson, "DriverID", 	sysCfg.device_id);
	
	cJSON_AddNumberToObject(returnDataJson, "RightCount", 	gamePack->RightCount);
	cJSON_AddNumberToObject(returnDataJson, "ErrCount", 	gamePack->ErrCount);
	cJSON_AddNumberToObject(returnDataJson, "FinGroup", 	gamePack->FinGroup);
	cJSON_AddNumberToObject(returnDataJson, "IsUsed", 	gamePack->IsUsed);
	
	WifiDataStr=cJSON_PrintUnformatted(returnDataJson);
	
	WifiSendData( WifiDataStr);
	my_free(WifiDataStr);
  cJSON_Delete(returnDataJson);  // ɾ��		json
											 
	
}

gamePackAction_TypeDef *gameAction_temp ;
void getNextPack(void)
{
						Step_Time_10ms=0;					 
						if(gameAction_temp->next == NULL  )	//���һ��
						{								
								gamePack1.FinGroup++;
								gamePack1.NowStep=1;
								if(gamePack1.FinGroup == gamePack1.Group)//ȫ����� 
								{
										//FinGroup=0;
										CloseAllFloor();	
										overGame();			
										GameRunFlag1=0; // ������Ϸ
										return ;
								}
								else 
								{
									 gameAction_temp=gamePcakTemp->head; //????
									 Mode1_floodCon(gameAction_temp, 75);						//���µ�
								}
						}
						else		////??????????
						{
							gamePack1.NowStep++;
							gameAction_temp=gameAction_temp->next;
							Mode1_floodCon(gameAction_temp, 75);		//
						}	
}	

//10ms ����һ��
void GameRun1_task(void)
{
	static u8 rightFlag;
	static u8 i=0;


	if(GameRunDelay10ms1>0) //��ʱ����
	{
		GameRunDelay10ms1--;
		return;
	}
	else if(GameRunFlag1 == 0) //�ս�����Ϸ����ʼ
	{
	  LightAllFloor();
		GameRunFlag1=1;
		GameRunDelay10ms1=50;
 
		Step_Time_10ms=0;
 
		return ;
	}
	else if(GameRunFlag1 == 1)  //�ս�����Ϸ����
	{
 	  CloseAllFloor();
		GameRunFlag1=2;
		GameRunDelay10ms1=20;
		return ;
	}
	else if(GameRunFlag1 == 2) // ����ȡֵ
	{
		gamePcakTemp=&gamePack1;
		gameAction_temp = gamePcakTemp->head;
	
		Mode1_floodCon(gameAction_temp, 75);
		GameRunFlag1=3;
		gamePack1.Timer=0;
		gamePack1.NowStep=1;
		gamePack1.FinGroup=0;
	}
	else if(GameRunFlag1 == 3) //��������
	{
		Mode1noRecDataTime_10ms++;
		
		if(++count10ms>=10)
		{
			count10ms=0;
			gamePack1.Timer++;
		}
//		res=Mode1_ReceiverBuf_Read(canbuf);  //canbuf[0] = id    canbuf[1] = 0 ???
// 		if(res)
//		{
// 			Mode1noRecDataTime_10ms=0;
//			floodSta[canbuf[0]]=canbuf[1];
//		}

 		if(Step_Time_10ms<10000)Step_Time_10ms++;

		if((gameAction_temp->time>0)   && (Step_Time_10ms > (gameAction_temp->time*50)) ) //ʱ������
		{
			  gamePack1.nowKey=gamePack1.nowFloorID;
				rightFlag=0;
				if(gamePack1.nowFloorID == 0) //����
				{
					gamePack1.RightCount++;				
					getNextPack();
				}
		}
		
		if(gamePack1.nowKey ) //��ǰ�����Ƿ����
		{
			  if(gamePack1.nowKey == gamePack1.nowFloorID) //������ȷ
				{
					  if(rightFlag)
						{
								gamePack1.RightCount++;						
						}
						else
						{
								gamePack1.ErrCount++;				
						}
						CloseAllFloor();
						delay_ms(10);
						gamePack1.noTouchTime=20;  //0.25s
						rightFlag=1;
						getNextPack();		
				}
				else 
				{
						rightFlag=0;				
				}
		}
		
	}
}

 



