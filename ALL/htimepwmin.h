#ifndef __HTIMEPWMIN_H
#define __HTIMEPWMIN_H

#include "sys.h"
#include "delay.h"
extern TIM_HandleTypeDef Tim8_Handler;

extern u8 g_timchy_pwmin_sta ; //PWM����״̬�жϣ��Ƿ񲶻������غ��½����Լ��Ƿ��ڲ���״̬
extern u16 g_timchy_pwmin_psc ;//PWM�����Ƶϵ��������̶�Ϊ0
extern u32 g_timchy_pwmin_hval ;//PWM�ĸߵ�ƽ����Ҫ���ֵ
extern u32 g_timchy_pwmin_cval ;//PWM�����ڿ����Ҫ���ֵ

void Htime_inpwm_init(void);



#endif

