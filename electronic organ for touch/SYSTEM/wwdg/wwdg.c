#include "wwdg.h"
//����WWDG������������ֵ,Ĭ��Ϊ���. 
u8 WWDG_CNT=0x7f; 

//���ڿ��Ź��жϷ������
void WWDG_NVIC_Init()
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = WWDG_IRQn;    //WWDG�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;   //��ռ2�������ȼ�3����2	
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	 //��ռ2�������ȼ�3����2	
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE; 
	NVIC_Init(&NVIC_InitStructure);//NVIC��ʼ��
}

//������WWDG��������ֵ
void WWDG_Set_Counter(u8 cnt)
{
    WWDG_Enable(cnt);//ʹ�ܿ��Ź� ,	���� counter .	 
}

void wwdg_init(u8 tr,u8 wr,u32 fprer)
{
	WWDG_DeInit();
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_WWDG, ENABLE);	//ʹ�ܿ��Ź�ʱ��
	WWDG_CNT=tr&0x7f;
	WWDG_SetPrescaler(fprer);	//���÷�Ƶϵ��
	WWDG_SetWindowValue(wr);	//�����ϴ���ֵ
	WWDG_Enable(WWDG_CNT);	 	//ʹ�ܿ��Ź� ,	���� counter .
	WWDG_EnableIT();   			//�������ڿ��Ź��ж�
	WWDG_NVIC_Init();
	WWDG_ClearFlag(); 
}

void WWDG_IRQHandler(void)
{

	WWDG_SetCounter(WWDG_CNT);	  //�������˾��,���ڿ��Ź���������λ

	WWDG_ClearFlag();	  //�����ǰ�����жϱ�־λ

}
