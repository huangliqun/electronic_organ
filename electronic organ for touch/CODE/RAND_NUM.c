#include "RAND_NUM.h"
#include "adc.h"

u8 Ban_Rand_Table[MAX_RAND];
u8 Rele_Rand_Table[MAX_RAND];

void Rand_Init()
{
	u8 i;
	Adc_Init();
	for(i=0;i<MAX_RAND;i++)
	{
		Ban_Rand_Table[i]=0;
		
	}
}
void Rand_Running(u8 max_num,u8 min_num,u8 rele_flag)
{
	u16 temp;
	u8 i,j;
	for(i=0;i<MAX_RAND;)
	{
		temp=0x1F&Get_Adc(ADC_Channel_5);
		temp=0x1F&(temp+Get_Adc(ADC_Channel_5));
		if(temp>min_num&&temp<max_num)
		{
			Ban_Rand_Table[i]=temp;
			i++;
		}
	}
	for(i=0;i<MAX_RAND;i++)
	{
		if(Ban_Rand_Table[i]>9)
		{
			Ban_Rand_Table[i]=0x40|Ban_Rand_Table[i];
		}
		else
		{
			Ban_Rand_Table[i]=0x20|Ban_Rand_Table[i];
		}
	}
	if(rele_flag)
	{
		for(i=0;i<MAX_RAND;)
		{
			temp=0x1F&Get_Adc(4);
			temp=0x1F&(temp+Get_Adc(4));
			temp=temp%10;
			if(temp>0&&temp<5)
			{
				for(j=0;j<temp;j++)
				{
					Rele_Rand_Table[i]=temp;
					i++;
					if(i>=MAX_RAND)
					{				
						break;
					}
				}
				if(i>=MAX_RAND)
				break;
			}
		}
	}
	if(rele_flag==0)
	{
		for(i=0;i<MAX_RAND;i++)
		Rele_Rand_Table[i]=0x01;
	}
	for(i=0;i<(MAX_RAND-1);i++)
	{
		if((Ban_Rand_Table[i]&0x1F)==(Ban_Rand_Table[(i+1)]&0x1F))
		{
			Ban_Rand_Table[i]=0x80|(Ban_Rand_Table[i]&0x1F);
			Ban_Rand_Table[(i+1)]=0x80|(Ban_Rand_Table[(i+1)]&0x1F);
		}
	}
}
