#ifndef __LED_H
#define __LED_H
#include "main.h"

//3 关  2蓝色   1绿色  0白色 
#define LED_COLOR_OFF   3
#define LED_COLOR_BLUE   2
#define LED_COLOR_GREEN   1
#define LED_COLOR_WHITE   0
extern u8 ledColor[8];

#define LED0 PBout(0)
#define POWER1 PBout(5)
#define LED_IR PBout(0)


 

#define SH_CP PBout(13)
#define ST_CP PBout(12)
#define DATA  PBout(11)

 



#define LED_B PCout(15)
#define LED_G PCout(14)
#define LED_R PCout(13)


void LED_Init(void);
void LED_COLOR_2_595(u8 *color);


/*
void LED_All_color(u8 color);


void LED595SendData(unsigned char OutData);
void  CloseAllFloor(void);
void  LightAllFloor(void);
*/
#endif

