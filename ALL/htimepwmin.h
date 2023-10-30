#ifndef __HTIMEPWMIN_H
#define __HTIMEPWMIN_H

#include "sys.h"
#include "delay.h"
extern TIM_HandleTypeDef Tim8_Handler;

extern u8 g_timchy_pwmin_sta ; //PWM输入状态判断，是否捕获上升沿和下降沿以及是否在捕获状态
extern u16 g_timchy_pwmin_psc ;//PWM输入分频系数，下面固定为0
extern u32 g_timchy_pwmin_hval ;//PWM的高电平脉宽，要测的值
extern u32 g_timchy_pwmin_cval ;//PWM的周期宽带，要测得值

void Htime_inpwm_init(void);



#endif

