#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"

void TIM1_PWM_Init(u16 arr,u16 psc);
void TIM2_Int_Init(u16 arr,u16 psc);
void TIM3_Int_Init(u16 arr,u16 psc);
void TIM5_Cap_Init(u16 arr,u16 psc);

extern u32 SYS_TIME;

#endif
