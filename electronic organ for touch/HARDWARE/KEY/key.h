#ifndef __KEY_H
#define __KEY_H
#include "sys.h"

#define KEY0 	    GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)
#define KEY1 	    GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)
#define KEY2 	    GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)
#define KEY3 	    GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8)
#define KEY4 	    GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)
#define KEY5 	    GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)
#define KEY6 	    GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)
#define KEY7 	    GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4)
#define KEY8 	    GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3)

#define TOUCH 	  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)
#define IR_READ   GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)
#define BLEREAD 	  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8)

#define KEY0_PRES 1


void KEY_Init(void);
u8 KEY_Scan( void);
u8 Touch_Scan(u8 *data);
#endif

