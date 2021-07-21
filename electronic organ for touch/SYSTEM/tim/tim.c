#include "tim.h"
#include "main.h"
u8 msec_time;								//计时毫秒
u8 sec_time;								//计时秒
u8 miu_time;								//计时分
u8 hor_time;								//计时时
u8 key_time;								//按键容错时间
u8 standby_time;							//待机时间

void TIM3_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStrue;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能
	TIM_TimeBaseInitStrue.TIM_Period=arr;	 //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseInitStrue.TIM_Prescaler=psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseInitStrue.TIM_ClockDivision=TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseInitStrue.TIM_CounterMode=TIM_CounterMode_Up; //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStrue); //根据指定的参数初始化TIMx的时间基数单位
	
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器
	
	TIM_Cmd(TIM3, ENABLE);  //使能TIMx
}

void TIM2_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStrue;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能
	
	TIM_TimeBaseInitStrue.TIM_Period=arr;	 //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseInitStrue.TIM_Prescaler=psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseInitStrue.TIM_ClockDivision=TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseInitStrue.TIM_CounterMode=TIM_CounterMode_Up; //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStrue); //根据指定的参数初始化TIMx的时间基数单位
	
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器
}

void TIM2_IRQHandler(void)   //TIM2中断
{
	static char flag=0;
	
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //检查TIM2更新中断发生与否
	{
		flag=!flag;
//		if(LED_ON_FLAG)
//			LED_IR= flag;
//		else 
//			LED_IR=0;
//		
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //清除TIMx更新中断标志 
	}
}

