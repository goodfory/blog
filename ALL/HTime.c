#include "HTime.h"

TIM_HandleTypeDef GTime_Handler;
TIM_OC_InitTypeDef GTIME_CHy_Handler;

static u8 g_npwm_remain = 0;

void HTIME_PWM_Init(u16 arr,u16 psc)
{
	GTime_Handler.Instance = TIM1;
	GTime_Handler.Init .Period = arr;
	GTime_Handler.Init .ClockDivision = TIM_CLOCKDIVISION_DIV1;
	GTime_Handler.Init .CounterMode = TIM_COUNTERMODE_UP;
	GTime_Handler.Init .Prescaler = psc;
	GTime_Handler.Init .RepetitionCounter = 0;//重复计数器初始值
	
	HAL_TIM_PWM_Init(&GTime_Handler);

	GTIME_CHy_Handler.OCFastMode = TIM_OCFAST_ENABLE;//快速模式状态
//	GTIME_CHy_Handler.OCIdleState = ;
	GTIME_CHy_Handler.OCMode = TIM_OCMODE_PWM1;
//	GTIME_CHy_Handler.OCNIdleState = ;
	GTIME_CHy_Handler.OCPolarity = TIM_OCPOLARITY_HIGH;//输出比较的极性为高
	GTIME_CHy_Handler.Pulse = arr/2;//此值用于设置占空比
	
	HAL_TIM_PWM_ConfigChannel(&GTime_Handler,&GTIME_CHy_Handler,TIM_CHANNEL_1);
	
	__HAL_TIM_ENABLE_IT(&GTime_Handler,TIM_IT_UPDATE);
	HAL_TIM_PWM_Start(&GTime_Handler, TIM_CHANNEL_1);

}


//我用的是LED0即PA8,且用的是TIM1定时器通道1


void  HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
	GPIO_InitTypeDef Gpio_Handler;
	
  if(htim->Instance ==  TIM1)
	{
	  __HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_TIM1_CLK_ENABLE();
	  
		Gpio_Handler.Pin = GPIO_PIN_8;
		Gpio_Handler.Mode = GPIO_MODE_AF_PP;
		Gpio_Handler.Speed = GPIO_SPEED_FREQ_HIGH;
		
		HAL_GPIO_Init(GPIOA, &Gpio_Handler);
    
	  HAL_NVIC_SetPriority(TIM1_UP_IRQn, 2 ,1);//更新中断：TIM1_UP_IRQn
		HAL_NVIC_EnableIRQ(TIM1_UP_IRQn);

	}
}

void Htim_set_pwmcount(u8 npwm)
{
	if(npwm == 0) return;
	g_npwm_remain = npwm;
	
	HAL_TIM_GenerateEvent(&GTime_Handler,TIM_EVENTSOURCE_UPDATE);
	__HAL_TIM_ENABLE(&GTime_Handler);
	
}
void TIM1_UP_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&GTime_Handler);

}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM1)
	{
		if(g_npwm_remain)
		{
		
		  TIM1->RCR = g_npwm_remain - 1 ;
		  HAL_TIM_GenerateEvent(&GTime_Handler,TIM_EVENTSOURCE_UPDATE);
			g_npwm_remain = 0;
			
		}
		else
		{
		  TIM1->CR1 &= 0XFE;
		}
	}
}






