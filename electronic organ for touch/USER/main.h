#ifndef __MAIN_H
#define __MAIN_H	 
#include "sys.h"	    
#include "stm32f10x.h"
 
#include "delay.h"
#include "wwdg.h"
#include "tim.h"
#include "key.h"
#include "led.h"
#include "eeprom.h"
#include "gamePackDeal.h"
//#include "canType.h"
#include "gameTask.h"
#include "usart.h"
//#include "uartDeal.h"
#include "24l01.h"

#include "24l01Buf.h"
#include "cJSON.h"
#include "cJsonDataDeal.h"
#include "wifiusart.h"
#include "malloc.h"
#include "XT1506S.h"
typedef struct SYSCFG_TYPE{
	char device_id[32];
	char bat;     
	char bright;  
 } SYSCFG_TYPE;


extern SYSCFG_TYPE sysCfg;


extern u8 	SetIDTaskFlag;
extern u8  GameRunTaskFlag ;
extern u8   playCount;
extern u8 LED_LightTime50Ms;

 #endif


