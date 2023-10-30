#ifndef __HTIMEDEAD_H
#define __HTIMEDEAD_H

#include "sys.h"
#include "delay.h"

extern  TIM_HandleTypeDef GTime_Handler;


void Htime_dead_init(u16 arr,u16 psc);
void gtim_set_pwm_ccrx(u16 ccr,u8 dtg);


#endif




