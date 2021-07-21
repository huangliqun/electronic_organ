#include "led.h"
#include "stm32f10x.h"
u8 ledColor[8];
//3 关  2蓝色   1绿色  0白色 
void LED_Init()
{
	GPIO_InitTypeDef GPIO_InitStr;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);/*???????????*/
 	GPIO_PinRemapConfig(GPIO_Remap_PD01,ENABLE);/*??PD01???*/
	GPIO_PinRemapConfig(GPIO_Remap_PD01,ENABLE);/*??PD01???*/
 
	GPIO_InitStr.GPIO_Mode=GPIO_Mode_Out_PP;
 	GPIO_InitStr.GPIO_Pin=GPIO_Pin_11 |GPIO_Pin_12 | GPIO_Pin_13 ;  //控制RGB的三个IO
	GPIO_InitStr.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStr);
}

 
//#define SH_CP PBout(1)
//#define ST_CP PBout(3)
//#define DATA  PBout(4)
void LED595SendData(unsigned char OutData)
{
    u8 i;
    for(i=0; i<8; i++) 
    {
        SH_CP=0; 
        if( OutData  & (0x80>>i))
        {
            DATA=1;                 
        }
        else
        {
             DATA=0;
        }
        SH_CP=1;  
				delay_us(2);
    }
		 ST_CP=0;
		 delay_us(2);
		 ST_CP=1;
}

void LED_COLOR_2_595(u8 *color)
{
		u8 temp1,temp2;
	
		temp1=color[0]+(color[1]<<2)+(color[2]<<4)+(color[3]<<6);
	  temp2=color[4]+(color[5]<<2)+(color[6]<<4)+(color[7]<<6);

	  LED595SendData(temp2);
		LED595SendData(temp1);
		
}

/*
void LED_All_color(u8 color)
{
	  u8 i;
	  if(color&0x01)
		{
				LED_R=1;
		}
		else
		{
				LED_R=0;
		}
		if(color&0x02)
		{
				LED_G=1;
		}
		else
		{
				LED_G=0;
		}
		if(color&0x04)
		{
				LED_B=1;
		}
		else
		{
		    LED_B=0;
		}	
}


void  CloseAllFloor(void)
{
	LED595SendData(0xff);
}

void  LightAllFloor(void)
{
	LED595SendData(0x00);
}
*/
