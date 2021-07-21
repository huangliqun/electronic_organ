#ifndef __CANTYPE_H
#define __CANTYPE_H	 
#include "sys.h"	   


 
typedef enum FLOOD_REC_CMD
{
	FLOOD_SETID_BEGIN =0x00,
	FLOOD_SETID_OVER=1,
	FLOOD_LED   =0x02	,		//控制led
	FLOOD_BREATH =0x03 ,	// 呼吸  
	FLOOD_ALWAYS =0x04, 		// 长亮
	FLOOD_LIUSHUI=0x05,
  FLOOD_RADIO1=0x06,    //广播控制 球 1-10
	FLOOD_RADIO2=0x07	,		//广播控制 球 11-20
  FLOOD_RADIONO1=0x08,    //广播控制 球 不能踩灭 ipad直接控制模式下使用
	FLOOD_RADIONO2=0x09	,   	//广播控制 球 不能踩灭
	FLOOD_SHUTDOWN=0x5a
}FLOOD_REC_CMD;

#endif
