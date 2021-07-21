#include "XT1506S.h"	
#include "delay.h"
u8 led_table[LED_LONG];

void LEDOUT_Init()
{
	GPIO_InitTypeDef GPIO_InitStr;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC ,ENABLE);  //使能IO口时钟
	GPIO_InitStr.GPIO_Mode=GPIO_Mode_Out_PP;
	//GPIO_InitStr.GPIO_Mode=GPIO_Mode_Out_OD;
	
	GPIO_InitStr.GPIO_Pin=GPIO_Pin_15 | GPIO_Pin_14;
	GPIO_InitStr.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStr);
}
/*
void V1()
{
   out=1;out=1;out=1;out=1; 
	 out=1;out=1; out=1; out=1; 
	 out=0;out=0; out=0;  
}

void V0()           
{
	 out=1;out=1;out=1;  
	 out=0;out=0;out=0;out=0;
	out=0;out=0; out=0; out=0;  
}*/

void V1()
{
   out=0;out=0;out=0;out=0; 
	 out=0;out=0; out=0; out=0; 
	 out=1;out=1; out=1;  
}

void V0()           
{
	 out=0;out=0;out=0;  
	 out=1;out=1;out=1;out=1;
	out=1;out=1; out=1; out=1;  
}

void WS_Reset() 
{ 
	out = 1; 
	delay_us(60);  
	out = 0; 
	out = 1; 
}


void V1_2()
{
   out2=1;out2=1;out2=1;out2=1; 
	 out2=1;out2=1;out2=1; out2=1; 
	 out2=0;out2=0; out2=0;  
}

void V0_2()           
{
	 out2=1;out2=1;out2=1; 
	 out2=0;out2=0;out2=0;out2=0; 
	 out2=0;out2=0;out2=0; out2=0; 
}
void WS_Reset_2() 
{ 
	out2 = 0; 
	delay_us(60);  
	out2 = 1; 
	out2 = 0; 
}



/*
*功能：  控制一个LED
*color ：颜色 ， 
*liangdu  ： 亮度
*/
void display_one_temp(LEDcolorType color,u8 liangdu)
{
     u8 i;
	
	if(color&0x02)
	{
		for(i=0;i<8;i++)
		{
			if(liangdu&(0x80>>i))V1();
			else V0();
		}
	}
	else{	V0(); V0();V0();V0();V0();V0();V0();V0();}	
	if(color&0x01)
	{
		for(i=0;i<8;i++)
		{
			if(liangdu&(0x80>>i))V1();
			else V0();
		}
	}
	else{	V0(); V0();V0();V0();V0();V0();V0();V0();}
	if(color&0x04)
	{
		for(i=0;i<8;i++)
		{
			if(liangdu&(0x80>>i))V1();
			else V0();
		}
	}	 
	else{	V0(); V0();V0();V0();V0();V0();V0();V0();}

}


/*
*功能：  控制一个LED
*color ：颜色 ， 
*liangdu  ： 亮度
*/
void display_one_temp_2(LEDcolorType color,u8 liangdu)
{
     u8 i;
	
	if(color&0x02)
	{
		for(i=0;i<8;i++)
		{
			if(liangdu&(0x80>>i))V1_2();
			else V0_2();
		}
	}
	else{	V0_2(); V0_2();V0_2();V0_2();V0_2();V0_2();V0_2();V0_2();}	
	if(color&0x01)
	{
		for(i=0;i<8;i++)
		{
			if(liangdu&(0x80>>i))V1_2();
			else V0_2();
		}
	}
	else{	V0_2(); V0_2();V0_2();V0_2();V0_2();V0_2();V0_2();V0_2();}	
	if(color&0x04)
	{
		for(i=0;i<8;i++)
		{
			if(liangdu&(0x80>>i))V1_2();
			else V0_2();
		}
	}	 
	else{	V0_2(); V0_2();V0_2();V0_2();V0_2();V0_2();V0_2();V0_2();}	
}

void LED_display_temp(u8 * led_table,u8 liangdu)	 //	  light_num 0-50;   color 1 2 3 RGB 
{
		u8 i;
		for(i=0;i<LED_LONG;i++)
		{	 	
			display_one_temp( (LEDcolorType) led_table[i],liangdu);
		}
	//	WS_Reset();
		
//		for(i=0;i<LED_LONG;i++)
//		{	 	
//			display_one_temp_2( (LEDcolorType) led_table[i],liangdu);
//		}
//		WS_Reset_2();
} 





void LED_All_color(LEDcolorType color  )
{
	  u8 i;
		for(i=0;i<LED_LONG;i++)
		{
				led_table[i]=color;
		}
		TIM_Cmd(TIM3, DISABLE);  //关TIMx
		
				
		LED_display_temp(led_table, 255);
		TIM_Cmd(TIM3, ENABLE);  //使能TIMx
}
 
void LED_Start()
{
	  u8 i;
	
	  for(i=0;i<2;i++)
		{
			LED_All_color(color_red);						//开始的闪灯
			delay_ms(500);
			LED_All_color(color_blue);
			delay_ms(500);
			LED_All_color(color_green);
			delay_ms(500);		
		}
		
		LED_All_color(color_no);
	  LED_All_color(color_no);
}


void LED_LIUSHUI()
{
	  u8 i;
		static u8 count=0;
	
		count++;
	  if(count>=LED_LONG) count=0;
		for(i=0;i<LED_LONG;i++)
		{
				led_table[i]=color_no;
		}
		led_table[count]=color_green;
		
		TIM_Cmd(TIM3, DISABLE);  //关TIMx		
		LED_display_temp(led_table, 50);
		TIM_Cmd(TIM3, ENABLE);  //使能TIMx
}


void  CloseAllFloor(void)
{
	LED_All_color(color_no);
}

void  LightAllFloor(void)
{
	LED_All_color(color_blue);
}

