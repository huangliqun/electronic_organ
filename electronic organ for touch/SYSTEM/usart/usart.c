#include "sys.h"
#include "usart.h"	
#include "led.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos 使用	  
#endif
 
 
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
_ttywrch(int ch)
{
ch = ch;
}

//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
    USART1->DR = (u8) ch;      
	return ch;
}

//#endif 

/*使用microLib的方法*/
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

#if EN_USART1_RX   //如果使能了接收
 
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
 
u16 USART_RX_STA=0;       //接收状态标记	  

 
 
void uart2_init(u32 bound){
    //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	   //使能USART2，GPIOA时钟
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);

	
	//USART2_TX   GPIOA.2
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 								//PA.2
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;						//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);										//初始化GPIOA.2
   
  //USART2_RX	  GPIOA.3
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;                 //PA.3
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;			//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);										//初始化GPIOA.3

  //Usart2 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;                      //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;												    //子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;														    //IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);																			          //根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;																	   //串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;										 //字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;												 //一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;														 //无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;								 //收发模式
  USART_Init(USART2, &USART_InitStructure); 																		 //初始化串口2
	USART_ITConfig(USART2,USART_IT_RXNE  ,ENABLE);																	 //开启USART2空闲终端(DMA)
	
  USART_Cmd(USART2, ENABLE);                    																//使能串口2 	
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


void USART2_IRQHandler(void)	//串口2中断函数
{
	static u8 k;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
			USART2_RX_BUF[USART2_RX_LEN]=USART_ReceiveData(USART2);
		  USART2_RX_LEN++;
		  if(USART2_RX_LEN>31) USART2_RX_LEN=0;
	    USART2_RX_STA=1; 
	}
	
//  if(USART_GetITStatus(USART2,USART_IT_RXNE)!=Bit_RESET)//检查指定的USART中断发生与否
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
void USART2_IRQHandler(void)                	//串口2中断服务程序
	{
	u8 Res;
#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntEnter();    
#endif
		
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0xEF 0xFF结尾)
		{
				Res =USART_ReceiveData(USART2);	//读取接收到的数据
				Light_LED(Res);
     } 
		
#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntExit();  											 
#endif
} */
#endif	



void uart1_disable(void )
{
	GPIO_InitTypeDef GPIO_InitStructure;	//声明一个结构体变量，用来初始化GPIO
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
 
	/*  配置GPIO的模式和IO口 */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9|GPIO_Pin_10;//TX			   //串口输出PA9
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;	    //复用推挽输出
	GPIO_Init(GPIOA,&GPIO_InitStructure);  /* 初始化串口输入IO */
 
}


void usart1_init(uint32_t bote)
{
	GPIO_InitTypeDef GPIO_InitStructure;	//声明一个结构体变量，用来初始化GPIO

	USART_InitTypeDef  USART_InitStructure;	  //串口结构体定义

	NVIC_InitTypeDef NVIC_InitStructure;//中断结构体定义
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	 //打开时钟
	
	/*  配置GPIO的模式和IO口 */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;//TX			   //串口输出PA9
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;	    //复用推挽输出
	GPIO_Init(GPIOA,&GPIO_InitStructure);  /* 初始化串口输入IO */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;//RX			 //串口输入PA10
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;		  //模拟输入
	GPIO_Init(GPIOA,&GPIO_InitStructure); /* 初始化GPIO */


	USART_InitStructure.USART_BaudRate=bote;   //波特率设置为9600	//波特率
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;		//数据长8位
	USART_InitStructure.USART_StopBits=USART_StopBits_1;			//1位停止位
	USART_InitStructure.USART_Parity=USART_Parity_No;				//无效验
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None; //失能硬件流
	USART_InitStructure.USART_Mode=USART_Mode_Tx | USART_Mode_Rx;	 //开启发送和接受模式
	USART_Init(USART1,&USART_InitStructure);	/* 初始化USART1 */
   USART_Cmd(USART1, ENABLE);		   /* 使能USART1 */
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//使能或者失能指定的USART中断 接收中断
	USART_ClearFlag(USART1,USART_FLAG_TC);//清除USARTx的待处理标志位


/* 设置NVIC参数 */

	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);    
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; 	   //打开USART1的全局中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 	 //抢占优先级为0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 			//响应优先级为0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 			 //使能
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



void USART1_IRQHandler(void)	//串口1中断函数
{
		u8 Res;
 	//USART_ClearFlag(USART1,USART_IT_RXNE);
	if(USART_GetITStatus(USART1,USART_IT_RXNE)!=Bit_RESET)//检查指定的USART中断发生与否
	{
		Res =USART_ReceiveData(USART1);//(USART1->DR);	//读取接收到的数据
 		
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


