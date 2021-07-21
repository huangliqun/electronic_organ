#include "CRC.h"
const u16 crc_ta[16]={ /* CRC 余式表 */
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
		temp=((u8)(crc/256))/16;	  /* 暂存CRC 的高四位 */
		crc<<=4;						  /* CRC 右移4 位，相当于取CRC 的低12 位）*/
		crc^=crc_ta[temp^(*ptr/16)];  /* CRC 的高4 位和本字节的前半字节相加后查表计算CRC，
										然后加上上一次CRC 的余数 */
		temp=((u8)(crc/256))/16;	  /* 暂存CRC 的高四位 */
		crc<<=4;						  /* CRC 右移4 位，相当于取CRC 的低12 位）*/
		crc^=crc_ta[temp^(*ptr&0x0f)]; /* CRC 的高4 位和本字节的后半字节相加后查表计算
										CRC，然后再加上上一次CRC余数 */								
		ptr++;
	}
	return crc;	
}
