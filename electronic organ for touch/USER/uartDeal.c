
#include "main.h"
 
#include "string.h "
#include "stdlib.h"
ReceiverBuf_t ReceiverBuf;

//向接收缓冲区写入一个字节
void ReceiverBuf_Write(unsigned char data )
{
	//从尾追加
	  ReceiverBuf.Buf[ReceiverBuf.writeIndex]=data;	
		if(++ReceiverBuf.writeIndex>= RECEIVE_SIZE)
		{
					ReceiverBuf.writeIndex=0;		//尾节点偏移
		}	
		//尾追到头
		if(ReceiverBuf.writeIndex ==  ReceiverBuf.readIndex)
		{
			   if(++ReceiverBuf.readIndex >= RECEIVE_SIZE)
				 {
							ReceiverBuf.readIndex=0;
				 }
		}
}

//#define CMD_BEGIN     1 		//开始
//#define CMD_OVER      2			//结束
//#define CMD_PACK 			3			//发送课程包
//#define CMD_SCHEDULE  4			//查询课程进度
//#define CMD_STATE 		5			//查询设备状态
//#define CMD_UPDATA 		6			//球状态上传
//#define CMD_CONTROL   7			//直接控制球
void UartReturn_Begin(u8 * receverData)
{
		u8  sendData[7];
		sendData[0]=0x55;
		sendData[1]=0xaa;
		sendData[2]=0x01;
		sendData[3]=0x00;
		sendData[4]=0x00;
		sendData[5]=0xef;
		sendData[6]=0xff;
		USART1_printf(7,sendData);
		GameRunTaskFlag=1;
		GameRunFlag1=0;
		GameRunFlag2=0;
		GameRunFlag3=0;
	playCount=receverData[0];
	//	playCount=1;
}

void UartReturn_Over(void)
{
		u8  sendData[7];
		sendData[0]=0x55;
		sendData[1]=0xaa;
		sendData[2]=0x02;
		sendData[3]=0x00;
		sendData[4]=0x01;
		sendData[5]=0xef;
		sendData[6]=0xff;
		USART1_printf(7,sendData);
		GameRunTaskFlag=0;
		GameRunFlag1=0;
		GameRunFlag2=0;
		GameRunFlag3=0;
}
void UartReturn_Pack(u8 * receverData , u8 dataLen)
{
		u8  sendData[7];
		sendData[0]=0x55;
		sendData[1]=0xaa;
		sendData[2]=0x03;
		sendData[3]=0x00;
		sendData[4]=0x02;
		sendData[5]=0xef;
		sendData[6]=0xff;
	
		ApiPacket( receverData ,   dataLen);
	
		USART1_printf(7,sendData);
}

void UartReturn_Schedule(u8 * receverData)
{
		u8  sendData[12];
		sendData[0]=0x55;
		sendData[1]=0xaa;
		sendData[2]=0x04;
		sendData[3]=0x05;
	
	if(receverData[0]==1)
	{
	  sendData[4]=gamePack1.Timer/256;  //time
		sendData[5]=gamePack1.Timer%256;  //time
		sendData[6]=gamePack1.finNumber/256;  //group
		sendData[7]=gamePack1.finNumber%256;  //group
		sendData[8]=gamePack1.nowStep;  //step
	}
	else if(receverData[0]==2)
	{
		  sendData[4]=gamePack2.Timer/256;  //time
		sendData[5]=gamePack2.Timer%256;  //time
		sendData[6]=gamePack2.finNumber/256;  //group
		sendData[7]=gamePack2.finNumber%256;  //group
		sendData[8]=gamePack2.nowStep;  //step
	}
	else
	{
		  sendData[4]=gamePack3.Timer/256;  //time
		sendData[5]=gamePack3.Timer%256;  //time
		sendData[6]=gamePack3.finNumber/256;  //group
		sendData[7]=gamePack3.finNumber%256;  //group
		sendData[8]=gamePack3.nowStep;  //step
	}

	
	
		sendData[9]=0x02;  //xor
		sendData[10]=0xef;
		sendData[11]=0xff;
		USART1_printf(12,sendData);
}

void UartReturn_State(void)
{
		u8  sendData[19],i;
	u8 dataXor=0;
		sendData[0]=0x55;
		sendData[1]=0xaa;
		sendData[2]=0x05;
		sendData[3]=0x0C;
	
	for(i=4;i<10;i++)
	{
			sendData[i]=ballSta.bat[i-3];  //电量
	}
  for(i=10;i<16;i++)
	{
			sendData[i]=ballSta.Online[i-9];  //在线状态
	}
	for(i=0;i<16;i++)
	{
		dataXor+=sendData[i];
	}
	 
		sendData[16]=dataXor; //xor
		sendData[17]=0xef;
		sendData[18]=0xff;
		USART1_printf(19,sendData);
}


//flood  是哪个球  
void UartReturn_Updata(u8 flood)
{
		u8  sendData[9] ;
		sendData[0]=0x55;
		sendData[1]=0xaa;
		sendData[2]=0x06;
		sendData[3]=0x02;
	
		sendData[4]=flood; //哪个球被踩下？
	  if(ballSta.floodSta[flood])
		{
			sendData[5]=1; //是否是当前步骤
		}
		else
		{
			sendData[5]=0; //是否是当前步骤
			return ;
		}
		sendData[6]=0x02; //xor
		sendData[7]=0xef;
		sendData[8]=0xff;
		USART1_printf(9,sendData);
}

void UartReturn_Control(u8 * receverData)
{
		u8 canbuf[8];

		u8 sendData[7],i;
		sendData[0]=0x55;
		sendData[1]=0xaa;
		sendData[2]=0x07;
		sendData[3]=0x00;
		sendData[4]=0x02;
		sendData[5]=0xef;
		sendData[6]=0xff;
		USART1_printf(7,sendData);
	
		GameRunTaskFlag=0;
		GameRunFlag1=0;
		GameRunFlag2=0;
		GameRunFlag3=0;
	  for(i=0;i<6;i++)
		{
			 if(receverData[i]<9)
			 {
					canbuf[0] = FLOOD_LED;
					canbuf[1]	= receverData[i];
					canbuf[2]	= 50;
					SI24R1_SendBuf_Write(canbuf,i+1);			
			 }
		}
}

void 	UartReturn_shutDownBall(void)  //直接关所有球
{		 
		u8 canbuf[8],i;
	u8  sendData[7];
		canbuf[0]=FLOOD_SHUTDOWN;	
		canbuf[1]=FLOOD_SHUTDOWN;	
		canbuf[2]=FLOOD_SHUTDOWN;	
		canbuf[3]=FLOOD_SHUTDOWN;
	
		SI24R1_SendBuf_Write(canbuf,CAN_BASE_ID);
	
		
		sendData[0]=0x55;
		sendData[1]=0xaa;
		sendData[2]=0x08;
		sendData[3]=0x00;
		sendData[4]=0x07;
		sendData[5]=0xef;
		sendData[6]=0xff;
		USART1_printf(7,sendData);
	
}	

void 	UartReturn_setSensor(u8 * receverData)  //设置传感器
{		 
 
	 	u8 dataXor=0,i;
		u8  sendData[8];
		sendData[0]=0x55;
		sendData[1]=0xaa;
		sendData[2]=0x09;
		sendData[3]=0x01;
		sendData[4]=*receverData;
	  
	  for(i=0;i<5;i++) dataXor+=sendData[i];
	
	 
		sendData[5]=dataXor;
		sendData[6]=0xef;
		sendData[7]=0xff;
		USART1_printf(8,sendData);
	
 }

unsigned char changBuf(u16 index)
{
		if(index>= RECEIVE_SIZE)
		{
			index=index-RECEIVE_SIZE;
		}
		return ReceiverBuf.Buf[index];
}

void UartBufDeal(void)
{
	 u16 beginIndex=ReceiverBuf.readIndex,i,j;
	 u8 dataLen,cmd,dataXor; 
	 u8 *recData;
	 if(ReceiverBuf.writeIndex>ReceiverBuf.readIndex)
	 {
			for(i=ReceiverBuf.readIndex;i<ReceiverBuf.writeIndex;i++)
			{
					if((ReceiverBuf.Buf[i]==0x55) && (ReceiverBuf.Buf[i+1]==0xAA)) //开始帧
					{
							dataLen=ReceiverBuf.Buf[i+3];
							LED_LightTime50Ms+=2;
							if((ReceiverBuf.Buf[i+5+dataLen]==0xef )&&(ReceiverBuf.Buf[i+6+dataLen]==0xff )) //结束帧
							{
									cmd=ReceiverBuf.Buf[i+2];
									recData=malloc(dataLen);	
			
									for(j=i+4;j<i+4+dataLen;j++)
									{
										recData[j-(i+4)]=changBuf(j);
									}
								
								  if(cmd==CMD_BEGIN)
									{					
											UartReturn_Begin(recData);				
									}
									else if(cmd==CMD_OVER)
									{
											UartReturn_Over();
									}
									else if(cmd==CMD_PACK)
									{
										  UartReturn_Pack(recData,dataLen);
									}
									else if(cmd==CMD_SCHEDULE)
									{
											UartReturn_Schedule(recData);
									}
									else if(cmd==CMD_STATE)
									{
											UartReturn_State();
									}
									else if(cmd==CMD_UPDATA)
									{
											UartReturn_Updata(1);
									}
									else if(cmd==CMD_CONTROL)
									{
											UartReturn_Control(recData);
									}	
								  else if(cmd==CMD_CLOSEALL)
									{
											UartReturn_shutDownBall();
									}	
								 else if(cmd==CMD_SETSENSOR)
									{
											UartReturn_setSensor(recData);
									}	
									memset(&ReceiverBuf.Buf[i],0,7+dataLen); //清空已经接收到的数据
									ReceiverBuf.readIndex=i+6+dataLen;
			 
								  free(recData);
							}
					}
			}
	 }
	 else if(ReceiverBuf.writeIndex<ReceiverBuf.readIndex)
	 {
			for(i=ReceiverBuf.readIndex;i<ReceiverBuf.writeIndex+RECEIVE_SIZE;i++)
			{
					if((changBuf(i)==0x55) && (changBuf(i+1)==0xAA)) //开始帧
					{
							dataLen=changBuf(i+3);
							
							if((changBuf(i+5+dataLen)==0xef )&&(changBuf(i+6+dataLen)==0xff )) //结束帧
							{
									cmd=changBuf(i+2);
									recData=malloc(dataLen);	
								
								  for(j=i+4;j<i+4+dataLen;j++)
									{
										recData[j-(i+4)]=changBuf(j);
									}
								  						
								  if(cmd==CMD_BEGIN)
									{					
											UartReturn_Begin(recData);				
									}
									else if(cmd==CMD_OVER)
									{
											UartReturn_Over();
									}
									else if(cmd==CMD_PACK)
									{
										  UartReturn_Pack(recData,dataLen);
									}
									else if(cmd==CMD_SCHEDULE)
									{
											UartReturn_Schedule(recData);
									}
									else if(cmd==CMD_STATE)
									{
											UartReturn_State();
									}
									else if(cmd==CMD_UPDATA)
									{
											UartReturn_Updata(1);
									}
									else if(cmd==CMD_CONTROL)
									{
											UartReturn_Control(recData);
									}
									
							    for(j=i;j<i+7+dataLen;j++) //清空已经接收到的数据
									{
										if(j<RECEIVE_SIZE)
										{
											ReceiverBuf.Buf[j]=0;
										}			
										else
										{
											ReceiverBuf.Buf[j-RECEIVE_SIZE]=0;
										}
									}
								 
									ReceiverBuf.readIndex=i+6+dataLen;
									if(ReceiverBuf.readIndex>RECEIVE_SIZE)
									{
											ReceiverBuf.readIndex=ReceiverBuf.readIndex-RECEIVE_SIZE;
									}
								  free(recData);
							}
					}
			}	 
	 }
		
}


