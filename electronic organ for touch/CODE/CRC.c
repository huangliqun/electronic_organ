#include "CRC.h"
const u16 crc_ta[16]={ /* CRC ��ʽ�� */
0x0000,0x1021,0x2042,0x3063,0x4084,0x50a5,0x60c6,0x70e7,
0x8108,0x9129,0xa14a,0xb16b,0xc18c,0xd1ad,0xe1ce,0xf1ef,
};
u16 cal_crc(u8 *ptr,u8 len)
{
	u16 crc;
	u8 temp;
	crc=0;	
	while(len--!=0)
	{
		temp=((u8)(crc/256))/16;	  /* �ݴ�CRC �ĸ���λ */
		crc<<=4;						  /* CRC ����4 λ���൱��ȡCRC �ĵ�12 λ��*/
		crc^=crc_ta[temp^(*ptr/16)];  /* CRC �ĸ�4 λ�ͱ��ֽڵ�ǰ���ֽ���Ӻ������CRC��
										Ȼ�������һ��CRC ������ */
		temp=((u8)(crc/256))/16;	  /* �ݴ�CRC �ĸ���λ */
		crc<<=4;						  /* CRC ����4 λ���൱��ȡCRC �ĵ�12 λ��*/
		crc^=crc_ta[temp^(*ptr&0x0f)]; /* CRC �ĸ�4 λ�ͱ��ֽڵĺ���ֽ���Ӻ������
										CRC��Ȼ���ټ�����һ��CRC���� */								
		ptr++;
	}
	return crc;	
}
