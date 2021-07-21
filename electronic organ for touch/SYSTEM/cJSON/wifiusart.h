#ifndef __BULETOOTH_H
#define __BULETOOTH_H

#include "main.h"
#define RXBUFSIZE       2048
  
void WifiUsart_task(void);//ÈÎÎñº¯Êý 
 
extern u8 g_uint8_tRecData[RXBUFSIZE];                    //???????
extern u16 g_uint8_tRecData_Len;                        //???????
 
  
void clc_uart(void);
 void wifi_recver_data_deal(void);
 
 
#endif

