#define	PAGE_ADDR	(0x08000000 + 63 * 1024)
#include"eeprom.h"
//#include"stm32f10x_flash.h"
#include "stm32f10x.h"
u8 MemReadByte(u16 *data,u16 num)        //读出 num字节的数据  成功返回1                          
{
	u16 *temp_addr = (u16 *)PAGE_ADDR;

	while(num --)
	{
		*data ++ = *temp_addr ++;
	}
																									
	return 1;                                                                                                        
}

u8 MemWriteByte(u16 *data,u16 num)           //写入 num字节的数据  成功返回1                       
{
	FLASH_Status temp_stat;
	u32 temp_addr = PAGE_ADDR;
			
	FLASH_Unlock();                        //解锁flash                                                                                    // Flash??,??????????
	temp_stat = FLASH_ErasePage(PAGE_ADDR);                                                        // ??????
	
	if(temp_stat != FLASH_COMPLETE)
	{
		FLASH_Lock();          //锁定
		return 0;
	}
	
	while(num --)
	{
		temp_stat = FLASH_ProgramHalfWord(temp_addr,*data);
		if(temp_stat != FLASH_COMPLETE)
		{
				FLASH_Lock();
				return 0;
		}

		temp_addr += 2;
		data++;
	}
	
	FLASH_Lock();        
	return 1;
}


//#include "app_cfg.h"                                        


//int main(void) 
//{ 
//        uint16 cnt;
//                
//        STM32Init();  
//        
//        MemWriteByte(write_buf,10);  
//        for(cnt = 0;cnt < 10;cnt ++)
//        {
//                write_buf[cnt] = 11;
//        }
//        MemReadByte(write_buf,10); 
//         
//    while(1)                                                                          
//    {
//        ;        
//    }  
//} 
