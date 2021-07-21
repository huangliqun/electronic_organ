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
			if(gamePack1.beforeColor == color_red)				//重复的时候切换颜色
				led_table[game_temp->floorID]=color_blue;	
			else led_table[game_temp->floorID]=color_red;
		}
		else
		{
			led_table[game_temp->floorID]=color_green;
		}
		
		gamePack1.beforeColor=led_table[game_temp->floorID];
		gamePack1.beforeFloorID=gamePack1.nowFloorID;		
		
		

		
		TIM_Cmd(TIM3, DISABLE);  //关TIMx		
		LED_display_temp(led_table, 255);
		TIM_Cmd(TIM3, ENABLE);  //使能TIMx
 

	//   LED0=~LED0;
}

/*
 * ???????????
 *  floorID 当前按下的球是否正确
 *  game_temp 当前进行的步骤
*/
u8 Mode1_checkFloodStaOnlyOne(u8 floorID,gamePackAction_TypeDef *game_temp)
{
		u8 sta=0; //0 地板没有完成   1地板按下OK
	  if(game_temp->floorID & (0x01<<floorID))
		{
			sta=1;
		}
		return sta;
}

/*
 *  当前步骤是否完成
 *  floodSta 当前地板状态
 *  game_temp 当前进行的步骤
 *  返回值0 没有完成  1：完成 
*/

u8 Mode1_checkFloodSta(u8 *floodSta,gamePackAction_TypeDef *game_temp)
{
		u8 i=0,sta=1; //0 地板没有完成   1地板按下OK

		for(i=1;i<=6;i++)
		{
			 if(game_temp->floorID & (0x01<<i))
			 {
				  if(floodSta[i])  //对应的球还没有按下
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
  cJSON_Delete(returnDataJson);  // 删除		json
											 
	
}

gamePackAction_TypeDef *gameAction_temp ;
void getNextPack(void)
{
						Step_Time_10ms=0;					 
						if(gameAction_temp->next == NULL  )	//完成一组
						{								
								gamePack1.FinGroup++;
								gamePack1.NowStep=1;
								if(gamePack1.FinGroup == gamePack1.Group)//全部完成 
								{
										//FinGroup=0;
										CloseAllFloor();	
										overGame();			
										GameRunFlag1=0; // 结束游戏
										return ;
								}
								else 
								{
									 gameAction_temp=gamePcakTemp->head; //????
									 Mode1_floodCon(gameAction_temp, 75);						//亮新的
								}
						}
						else		////??????????
						{
							gamePack1.NowStep++;
							gameAction_temp=gameAction_temp->next;
							Mode1_floodCon(gameAction_temp, 75);		//
						}	
}	

//10ms 进行一次
void GameRun1_task(void)
{
	static u8 rightFlag;
	static u8 i=0;


	if(GameRunDelay10ms1>0) //延时作用
	{
		GameRunDelay10ms1--;
		return;
	}
	else if(GameRunFlag1 == 0) //刚进入游戏发开始
	{
	  LightAllFloor();
		GameRunFlag1=1;
		GameRunDelay10ms1=50;
 
		Step_Time_10ms=0;
 
		return ;
	}
	else if(GameRunFlag1 == 1)  //刚进入游戏发关
	{
 	  CloseAllFloor();
		GameRunFlag1=2;
		GameRunDelay10ms1=20;
		return ;
	}
	else if(GameRunFlag1 == 2) // 初次取值
	{
		gamePcakTemp=&gamePack1;
		gameAction_temp = gamePcakTemp->head;
	
		Mode1_floodCon(gameAction_temp, 75);
		GameRunFlag1=3;
		gamePack1.Timer=0;
		gamePack1.NowStep=1;
		gamePack1.FinGroup=0;
	}
	else if(GameRunFlag1 == 3) //正常运行
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

		if((gameAction_temp->time>0)   && (Step_Time_10ms > (gameAction_temp->time*50)) ) //时间跳过
		{
			  gamePack1.nowKey=gamePack1.nowFloorID;
				rightFlag=0;
				if(gamePack1.nowFloorID == 0) //空音
				{
					gamePack1.RightCount++;				
					getNextPack();
				}
		}
		
		if(gamePack1.nowKey ) //当前步骤是否完成
		{
			  if(gamePack1.nowKey == gamePack1.nowFloorID) //击打正确
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

 



