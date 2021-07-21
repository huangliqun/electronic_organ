#include "stm32f10x.h"
#include "usart.h"
#include "RAND_NUM.h"

#define max_ren 4							//�������
#define max_diban 18							//���ذ���
#define max_len 160							//�ذ�����װ����

#define MODE_1	1
#define MODE_2	2
#define MODE_3  3	//����ģʽ
#define MODE_4  4 	//���ģʽ

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

u8 Game_CMD;								//��Ϸ����ָ�����
#define Game_Start 1
#define Game_Stop  2


u8 Config_table[4];						//4�����ݸ�����ѭ�� ���� �ܴ���/��ʱ��
u8 Ban_table[max_ren][max_len];			//4��������� 200�ǵذ�˳����� ���ܵ����������
u8 Rele_table[max_len];					//50�ǹ�������
u8 Num_table[max_len];					//200�Ǵ������� ͬһ���������Ĵ����ۼ�װ��ֵ
u8 Mode_table[max_len];					//�ذ���ʾ��ģʽ
u8 LED_table[max_len];					//�ذ����
u8 Ban_length;							//�ذ������ܳ���
u8 EBan_length;							//Ч���ذ峤��
u8 EBan_table[max_ren][max_diban];		//Ч���ذ����
u8 ELED_table[max_diban];				//Ч���ذ�ƴ�����
u8 EMode_table[max_diban];				//Ч���ذ��ģʽ
u8 Display[max_diban];					//��ʾ��װ�ر���

u8 Xiao_Bian_Flag;

u8 colo_table[6]={0x00,0x01,0x02,0x04,0x03,0x07};	//���͸��װ����ɫ����

extern u8 msec_time;								//��ʱ����
extern u8 sec_time;								//��ʱ��
extern u8 miu_time;								//��ʱ��
extern u8 hor_time;								//��ʱʱ
extern u8 key_time;								//�����ݴ�ʱ��
extern u8 standby_time;							//����ʱ��

u16 all_num[max_ren];								//�ܸ���	4���������
u16 jisuan[max_ren];

typedef struct
{
	u8 id_num;			  //�ذ�id
	u8 diban_mode;		 //�ذ�ģʽ
	u8 colo_dat; 		 //��ɫ
	u8 wei_dat;			//λ����
	u8 duan_dat;		//������	
}Led_CmdTypeDef;

typedef struct
{
	u8 MODE;			//��Ϸģʽװ��
	u8 CYCLE;		    //ѭ������
	u8 NUM_PEOPLE; 		//����
	u16 SET_TAL_NUM;	//�����ܴ���
	u16 SET_TAL_TIME;	//������ʱ��	
	u8 AF;			    //һ��˳��ذ�������
}Config_InitTypeDef;

typedef struct
{
	u8 Ban_Mode;				//�ذ�ģʽ
	u8 Ban_Seq;		//4��Ӧ���� ˳�򣨵ڼ������ӣ�
	u8 Ban_Rele;		//4��Ӧ���� ������������	
	u8 Ban_Ddat;
	u8 Ban_Wdat;
}Ban_ControlTypeDef;


Ban_ControlTypeDef Ban_ControlStrue;		//�ṹ�����
Led_CmdTypeDef Led_CmdStrue;
Config_InitTypeDef Config_InitStrue;

void Ban_MG_n(u8 player_num, Ban_ControlTypeDef *Ban_ControlStruct);	   //Ban����һ������ colo����ʲô��ɫ
void time_out(u8 num);						//��ʱʱ����ʾ
void num_out(u8 num);						//������� ���65000�� num��Ӧ�����
void Ban_clean_all(void);						//������еذ�
void Ban_clean(u8 id);						//���ָ���ذ�
//void test();								//���Գ���
void RecData_Mange(void);						//�������ݴ���
void Floor_Play(Config_InitTypeDef *Config_InitStruct);	   //�ذ��淨������
void play(void);



//u8 code demo_1[]={21,23,25,27,40,42,44,46,48,60,61,62,63,65,66,67,68,84};
//u8 code demo_2[]={4,5,8,1};
