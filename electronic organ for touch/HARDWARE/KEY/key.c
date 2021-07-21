#include "key.h"
#include "sys.h"
#include "delay.h"

void KEY_Init(void)
{
	GPIO_InitTypeDef GPIO_Initstruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_Initstruct.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_Initstruct.GPIO_Pin=GPIO_Pin_10|GPIO_Pin_9|GPIO_Pin_8|GPIO_Pin_7|GPIO_Pin_6|GPIO_Pin_5|GPIO_Pin_4|GPIO_Pin_3;
	GPIO_Init(GPIOB,&GPIO_Initstruct);	
		
}
///*******************************************************************************
//* �� �� ��         : exti_init
//* ��������		   : �ⲿ�ж�2�˿ڳ�ʼ������	   
//* ��    ��         : ��
//* ��    ��         : ��
//*******************************************************************************/
//void key_irq_intr_init(void)  //�ⲿ�жϳ�ʼ��
//{
//	exti_inittypedef exti_initstructure;

//	nvic_inittypedef nvic_initstructure;

//	/* ����gpioʱ�� */
//	rcc_apb2periphclockcmd(rcc_apb2periph_afio,enable);
//	rcc_apb2periphclockcmd(rcc_apb2periph_gpioa,enable);
// 
//	 
//	gpio_extilineconfig(gpio_portsourcegpioa, gpio_pinsource0);//ѡ��gpio�ܽ������ⲿ�ж���·
//	//�˴�һ��Ҫ��ס���˿ڹܽż����ж��ⲿ��·
//	/* �����ⲿ�жϵ�ģʽ */ 
//	exti_initstructure.exti_line=exti_line0;
//	exti_initstructure.exti_mode=exti_mode_interrupt;
//	exti_initstructure.exti_trigger=exti_trigger_falling;
//	exti_initstructure.exti_linecmd = enable;
//	exti_init(&exti_initstructure); 
//	
//	/* ����nvic���� */	
//	nvic_prioritygroupconfig(nvic_prioritygroup_2);		 
//	nvic_initstructure.nvic_irqchannel = exti0_irqn; 	//��exti0��ȫ���ж�
//	nvic_initstructure.nvic_irqchannelpreemptionpriority = 0; //��ռ���ȼ�Ϊ0
//	nvic_initstructure.nvic_irqchannelsubpriority = 2;		  //��Ӧ���ȼ�Ϊ0
//	nvic_initstructure.nvic_irqchannelcmd = enable; 		  //ʹ��
//	nvic_init(&nvic_initstructure); 		
//}

//void exti0_irqhandler()	   //�ⲿ�ж�0�жϺ���
//{
//	u8 canbuf[8],res;
//	if(exti_getitstatus(exti_line0)==set)
//	{
//		exti_clearitpendingbit(exti_line0);//���exti��·����λ	
//		if(si24r1mode==0) //����ģʽ
//		{
//			res=nrf24l01_rxpacket(canbuf);
//			if(res)
//			{
//				si24r1_recdeal(canbuf);			
//			}		
//		}	
//	}	
//}

u8 Touch_Scan(u8 *data)
{
	if(KEY1==0)
	{
		 	data[0]=1;
	}
	else
	{
			data[0]=3;
	}		

	if(KEY2==0)
	{
		 	data[1]=1;
	}
	else
	{
			data[1]=3;
	}		

	if(KEY3==0)
	{
		 	data[2]=1;
	}
	else
	{
			data[2]=3;
	}		

	if(KEY4==0)
	{
		 	data[3]=1;
	}
	else
	{
			data[3]=3;
	}		

	if(KEY5==0)
	{
		 	data[4]=1;
	}
	else
	{
			data[4]=3;
	}		

	if(KEY6==0)
	{
		 	data[5]=1;
	}
	else
	{
			data[5]=3;
	}		

	if(KEY7==0)
	{
		 	data[6]=1;
	}
	else
	{
			data[6]=3;
	}	

	if(KEY8==0)
	{
		 	data[7]=1;
	}
	else
	{
			data[7]=3;
	}	
	
}
u8 KEY_Scan(void )
{
	static u8 readCount=0,noTouchCount1MS=0;
	u8 keyData=0;
	
//if( KEY0==0)
//{	 
//	if(++readCount>1)keyData=1;	
//	noTouchCount1MS=0;
//}
	
	if(KEY1==0)
	{
		 if(++readCount>1)keyData=1;
			noTouchCount1MS=0;		
	}
	else if(KEY2==0)
	{
		 if(++readCount>1)keyData=2;	
			noTouchCount1MS=0;
	}
	else if(KEY3==0)
	{
		 if(++readCount>1)keyData=3;
			noTouchCount1MS=0;		
	}
	else if(KEY4==0)
	{
		 if(++readCount>1)keyData=4;
			noTouchCount1MS=0;		
	}
	else if(KEY5==0)
	{
		 if(++readCount>1)keyData=5;	
			noTouchCount1MS=0;
	}
	else if(KEY6==0)
	{
		 if(++readCount>1)keyData=6;
			noTouchCount1MS=0;		
	}
	else if(KEY7==0)
	{
		 if(++readCount>1)keyData=7;	
			noTouchCount1MS=0;
	}
  else if(KEY8==0)
	{
		 if(++readCount>1)keyData=8;	
			noTouchCount1MS=0;
	}
	else
	{
		  if(++noTouchCount1MS>100)
			{
					readCount=0;
				  noTouchCount1MS=0;
			}	
	}
	
	return keyData;
}






