
#include "main.h"
#include "string.h"
#include "stdlib.h"
#include "24l01Buf.h"
#include "gamePackDeal.h"
RF_ReceiverBuf_t Mode1ReceiverBuf,Mode2ReceiverBuf,Mode3ReceiverBuf;
RF_SendBuf_t RF_SendBuf;
u8 sendFailCount[MAX_BALL+1];
u8 SI24R1Mode; //=0接收模式   1发送模式
/*
* 功能 ：向接收缓存中写入数据
* 返回值 ： 无
* 输入   ： 写入的4个字节
*/
void   Mode1_ReceiverBuf_Write(unsigned char data[4] )
{
	//从尾追加
		memcpy((char*) &Mode1ReceiverBuf.Buf[Mode1ReceiverBuf.tail][0] , (char*)data , 4);

		if(++Mode1ReceiverBuf.tail>= RF_RECEIVE_SIZE)
		{
					Mode1ReceiverBuf.tail=0;		//尾节点偏移
		}

		//尾追到头
		if(Mode1ReceiverBuf.tail ==  Mode1ReceiverBuf.head)
		{
			   if(++Mode1ReceiverBuf.head >= RF_RECEIVE_SIZE)
				 {
							Mode1ReceiverBuf.head=0;
				 }
		}
}

/*
* 功能 ：向接收缓存读数据
* 返回值 ： 1 空的  ，  0返回成功
* 输入   ： 保证读4个字节的地址
*/
unsigned char   Mode1_ReceiverBuf_Read(unsigned char *pData)
{
		if(Mode1ReceiverBuf.tail ==  Mode1ReceiverBuf.head)
		{
			  return 0;			//返回0  空的
		}
		else
		{
				memcpy((char *)pData , (char *)&Mode1ReceiverBuf.Buf[Mode1ReceiverBuf.head][0]  , 4);
				if(++Mode1ReceiverBuf.head >= RF_RECEIVE_SIZE)
				{
					Mode1ReceiverBuf.head=0;
				}
				return 4;		// 数据长度
		}
}

void   Mode1_ReceiverBufClear()
{
	Mode1ReceiverBuf.tail =  Mode1ReceiverBuf.head;
}






void   SI24R1_SendBuf_Write(unsigned char *data ,unsigned char txid)
{
	  if( (data[0]== FLOOD_LED) && (data[1]!= color_no) && (data[2]!= 0)) //亮度调整
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
					RF_SendBuf.tail=0;		//尾节点偏移
		}
		//尾追到头
		if(RF_SendBuf.tail ==  RF_SendBuf.head)
		{
			   if(++RF_SendBuf.head >= RF_SEND_SIZE)
				 {
							RF_SendBuf.head=0;
				 }
		}
}

/*
* 功能 ：向发送缓存读数据
* 返回值 ： 0 空的  ，  1返回成功
* 输入   ： 读4个字节的地址
*/
unsigned char   SI24R1_SendBuf_Read(unsigned char *pData , unsigned  char *txid)
{
		if(RF_SendBuf.tail ==  RF_SendBuf.head)
		{
			  return 0;			//返回1  空的
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
 
	if(++RadioCount50Ms==40)  //2.5s 广播一次数据  
	{
			RadioCount50Ms=0;
			canbuf[0]=FLOOD_RADIO1;			  
			for(i=1;i<=3;i++)  //压缩后发送1-6
			{
				canbuf[i]=ballSta.floodSta[i*2-1]*16+ballSta.floodSta[i*2];
			}			
				SI24R1Mode=1;									//关中断
				//NRF24L01_Init();	 
				NRF24L01_TX_Mode();
				NRF24L01_TxPacket(canbuf,CAN_BASE_ID);
				NRF24L01_RX_Mode();
				SI24R1Mode=0;												//开中断
	}		
}



							
//????  50ms
void   SI24R1_task(void )
{
	u8 canBuf[8],i , sendCount;
	u8 txID,res,sendFailFlag=1;

	SI24R1Mode=1;	
	res=NRF24L01_RxPacket(canBuf);  //读取接收的数据
	if(res)
	{
			  SI24R1_RecDeal(canBuf);
	}
	SI24R1Mode=0;	

	res=SI24R1_SendBuf_Read(canBuf,&txID); //读取发送缓存
 	 if(res)
 	 { 
 
	  sendCount = 3;
 
		SI24R1Mode=1;									//关中断
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
		SI24R1Mode=0;												//开中断
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
* 函 数 名         : exti_init
* 函数功能		   : 外部中断2端口初始化函数	   
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void IRQ_intr_init(void)  //外部中断初始化
{
	GPIO_InitTypeDef GPIO_InitStructure;

	EXTI_InitTypeDef EXTI_InitStructure;

	NVIC_InitTypeDef NVIC_InitStructure;

	/* 开启GPIO时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	 
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);//选择GPIO管脚用作外部中断线路
	//此处一定要记住给端口管脚加上中断外部线路
	/* 设置外部中断的模式 */ 
	EXTI_InitStructure.EXTI_Line=EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure); 
	
	/* 设置NVIC参数 */	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		 
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn; 	//打开EXTI0的全局中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //抢占优先级为0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		  //响应优先级为0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 		  //使能
	NVIC_Init(&NVIC_InitStructure); 		
}
void EXTI0_IRQHandler()	   //外部中断0中断函数
{
	u8 canBuf[8],res;
	if(EXTI_GetITStatus(EXTI_Line0)==SET)
	{
		EXTI_ClearITPendingBit(EXTI_Line0);//清除EXTI线路挂起位	
		if(SI24R1Mode==0) //接收模式
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
		else if(buf[0] == 0xaa)  //电量 信息  0xaa id bat 0xaa 
		{
				 ballSta.bat[ buf[1] ]= buf[2];
			   ballSta.Online[ buf[1] ] =1;  //设备在线
				 ballSta.noRecTime[ buf[1] ] =0;  //刷新时间
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
				if(ballSta.noRecTime[i]>5)  //连续5s 无信号离线
				{
						ballSta.Online[i]=0;
					  
				}
			}
			
		}
}


