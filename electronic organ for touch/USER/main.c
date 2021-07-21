#include "main.h"
#define PRESS 1			//按下
#define RELEASE 2		//松开
#define NOTHING 3		//相同
 
u8 key_status;
 
u8 	SetIDTaskFlag=0;
u8  GameRunTaskFlag=0;
u8   playCount=1; //玩家数量
static u8 keybuf=0xFF;
u8 flag_count_1ms;
u8 flag_1ms;
u8 flag_10ms=0;
u16 count_10ms=0; 

u8 colorTemp,liangduTemp;
u8 floorConTime;



u8 LED_LightTime50Ms;
SYSCFG_TYPE sysCfg;

//系统时钟为36M
int main()
{
	u8 i,res  ;	//标志位
  u8 count50Ms=0;
	 
 	 
	LED_Init();
	
	KEY_Init();
 	delay_init();
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	usart1_init(19200);
	
	sprintf(sysCfg.device_id,"%s","X2_1_10001");
	
	
	//OUT_Init();	//灯条IO口的初始化
    
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
  //TIM2_Init(999,7199);//10Khz的计数频率，计数到1000为100ms 
  //wwdg_init(0X7F,0X5F,WWDG_Prescaler_8);//计数器值为7f,窗口寄存器为5f,分频数为8	 
	
	
  //NRF24L01_Init();
	//while(NRF24L01_Check());
	//NRF24L01_RX_Mode();
	//IRQ_intr_init();
	
	
	//TIM2_Init(9,47); 		//1000k    38k*2=76k
	TIM3_Init(99,359);//10Khz的计数频率，计数到100为10ms  	
	//uart1_disable();
	
	LED_B=1;
	//LED_G=1;
	//LED_R=1;
	
 
	for(i=0;i<10;i++)
	{
	   
	}
 	TIM_Cmd(TIM3, ENABLE); 
  delay_ms(100);
	//printf("%s","sysSatrt!!!\r\n");	
  delay_ms(100);
	//printf("%s","sysSatrt!!!\r\n");	 	
  while(1)
	{
		 Touch_Scan(ledColor);
		 //if(res)
		 {
			//	memset(ledColor,3,8);
			 // ledColor[res-1]=1;
			   LED_COLOR_2_595(ledColor);
		 }
			delay_ms(10);
	}
  while(1)
	{
		if(flag_10ms)
		{
			flag_10ms=0;	
			if( gamePack1.IsUsed)
			{
				GameRun1_task();
			}
			if(	++count_10ms>=5)
			{
					count_10ms=0;
				  TIM_Cmd(TIM3, DISABLE);  //关TIMx		
					LED_display_temp(led_table, 255);
					TIM_Cmd(TIM3, ENABLE);  //使能TIMx
					WifiUsart_task();
			  //UartBufDeal();		
				//SI24R1_task();		
				//Ball_OnlineDeal();
				
				if(++count50Ms==10)
				{
					count50Ms=0;
					LED0=~LED0;
				}
//				 if(LED_LightTime50Ms>0)
//				 {
//						LED_LightTime50Ms--;
//					  LED0=1;
//				 }
//				 else 
//				 {
//						LED0=0;
//				 }	 
			}	
			if(gamePack1.noTouchTime)
			{
					gamePack1.noTouchTime--;			
			}
		}
		
		if(flag_1ms)
		{
				flag_1ms=0;
				res=KEY_Scan();
				if(res!=0  &&  gamePack1.noTouchTime==0 ) 
				{
					 gamePack1.nowKey=res;	
				}				
		}

	}
}

//定时器3中断服务程序
void TIM3_IRQHandler(void)   //TIM3中断
{
	if(floorConTime>0) floorConTime--;
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
	{
		flag_1ms=1;
		 if(++flag_count_1ms>=10)
		 {
			 flag_count_1ms=0;
				flag_10ms=1;
		 }
		
		 	
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //清除TIMx更新中断标志 
	}
}
