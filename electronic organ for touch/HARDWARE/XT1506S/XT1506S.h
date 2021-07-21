#ifndef __XT1506S_H
#define __XT1506S_H	 
#include "sys.h"


#define LED_LONG 12
#define out PCout(15)
#define out2 PCout(14)
//////can�е�һ�ֽڶ�Ӧ��  ָ��
#define normal 0x00    			//����ģʽ
#define fixation 0x01     	//�̶�ģʽվס
#define time_mode 0x02     	//ʱ��ģʽ��һ��ʱ����һֱ̤
#define count_mode 0x03     //����ģʽ ̤һ������
#define rotate_mode 0x04    //��תģʽ
#define huxideng_mode 0x05  //������ģʽ

typedef enum LEDcolorType
{
	color_no =0x00,
	color_blue =0x02	,
	color_red =0x01	,
	color_green =0x04	,
	color_yellow =0x05	,
	color_cyan =0x06	,
	color_mag =0x03	,
	color_white= 0x07
}LEDcolorType;



void LEDOUT_Init(void);
void V1(void);
void V0(void);
void display_one_temp(LEDcolorType color,u8 liangdu);
void LED_display_temp(u8 * led_table,u8 liangdu);	
void LED_All_color(LEDcolorType color );
void LED_Start();
void LED_LIUSHUI();
void  CloseAllFloor(void);
void  LightAllFloor(void);

extern u8 led_table[LED_LONG];


#endif

