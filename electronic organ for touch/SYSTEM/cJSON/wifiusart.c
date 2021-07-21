 #include "wifiusart.h"
#include "cJsonDataDeal.h"
#include "stdlib.h"
 
u8  g_uint8_tRecData[RXBUFSIZE];                    
u16 g_uint8_tRecData_Len=0;                         
u8  UART_HAD=0;                                   
 

char *cJsonDataReturn;
 
void clc_uart1(void)
{
    UART_HAD=0;
    memset(g_uint8_tRecData, 0, sizeof(g_uint8_tRecData));
    g_uint8_tRecData_Len= 0;
}
 
void WifiSendData(char *data)
{
			printf("<start>");
			printf("%s",data);				//返回	
			printf("<over>");		
}

  

void WifiUsart_task(void )//任务函数 
{ 
 	  u16 i;
	  char *p_start=NULL;
		char *p_over=NULL;
		u16 cJsonDataRec_len;
 
    //clc_uart1(); 		

 
				p_start=strstr((const char *)g_uint8_tRecData,"<start>");
				p_over=strstr((const char *)g_uint8_tRecData,"<over>");
				
				if( (p_start!=NULL)  && (p_over!=NULL))
				{
 		
						char *cJsonDataRec; 
					
						cJsonDataRec_len= p_over - p_start -7;
					  cJsonDataRec= my_malloc(cJsonDataRec_len+1); //收到的字符串
						memset(cJsonDataRec,0,cJsonDataRec_len+1);
					
						strncpy(cJsonDataRec , p_start+7 , cJsonDataRec_len);
					  		
					  cJsonDataReturn=main_json_data_deal( (const char *)cJsonDataRec);
						my_free(cJsonDataRec);
					  if(cJsonDataReturn)
						{
								WifiSendData((char *)cJsonDataReturn);
							  my_free(cJsonDataReturn);
						}
						  
						 
					  
					  //实现over之后的数据向前移动。
					  p_over+=6;  //<over> == 6byte
					  for(i=0;i<RXBUFSIZE;i++)
					  {
							  g_uint8_tRecData[i]= p_over[i];
								if(p_over[i]==0)
								{
									  g_uint8_tRecData_Len=i;
										memset(&g_uint8_tRecData[i],0,RXBUFSIZE-i);	 
									  i=RXBUFSIZE;
								}
						}			
				}		
	      else if((g_uint8_tRecData_Len>100) && (p_start==NULL))
        {
            clc_uart1();
        }
 
}


