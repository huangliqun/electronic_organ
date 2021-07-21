#include "MUSIC_PLAY.h"
#include "can.h"
#include "MUSIC_LIB.h"
#include "GLOBAL_VARIABLE.h"
void Music_Running()
{
	u8 rec_flag;
	u8 key_num[1];
	u8 i;
	Led_CmdStrue.id_num=0;
	Led_CmdStrue.diban_mode=1;
	Led_CmdStrue.colo_dat=2;
	Led_CmdStrue.duan_dat=0xFF;
	Led_CmdStrue.wei_dat=0xFF;
	Led_CAN_Send(&Led_CmdStrue);
	while(1)
	{
		rec_flag=Can_Receive_Msg(key_num);					//¥•∑¢…®√Ë
		if(rec_flag)
		{
			if(key_num[0]==music_test[i])
			{
				Led_CmdStrue.id_num=i;
				Led_CAN_Send(&Led_CmdStrue);	
			}
		}
		
	}
}