#include "sys.h"
#include "usart.h"	
#include "led.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos ʹ��	  
#endif
 
 
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
_ttywrch(int ch)
{
ch = ch;
}

//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
    USART1->DR = (u8) ch;      
	return ch;
}

//#endif 

/*ʹ��microLib�ķ���*/
 /* 
int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (uint8_t) ch);

	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}	
   
    return ch;
}
int GetKey (void)  { 

    while (!(USART1->SR & USART_FLAG_RXNE));

    return ((int)(USART1->DR & 0x1FF));
}
*/
 
 char  usart1LEN ;
 char USART1_RX_STA;
 u8   USART1_RX_BUF[50];

 
 
  u8   USART2_RX_BUF[32];
  char USART2_RX_STA,USART2_RX_LEN;

#if EN_USART1_RX   //���ʹ���˽���
 
u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
 
u16 USART_RX_STA=0;       //����״̬���	  

 
 
void uart2_init(u32 bound){
    //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	   //ʹ��USART2��GPIOAʱ��
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);

	
	//USART2_TX   GPIOA.2
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 								//PA.2
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;						//�����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);										//��ʼ��GPIOA.2
   
  //USART2_RX	  GPIOA.3
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;                 //PA.3
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;			//��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);										//��ʼ��GPIOA.3

  //Usart2 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;                      //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;												    //�����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;														    //IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);																			          //����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;																	   //���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;										 //�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;												 //һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;														 //����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;								 //�շ�ģʽ
  USART_Init(USART2, &USART_InitStructure); 																		 //��ʼ������2
	USART_ITConfig(USART2,USART_IT_RXNE  ,ENABLE);																	 //����USART2�����ն�(DMA)
	
  USART_Cmd(USART2, ENABLE);                    																//ʹ�ܴ���2 	
}

 



u16 getHigh()
{
	u16 distance,i;
	if(USART2_RX_STA)
	{
            if(USART2_RX_LEN>=9)
            {
                if(USART2_RX_BUF[0]=='D'&&USART2_RX_BUF[1]=='=' )	
                {
                  distance=0;
                  for(i=2;i<9;i++)
                  {
                      if(USART2_RX_BUF[i] != '.' && USART2_RX_BUF[i] != 'm')
                      {
                         distance=distance*10+(USART2_RX_BUF[i]-'0');                           
                      }
                      if(USART2_RX_BUF[i] == 'm')
                      {
                        
                        /*  if(READ_IR!=0)
                          {
                              SendDataDeal(idNumber,distance );
                              SI24R1_senddata(tmp_buf,10);
                              SI24R1_RX_Mode(9);
                              for(i=0;i<300;i++)
                              {
                                 BEEP_OPEN_H;
                                 delay100us(2);
                                 BEEP_OPEN_L;
                                 delay100us(2);
                              }    
                              SendString("iHALT");                               
                          }*/
            
                          
                          if(distance>15000)distance=15000;
                          return (15000-distance)/100;
                      }
                  }
                 
                } 
                
                USART2_RX_LEN=0;
            }
            USART2_RX_STA=0;
	}
	return 0;
}


void USART2_IRQHandler(void)	//����2�жϺ���
{
	static u8 k;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
			USART2_RX_BUF[USART2_RX_LEN]=USART_ReceiveData(USART2);
		  USART2_RX_LEN++;
		  if(USART2_RX_LEN>31) USART2_RX_LEN=0;
	    USART2_RX_STA=1; 
	}
	
//  if(USART_GetITStatus(USART2,USART_IT_RXNE)!=Bit_RESET)//���ָ����USART�жϷ������
//	{
//	  USART1_RX_BUF[usart1LEN]=USART_ReceiveData(USART2);
//		usart1LEN++;
//	  USART1_RX_STA=1;
//	}
	
}

void USART_printf(u8 num,u8 *dat)
{
	u8 i;
	for(i=0;i<num;i++)
	{
		USART_SendData(USART2,dat[i]);
//		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
	}
}

void USART1_printf(u8 num,u8 *dat)
{
	u8 i;
	for(i=0;i<num;i++)
	{
		USART_SendData(USART1,dat[i]);
//		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);
	}
}


/*
void USART2_IRQHandler(void)                	//����2�жϷ������
	{
	u8 Res;
#if SYSTEM_SUPPORT_OS 		//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntEnter();    
#endif
		
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0xEF 0xFF��β)
		{
				Res =USART_ReceiveData(USART2);	//��ȡ���յ�������
				Light_LED(Res);
     } 
		
#if SYSTEM_SUPPORT_OS 	//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntExit();  											 
#endif
} */
#endif	



void uart1_disable(void )
{
	GPIO_InitTypeDef GPIO_InitStructure;	//����һ���ṹ�������������ʼ��GPIO
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
 
	/*  ����GPIO��ģʽ��IO�� */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9|GPIO_Pin_10;//TX			   //�������PA9
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;	    //�����������
	GPIO_Init(GPIOA,&GPIO_InitStructure);  /* ��ʼ����������IO */
 
}


void usart1_init(uint32_t bote)
{
	GPIO_InitTypeDef GPIO_InitStructure;	//����һ���ṹ�������������ʼ��GPIO

	USART_InitTypeDef  USART_InitStructure;	  //���ڽṹ�嶨��

	NVIC_InitTypeDef NVIC_InitStructure;//�жϽṹ�嶨��
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	 //��ʱ��
	
	/*  ����GPIO��ģʽ��IO�� */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;//TX			   //�������PA9
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;	    //�����������
	GPIO_Init(GPIOA,&GPIO_InitStructure);  /* ��ʼ����������IO */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;//RX			 //��������PA10
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;		  //ģ������
	GPIO_Init(GPIOA,&GPIO_InitStructure); /* ��ʼ��GPIO */


	USART_InitStructure.USART_BaudRate=bote;   //����������Ϊ9600	//������
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;		//���ݳ�8λ
	USART_InitStructure.USART_StopBits=USART_StopBits_1;			//1λֹͣλ
	USART_InitStructure.USART_Parity=USART_Parity_No;				//��Ч��
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None; //ʧ��Ӳ����
	USART_InitStructure.USART_Mode=USART_Mode_Tx | USART_Mode_Rx;	 //�������ͺͽ���ģʽ
	USART_Init(USART1,&USART_InitStructure);	/* ��ʼ��USART1 */
   USART_Cmd(USART1, ENABLE);		   /* ʹ��USART1 */
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//ʹ�ܻ���ʧ��ָ����USART�ж� �����ж�
	USART_ClearFlag(USART1,USART_FLAG_TC);//���USARTx�Ĵ������־λ


/* ����NVIC���� */

	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);    
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; 	   //��USART1��ȫ���ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 	 //��ռ���ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 			//��Ӧ���ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 			 //ʹ��
	NVIC_Init(&NVIC_InitStructure);

}


void playSound(char file,char index)
{
	USART_SendData(USART1,0x7E);
	USART_SendData(USART1,0x04);
	USART_SendData(USART1,0x42);
	USART_SendData(USART1,file);
	USART_SendData(USART1,index);
	USART_SendData(USART1,0xEF);
}



void USART1_IRQHandler(void)	//����1�жϺ���
{
		u8 Res;
 	//USART_ClearFlag(USART1,USART_IT_RXNE);
	if(USART_GetITStatus(USART1,USART_IT_RXNE)!=Bit_RESET)//���ָ����USART�жϷ������
	{
		Res =USART_ReceiveData(USART1);//(USART1->DR);	//��ȡ���յ�������
 		
 		g_uint8_tRecData[g_uint8_tRecData_Len] = Res;
   
		g_uint8_tRecData_Len = g_uint8_tRecData_Len+1;   
		if(g_uint8_tRecData_Len>=RXBUFSIZE)
		{
				g_uint8_tRecData_Len=0;
		}
	}
}

void clcUart2(void)
{
	u8 i;
		for(i=0;i<32;i++)
			USART2_RX_BUF[i]=0;
			USART2_RX_LEN=0;
			USART2_RX_STA=0;
}


