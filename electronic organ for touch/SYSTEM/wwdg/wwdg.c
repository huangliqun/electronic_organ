#include "wwdg.h"
//保存WWDG计数器的设置值,默认为最大. 
u8 WWDG_CNT=0x7f; 

//窗口看门狗中断服务程序
void WWDG_NVIC_Init()
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = WWDG_IRQn;    //WWDG中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;   //抢占2，子优先级3，组2	
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	 //抢占2，子优先级3，组2	
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE; 
	NVIC_Init(&NVIC_InitStructure);//NVIC初始化
}

//重设置WWDG计数器的值
void WWDG_Set_Counter(u8 cnt)
{
    WWDG_Enable(cnt);//使能看门狗 ,	设置 counter .	 
}

void wwdg_init(u8 tr,u8 wr,u32 fprer)
{
	WWDG_DeInit();
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_WWDG, ENABLE);	//使能看门狗时钟
	WWDG_CNT=tr&0x7f;
	WWDG_SetPrescaler(fprer);	//设置分频系数
	WWDG_SetWindowValue(wr);	//设置上窗口值
	WWDG_Enable(WWDG_CNT);	 	//使能看门狗 ,	设置 counter .
	WWDG_EnableIT();   			//开启窗口看门狗中断
	WWDG_NVIC_Init();
	WWDG_ClearFlag(); 
}

void WWDG_IRQHandler(void)
{

	WWDG_SetCounter(WWDG_CNT);	  //当禁掉此句后,窗口看门狗将产生复位

	WWDG_ClearFlag();	  //清除提前唤醒中断标志位

}
