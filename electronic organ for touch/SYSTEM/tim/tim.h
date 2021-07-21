#ifndef __TIM_H
#define __TIM_H
#include "sys.h"
#include "stm32f10x_tim.h"
void TIM3_Init(u16 arr,u16 psc);		//arr重装载值 psc预分频系数
void TIM2_Init(u16 arr,u16 psc);		//arr重装载值 psc预分频系数
#endif
