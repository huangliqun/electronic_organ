
#include "main.h"
#include "string.h"
#include "stdlib.h"
#include "24l01Buf.h"
#include "gamePackDeal.h"
RF_ReceiverBuf_t Mode1ReceiverBuf,Mode2ReceiverBuf,Mode3ReceiverBuf;
RF_SendBuf_t RF_SendBuf;
u8 sendFailCount[MAX_BALL+1];
u8 SI24R1Mode; //=0����ģʽ   1����ģʽ
/*
* ���� ������ջ�����д������
* ����ֵ �� ��
* ����   �� д���4���ֽ�
*/
void   Mode1_ReceiverBuf_Write(unsigned char data[4] )
{
	//��β׷��
		memcpy((char*) &Mode1ReceiverBuf.Buf[Mode1ReceiverBuf.tail][0] , (char*)data , 4);

		if(++Mode1ReceiverBuf.tail>= RF_RECEIVE_SIZE)
		{
					Mode1ReceiverBuf.tail=0;		//β�ڵ�ƫ��
		}

		//β׷��ͷ
		if(Mode1ReceiverBuf.tail ==  Mode1ReceiverBuf.head)
		{
			   if(++Mode1ReceiverBuf.head >= RF_RECEIVE_SIZE)
				 {
							Mode1ReceiverBuf.head=0;
				 }
		}
}

/*
* ���� ������ջ��������
* ����ֵ �� 1 �յ�  ��  0���سɹ�
* ����   �� ��֤��4���ֽڵĵ�ַ
*/
unsigned char   Mode1_ReceiverBuf_Read(unsigned char *pData)
{
		if(Mode1ReceiverBuf.tail ==  Mode1ReceiverBuf.head)
		{
			  return 0;			//����0  �յ�
		}
		else
		{
				memcpy((char *)pData , (char *)&Mode1ReceiverBuf.Buf[Mode1ReceiverBuf.head][0]  , 4);
				if(++Mode1ReceiverBuf.head >= RF_RECEIVE_SIZE)
				{
					Mode1ReceiverBuf.head=0;
				}
				return 4;		// ���ݳ���
		}
}

void   Mode1_ReceiverBufClear()
{
	Mode1ReceiverBuf.tail =  Mode1ReceiverBuf.head;
}






void   SI24R1_SendBuf_Write(unsigned char *data ,unsigned char txid)
{
	  if( (data[0]== FLOOD_LED) && (data[1]!= color_no) && (data[2]!= 0)) //���ȵ���
		{
//			  if(sysCfg.bright==1) data[2]=10;
//			  else if(sysCfg.bright==2) data[2]=15;
//			  else
				  data[2]=20;
		}
		data[3] = txid;
	  memcpy((char*) &RF_SendBuf.Buf[RF_SendBuf.tail][0] , (char*)data , 4);
	  RF_SendBuf.txID[RF_SendBuf.tail] = txid;
		if(++RF_SendBuf.tail>= RF_SEND_SIZE)
		{
					RF_SendBuf.tail=0;		//β�ڵ�ƫ��
		}
		//β׷��ͷ
		if(RF_SendBuf.tail ==  RF_SendBuf.head)
		{
			   if(++RF_SendBuf.head >= RF_SEND_SIZE)
				 {
							RF_SendBuf.head=0;
				 }
		}
}

/*
* ���� �����ͻ��������
* ����ֵ �� 0 �յ�  ��  1���سɹ�
* ����   �� ��4���ֽڵĵ�ַ
*/
unsigned char   SI24R1_SendBuf_Read(unsigned char *pData , unsigned  char *txid)
{
		if(RF_SendBuf.tail ==  RF_SendBuf.head)
		{
			  return 0;			//����1  �յ�
		}
		else
		{
				//strncpy((char *)pData , (char *)&RF_SendBuf.Buf[RF_SendBuf.head][0]  , 4);
				memcpy((char *)pData , (char *)&RF_SendBuf.Buf[RF_SendBuf.head][0]  , 4);
			  *txid=RF_SendBuf.txID[RF_SendBuf.head];


				if(++RF_SendBuf.head >= RF_SEND_SIZE)
				{
					RF_SendBuf.head=0;
				}
				return 1;
		}
}

void   SI24R1_SendBufClear(void)
{
		RF_SendBuf.tail =  RF_SendBuf.head=0;
}


void   conMcu(u8 color,u8 light)
{
	LED_All_color(color);

}

void RadioRF(void)
{
	static u8 RadioCount50Ms=0;
	static u8 canbuf[8],i;
 
	if(++RadioCount50Ms==40)  //2.5s �㲥һ������  
	{
			RadioCount50Ms=0;
			canbuf[0]=FLOOD_RADIO1;			  
			for(i=1;i<=3;i++)  //ѹ������1-6
			{
				canbuf[i]=ballSta.floodSta[i*2-1]*16+ballSta.floodSta[i*2];
			}			
				SI24R1Mode=1;									//���ж�
				//NRF24L01_Init();	 
				NRF24L01_TX_Mode();
				NRF24L01_TxPacket(canbuf,CAN_BASE_ID);
				NRF24L01_RX_Mode();
				SI24R1Mode=0;												//���ж�
	}		
}



							
//????  50ms
void   SI24R1_task(void )
{
	u8 canBuf[8],i , sendCount;
	u8 txID,res,sendFailFlag=1;

	SI24R1Mode=1;	
	res=NRF24L01_RxPacket(canBuf);  //��ȡ���յ�����
	if(res)
	{
			  SI24R1_RecDeal(canBuf);
	}
	SI24R1Mode=0;	

	res=SI24R1_SendBuf_Read(canBuf,&txID); //��ȡ���ͻ���
 	 if(res)
 	 { 
 
	  sendCount = 3;
 
		SI24R1Mode=1;									//���ж�
 		//NRF24L01_Init();	 
 		NRF24L01_TX_Mode();
 		for(i=0;i<sendCount;i++)
 		{
			if( NRF24L01_TxPacket(canBuf,txID))
			{
			   delay_us(500);
			   if( NRF24L01_TxPacket(canBuf,txID))
				{
				  NRF24L01_TxPacket(canBuf,txID);
					i=0;
					sendFailFlag=0;  //????
					if(canBuf[0]==FLOOD_LED && canBuf[1]!=0x00) //???????????
					{
						//INFO("succ id=%d\n",txID);
						 sendFailCount[txID]=0;	//????????
					}
					break;
				}
			}
			delay_us(1000);
 		}


		
 		if(sendFailFlag == 1)
 		{
 			 sendFailCount[txID]++;
 			 if(sendFailCount[txID] >= 3) //???????? ????????
 			 {
 				 sendFailCount[txID]=0;
 				 ballSta.floodSta[txID] =0;
 			 }
 			 else
 			 {
 				 SI24R1_SendBuf_Write(canBuf,txID); //????
 			 }
 		}
		
		
 		NRF24L01_RX_Mode( );
		SI24R1Mode=0;												//���ж�
 	 }
	 
	 RadioRF();
}

static void SI24R1_IRQ_Intr_Read(void *arg)	//2.4G??????
{
//	u32 gpio_status = GPIO_REG_READ(GPIO_STATUS_ADDRESS);
//	u8 res=0;
//	u8 canBuf[8];
//	ETS_GPIO_INTR_DISABLE();//
//	if(gpio_status & BIT(4))//IRQ
//	{
//		res=SI24R1_RxPacket(canBuf);
//		if(res)
//		{
//			INFO("intrec:%d,%d,%d,%d\n",canBuf[0],canBuf[1],canBuf[2],canBuf[3]);
//			Mode1_ReceiverBuf_Write(canBuf);
//		}
//	}
//	GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, gpio_status);
//	ETS_GPIO_INTR_ENABLE();
}


 
/*******************************************************************************
* �� �� ��         : exti_init
* ��������		   : �ⲿ�ж�2�˿ڳ�ʼ������	   
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void IRQ_intr_init(void)  //�ⲿ�жϳ�ʼ��
{
	GPIO_InitTypeDef GPIO_InitStructure;

	EXTI_InitTypeDef EXTI_InitStructure;

	NVIC_InitTypeDef NVIC_InitStructure;

	/* ����GPIOʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	 
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);//ѡ��GPIO�ܽ������ⲿ�ж���·
	//�˴�һ��Ҫ��ס���˿ڹܽż����ж��ⲿ��·
	/* �����ⲿ�жϵ�ģʽ */ 
	EXTI_InitStructure.EXTI_Line=EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure); 
	
	/* ����NVIC���� */	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		 
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn; 	//��EXTI0��ȫ���ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռ���ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		  //��Ӧ���ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 		  //ʹ��
	NVIC_Init(&NVIC_InitStructure); 		
}
void EXTI0_IRQHandler()	   //�ⲿ�ж�0�жϺ���
{
	u8 canBuf[8],res;
	if(EXTI_GetITStatus(EXTI_Line0)==SET)
	{
		EXTI_ClearITPendingBit(EXTI_Line0);//���EXTI��·����λ	
		if(SI24R1Mode==0) //����ģʽ
		{
			res=NRF24L01_RxPacket(canBuf);
			if(res)
			{
				SI24R1_RecDeal(canBuf);			
			}		
		}	
	}	
}

void SI24R1_RecDeal(u8 * buf)
{
		LED_LightTime50Ms+=2;
	  if(buf[0]<=MAX_BALL)
		{
			//UartReturn_Updata(buf[0]);			
			ballSta.floodSta[buf[0]]=buf[1];	
 			Mode1_ReceiverBuf_Write(buf);
		}
		else if(buf[0] == 0xaa)  //���� ��Ϣ  0xaa id bat 0xaa 
		{
				 ballSta.bat[ buf[1] ]= buf[2];
			   ballSta.Online[ buf[1] ] =1;  //�豸����
				 ballSta.noRecTime[ buf[1] ] =0;  //ˢ��ʱ��
		}	
}

void Ball_OnlineDeal(void)
{
		static u8 Count50Ms=0,i;
	  
		if(++Count50Ms==20)  //1s  
		{
			Count50Ms=0;
			for(i=1;i<=MAX_BALL;i++) 
			{
				ballSta.noRecTime[i]++;
				if(ballSta.noRecTime[i]>5)  //����5s ���ź�����
				{
						ballSta.Online[i]=0;
					  
				}
			}
			
		}
}


