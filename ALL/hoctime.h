#ifndef __HOCTIME_H
#define __HOCTIME_H

#include "sys.h"
#include "delay.h"

extern TIM_HandleTypeDef g_htime_oc_handler;

void Htime_OC_Init(u16 arr,u16 psc);


#endif


