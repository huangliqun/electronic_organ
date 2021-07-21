#include "24l01.h"
#include "delay.h"
#include "spi.h"
#include "usart.h"
 
 
unsigned char   ADDRESS_MAIN[TX_ADR_WIDTH] = {0Xff,0x01,0x07,0x0E,0x02};
unsigned char   ADDRESS_SLAVE_ALL[TX_ADR_WIDTH] = {0Xf0,0x01,0x07,0x0E,0x02};
unsigned char   ADDRESS_SLAVE_ONE[TX_ADR_WIDTH] = {0x01,0x01,0x07,0x0E,0x02};

 

//��ʼ��24L01��IO��

/*
	PC4  IQR
	PC5  CE
	PA4  CSN
*/
void NRF24L01_Init(void)
{ 	
	GPIO_InitTypeDef GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOA, ENABLE);//ʹ��PB,G�˿�ʱ��
    
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_11;				//CE
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);						 
  
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;     //IRQ
 	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
 	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4;				//CSN
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);	

	 
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);	
		 
  SPI2_Init();    		//��ʼ��SPI	 
 
	SPI_Cmd(SPI1, DISABLE); // SPI���費ʹ��

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;											  //SPI����
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;									  //���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;								          //ʱ�����յ�
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;								        //���ݲ����ڵ�1��ʱ����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;													  //NSS�ź����������
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;	//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ16
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;									//���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;											      //CRCֵ����Ķ���ʽ
	SPI_Init(SPI1, &SPI_InitStructure);  																//����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
 
	SPI_Cmd(SPI1, ENABLE); //ʹ��SPI����
			 
	NRF24L01_CE=0; 			//ʹ��24L01
	NRF24L01_CSN=1;			//SPIƬѡȡ��  
	 		 	 
}
//���24L01�Ƿ����
//����ֵ:0���ɹ�;1��ʧ��	
u8 NRF24L01_Check(void)
{
	u8 buf[5]={0XA5,0XA5,0XA5,0XA5,0XA5};
	u8 i;
	SPI2_SetSpeed(SPI_BaudRatePrescaler_4); //spi�ٶ�Ϊ9Mhz��24L01�����SPIʱ��Ϊ10Mhz��   	 
	NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,buf,5);//д��5���ֽڵĵ�ַ.	
	NRF24L01_Read_Buf(TX_ADDR,buf,5); //����д��ĵ�ַ  
	for(i=0;i<5;i++)if(buf[i]!=0XA5)break;	 							   
	if(i!=5)return 1;//���24L01����	
	return 0;		 //��⵽24L01
}	 	 
//SPIд�Ĵ���
//reg:ָ���Ĵ�����ַ
//value:д���ֵ
u8 NRF24L01_Write_Reg(u8 reg,u8 value)
{
	u8 status;	
   	NRF24L01_CSN=0;                 //ʹ��SPI����
  	status =SPI2_ReadWriteByte(reg);//���ͼĴ����� 
  	SPI2_ReadWriteByte(value);      //д��Ĵ�����ֵ
  	NRF24L01_CSN=1;                 //��ֹSPI����	   
  	return(status);       			//����״ֵ̬
}
//��ȡSPI�Ĵ���ֵ
//reg:Ҫ���ļĴ���
u8 NRF24L01_Read_Reg(u8 reg)
{
	u8 reg_val;	    
 	NRF24L01_CSN = 0;          //ʹ��SPI����	
  	SPI2_ReadWriteByte(reg);   //���ͼĴ�����
  	reg_val=SPI2_ReadWriteByte(0XFF);//��ȡ�Ĵ�������
  	NRF24L01_CSN = 1;          //��ֹSPI����		    
  	return(reg_val);           //����״ֵ̬
}	
//��ָ��λ�ö���ָ�����ȵ�����
//reg:�Ĵ���(λ��)
//*pBuf:����ָ��
//len:���ݳ���
//����ֵ,�˴ζ�����״̬�Ĵ���ֵ 
u8 NRF24L01_Read_Buf(u8 reg,u8 *pBuf,u8 len)
{
	u8 status,u8_ctr;	       
  	NRF24L01_CSN = 0;           //ʹ��SPI����
  	status=SPI2_ReadWriteByte(reg);//���ͼĴ���ֵ(λ��),����ȡ״ֵ̬   	   
 	for(u8_ctr=0;u8_ctr<len;u8_ctr++)pBuf[u8_ctr]=SPI2_ReadWriteByte(0XFF);//��������
  	NRF24L01_CSN=1;       //�ر�SPI����
  	return status;        //���ض�����״ֵ̬
}
//��ָ��λ��дָ�����ȵ�����
//reg:�Ĵ���(λ��)
//*pBuf:����ָ��
//len:���ݳ���
//����ֵ,�˴ζ�����״̬�Ĵ���ֵ
u8 NRF24L01_Write_Buf(u8 reg, u8 *pBuf, u8 len)
{
	u8 status,u8_ctr;	    
 	NRF24L01_CSN = 0;          //ʹ��SPI����
  	status = SPI2_ReadWriteByte(reg);//���ͼĴ���ֵ(λ��),����ȡ״ֵ̬
  	for(u8_ctr=0; u8_ctr<len; u8_ctr++)SPI2_ReadWriteByte(*pBuf++); //д������	 
  	NRF24L01_CSN = 1;       //�ر�SPI����
  	return status;          //���ض�����״ֵ̬
}				   

u8  SI24R1_senddata(u8 *txbuf, u8 driver_flag)
{
  u8 i;
	NRF24L01_TX_Mode();	
	for(i=0;i<20;i++)
	{
		if(NRF24L01_TxPacket(txbuf,driver_flag)== TX_OK)
		{
			delay_ms(5);
			NRF24L01_TxPacket(txbuf,driver_flag);
			 return 1;
		}
		else 
		{
			delay_ms(5);
		}
	}	
	return 0;
}

				    
//�ú�����ʼ��NRF24L01��RXģʽ
//����RX��ַ,дRX���ݿ��,ѡ��RFƵ��,�����ʺ�LNA HCURR
//��CE��ߺ�,������RXģʽ,�����Խ���������		   
void NRF24L01_RX_Mode()
{
	  NRF24L01_Init();
	
	  NRF24L01_CE=0;	
		SPI2_SetSpeed(SPI_BaudRatePrescaler_8); //spi�ٶ�Ϊ9Mhz��24L01�����SPIʱ��Ϊ10Mhz�� 
  	NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(u8*)ADDRESS_MAIN,TX_ADR_WIDTH);//дRX�ڵ��ַ
	  
  	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,0x03);    //ʹ��ͨ��0���Զ�Ӧ��    
  	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x03);//ʹ��ͨ��0�Ľ��յ�ַ  
	
		NRF24L01_Write_Reg(NRF_WRITE_REG+SETUP_AW,0x03);
		NRF24L01_Write_Reg(NRF_WRITE_REG+FEATURE,0x04);
		NRF24L01_Write_Reg(NRF_WRITE_REG+DYNPD,0x3f);
	
  	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,SI24R1_REC_CHA);	     //����RFͨ��Ƶ��
	
  	NRF24L01_Write_Reg(NRF_WRITE_REG+RX_PW_P0,TX_PLOAD_WIDTH);//ѡ��ͨ��0����Ч���ݿ�� 	    
	 
		NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP,RF_SETUP_SET);//����TX�������,0db����,2Mbps,���������濪��   
  	NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG, 0x0f);//���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ 
  	
		NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS, 0xff);
		NRF24L01_Write_Reg(FLUSH_TX, 0xff);
		NRF24L01_Write_Reg(FLUSH_RX, 0xff);
			
		NRF24L01_CE = 1; //CEΪ��,�������ģʽ 
}

//�ú�����ʼ��NRF24L01��TXģʽ
//����TX��ַ,дTX���ݿ��,����RX�Զ�Ӧ��ĵ�ַ,���TX��������,ѡ��RFƵ��,�����ʺ�LNA HCURR
//PWR_UP,CRCʹ��
//��CE��ߺ�,������RXģʽ,�����Խ���������		   
//CEΪ�ߴ���10us,����������.	 
void NRF24L01_TX_Mode(void)
{														 
		NRF24L01_CE=0;	    
  //	NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,(u8*)TX_ADDRESS,TX_ADR_WIDTH);//дTX�ڵ��ַ 
  //	NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH); //����TX�ڵ��ַ,��ҪΪ��ʹ��ACK	  
		SPI2_SetSpeed(SPI_BaudRatePrescaler_8); //spi�ٶ�Ϊ9Mhz��24L01�����SPIʱ��Ϊ10Mhz�� 

		NRF24L01_Write_Reg(NRF_WRITE_REG+SETUP_AW,0x03);  
		NRF24L01_Write_Reg(NRF_WRITE_REG+FEATURE,0x04);  
		NRF24L01_Write_Reg(NRF_WRITE_REG+DYNPD,0x01);  
	
  	NRF24L01_Write_Reg(NRF_WRITE_REG + SETUP_RETR, 0x1a);     //ʹ��ͨ��(ȫ��)���Զ�Ӧ��    
  	NRF24L01_Write_Reg(NRF_WRITE_REG + RF_CH, SI24R1_SEND_CHA); //  
  	NRF24L01_Write_Reg(NRF_WRITE_REG + RF_SETUP, RF_SETUP_SET);//�����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10��
		NRF24L01_Write_Reg(NRF_WRITE_REG + CONFIG, 0x0e);
 
		NRF24L01_CE=1;//CEΪ��,10us����������
	  delay_ms(1);
		NRF24L01_CE=0;
}

//����NRF24L01����һ������
//txbuf:�����������׵�ַ
//����ֵ:0��������ɣ��������������
u8 NRF24L01_RxPacket(u8 *rxbuf)
{
		u8 sta,fifo_status;    							   
	  
	sta=NRF24L01_Read_Reg(STATUS);  //��ȡ״̬�Ĵ�����ֵ 
	fifo_status=NRF24L01_Read_Reg(NRF_FIFO_STATUS);  //
	NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,sta); //���TX_DS��MAX_RT�жϱ�־
	
	 if(fifo_status& 0x02)//rx fifo manle
	 {
			NRF24L01_Read_Buf(RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);//��ȡ����
			NRF24L01_Write_Reg(FLUSH_RX,0xff);//���RX FIFO�Ĵ��� 
			return 1;
	 }
		
	else if( sta==2)//���յ�����
	{
		NRF24L01_Read_Buf(RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);//��ȡ����
		NRF24L01_Write_Reg(FLUSH_RX,0xff);//���RX FIFO�Ĵ��� 
		return 1; 
	}
  else if((sta & RX_OK) )	
	{
		NRF24L01_Read_Buf(RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);//��ȡ����
		NRF24L01_Write_Reg(FLUSH_RX,0xff);//���RX FIFO�Ĵ��� 
		return 1; 
	}
	
	return 0;//û�յ��κ�����
}	

//����NRF24L01����һ������
//txbuf:�����������׵�ַ
//����ֵ:�������״��
u8 NRF24L01_TxPacket(u8 *txbuf,  u8 sendAddr)
{
 
	u8 state,flag=0,count_1ms=0; 
	NRF24L01_CE=0;	
	ADDRESS_SLAVE_ONE[0]=sendAddr;
	SPI2_SetSpeed(SPI_BaudRatePrescaler_8);//spi�ٶ�Ϊ9Mhz��24L01�����SPIʱ��Ϊ10Mhz��   
	NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,(u8*)ADDRESS_SLAVE_ONE,TX_ADR_WIDTH);//дTX�ڵ��ַ 
	NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(u8*)ADDRESS_SLAVE_ONE,TX_ADR_WIDTH);//дTX�ڵ��ַ
	
  NRF24L01_Write_Buf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);//д���ݵ�TX BUF  32���ֽ�
 	NRF24L01_CE=1;//��������	   
	
	while(NRF24L01_IRQ!=0)//�ȴ��������
	{
	  if(count_1ms++>=30)
		{
			break;
		}
		delay_ms(1);
	}
	state=NRF24L01_Read_Reg(STATUS);  //��ȡ״̬�Ĵ�����ֵ	
 
	
  if(state&TX_OK)flag=1;
	
	NRF24L01_CSN=0;       //CSN=0
	NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS, 0xff);
	NRF24L01_Write_Reg(FLUSH_TX,0xff);            
	NRF24L01_CSN=1 ;       //CSN=1

	 
	return flag;//����ԭ����ʧ��
}


  
