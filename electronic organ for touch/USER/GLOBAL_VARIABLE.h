#include "stm32f10x.h"
#include "usart.h"
#include "RAND_NUM.h"

#define max_ren 4							//最大人数
#define max_diban 18							//最大地板数
#define max_len 160							//地板数据装载量

#define MODE_1	1
#define MODE_2	2
#define MODE_3  3	//音乐模式
#define MODE_4  4 	//随机模式

extern u8 rec[USART_REC_LEN];
extern u8 rec_over;
extern u8 r_num;
extern u8 Ban_Rand_Table[MAX_RAND];
extern u8 Rele_Rand_Table[MAX_RAND];

extern u8 error[];
extern u8 ok[];
extern u8 Game_StartTable[];
extern u8 Game_StopTable[];
extern u8 WinnerTable[];
extern u8 player[];
extern u8 ready[];

u8 Game_CMD;								//游戏特殊指令变量
#define Game_Start 1
#define Game_Stop  2


u8 Config_table[4];						//4是数据个数：循环 人数 总次数/总时长
u8 Ban_table[max_ren][max_len];			//4是玩家人数 200是地板顺序个数 接受到的玩家数据
u8 Rele_table[max_len];					//50是关联数据
u8 Num_table[max_len];					//200是次数个数 同一变量触发的次数累计装载值
u8 Mode_table[max_len];					//地板显示的模式
u8 LED_table[max_len];					//地板参数
u8 Ban_length;							//地板数据总长度
u8 EBan_length;							//效果地板长度
u8 EBan_table[max_ren][max_diban];		//效果地板参数
u8 ELED_table[max_diban];				//效果地板灯带参数
u8 EMode_table[max_diban];				//效果地板的模式
u8 Display[max_diban];					//演示用装载变量

u8 Xiao_Bian_Flag;

u8 colo_table[6]={0x00,0x01,0x02,0x04,0x03,0x07};	//发送给底板的颜色变量

extern u8 msec_time;								//计时毫秒
extern u8 sec_time;								//计时秒
extern u8 miu_time;								//计时分
extern u8 hor_time;								//计时时
extern u8 key_time;								//按键容错时间
extern u8 standby_time;							//待机时间

u16 all_num[max_ren];								//总个数	4是玩家人数
u16 jisuan[max_ren];

typedef struct
{
	u8 id_num;			  //地板id
	u8 diban_mode;		 //地板模式
	u8 colo_dat; 		 //颜色
	u8 wei_dat;			//位数据
	u8 duan_dat;		//段数据	
}Led_CmdTypeDef;

typedef struct
{
	u8 MODE;			//游戏模式装载
	u8 CYCLE;		    //循环次数
	u8 NUM_PEOPLE; 		//人数
	u16 SET_TAL_NUM;	//设置总次数
	u16 SET_TAL_TIME;	//设置总时间	
	u8 AF;			    //一次顺序地板所有量
}Config_InitTypeDef;

typedef struct
{
	u8 Ban_Mode;				//地板模式
	u8 Ban_Seq;		//4对应人数 顺序（第几个板子）
	u8 Ban_Rele;		//4对应人数 关联板子数量	
	u8 Ban_Ddat;
	u8 Ban_Wdat;
}Ban_ControlTypeDef;


Ban_ControlTypeDef Ban_ControlStrue;		//结构体变量
Led_CmdTypeDef Led_CmdStrue;
Config_InitTypeDef Config_InitStrue;

void Ban_MG_n(u8 player_num, Ban_ControlTypeDef *Ban_ControlStruct);	   //Ban是那一个板亮 colo是亮什么颜色
void time_out(u8 num);						//计时时间显示
void num_out(u8 num);						//个数输出 最多65000个 num对应的玩家
void Ban_clean_all(void);						//清除所有地板
void Ban_clean(u8 id);						//清除指定地板
//void test();								//测试程序
void RecData_Mange(void);						//接收数据处理
void Floor_Play(Config_InitTypeDef *Config_InitStruct);	   //地板玩法处理函数
void play(void);



//u8 code demo_1[]={21,23,25,27,40,42,44,46,48,60,61,62,63,65,66,67,68,84};
//u8 code demo_2[]={4,5,8,1};
