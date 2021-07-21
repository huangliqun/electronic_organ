
#ifndef _USER_GAME_PACK1_H
#define _USER_GAME_PACK1_H


 #include "main.h"   
 #include "sys.h" 
 #include "gameTask.h"

#define  MAX_BALL 6

#define CAN_BASE_ID 0xf0
#define MAIN_SERVER_ID  101



//0xff 0xff 0xff 0xff
typedef enum FLOOD_REC_CMD
{
	FLOOD_SETID_BEGIN =0x00,
	FLOOD_SETID_OVER=1,
	FLOOD_LED   =0x02	,		//控制led
	FLOOD_BREATH =0x03 ,	// 呼吸  
	FLOOD_ALWAYS =0x04, 		// 长亮
	FLOOD_LIUSHUI=0x05,
  FLOOD_RADIO1=0x06,    //广播控制 球 1-10
	FLOOD_RADIO2=0x07	,		//广播控制 球 11-20
  FLOOD_RADIONO1=0x08,    //广播控制 球 不能踩灭 ipad直接控制模式下使用
	FLOOD_RADIONO2=0x09	,   	//广播控制 球 不能踩灭
	FLOOD_SHUTDOWN=0x5a
}FLOOD_REC_CMD;

typedef struct BallSta_Type
{
	u8 floodSta[MAX_BALL+1];  //1-6
	u8 Online[MAX_BALL+1];
	u8 bat[MAX_BALL+1];
	u8 noRecTime[MAX_BALL+1];
}BallSta_Type;
 
//extern u8 floodSta[MAX_BALL+1];
//extern u8 GameBallOnline[MAX_BALL+1];
extern BallSta_Type ballSta;

extern u8 GameRunFlag1 ;
 

extern u8 SetIDFlag ;
extern gamePackAction_TypeDef *gameAction_temp ;
void Mode1_floodCon(gamePackAction_TypeDef *game_temp,u8 liangdu);

void  gameStart(void);
void  gameOver(void);
void SetID_task( void);
void GameRun1_task(void);
 
void  LightAllFloor(void);
void  CloseAllFloor(void);

#endif
