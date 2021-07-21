#ifndef __RAND_NUM_H
#define __RAND_NUM_H
#include "stm32f10x.h"
#define  MAX_RAND 150
void Rand_Init(void);
void Rand_Running(u8 max_num,u8 min_num,u8 rele_flag);
#endif
