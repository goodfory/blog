#include "htimedead.h"

TIM_HandleTypeDef GTime_Handler;
TIM_OC_InitTypeDef GTIME_CHy_Handler;
TIM_BreakDeadTimeConfigTypeDef Gtime_breakdead_handler;
void Htime_dead_init(u16 arr,u16 psc)
{

	GTime_Handler.Instance = TIM1;
	GTime_Handler.Init .Period = arr;
	GTime_Handler.Init .ClockDivision = TIM_CLOCKDIVISION_DIV4;//可用于配置Tdts；
	GTime_Handler.Init .CounterMode = TIM_COUNTERMODE_UP;
	GTime_Handler.Init .Prescaler = psc;
	GTime_Handler.Init .RepetitionCounter = 0;//重复计数器初始值
	
	HAL_TIM_PWM_Init(&GTime_Handler);
	
//	GTIME_CHy_Handler.OCFastMode = TIM_OCFAST_ENABLE;//快速模式状态
//	GTIME_CHy_Handler.OCIdleState = ;
	GTIME_CHy_Handler.OCMode = TIM_OCMODE_PWM1;
//	GTIME_CHy_Handler.OCNIdleState = ;
	GTIME_CHy_Handler.OCPolarity = TIM_OCPOLARITY_HIGH;//输出比较的极性为高
	GTIME_CHy_Handler.OCNIdleState =TIM_OCNPOLARITY_HIGH;
	GTIME_CHy_Handler.OCIdleState = TIM_OCIDLESTATE_RESET;
	GTIME_CHy_Handler.OCNIdleState = TIM_OCNIDLESTATE_RESET;
	
//	GTIME_CHy_Handler.Pulse = arr*(3/10);//此值用于设置占空比

	HAL_TIM_PWM_ConfigChannel(&GTime_Handler,&GTIME_CHy_Handler,TIM_CHANNEL_1);
	
	Gtime_breakdead_handler.OffStateRunMode = TIM_OSSR_DISABLE;	//ADC数模用
	Gtime_breakdead_handler.OffStateIDLEMode =TIM_OSSI_DISABLE ;//ADC数模用
	/*在STM32F1系列微控制器中，OSSR（One Sample per Second Rate）和OSSI（One Sample per Second Inactive）
	是与ADC（模数转换器）相关的配置选项。
OSSR（运行模式下“关闭状态”选择）用于设置ADC在运行模式下的关闭状态。
当OSSR被使能时，ADC会在转换完成后自动关闭。这可以帮助节省功耗，特别是在需要连续进行多次ADC转换的情况下。
OSSI（空闲模式下“关闭状态”选择）用于设置ADC在空闲模式下的关闭状态。
当OSSI被使能时，ADC会在设备进入空闲模式后自动关闭。这也可以帮助节省功耗，特别是在需要在设备空闲时进行ADC转换的情况下。
通过使用OSSR和OSSI选项，可以根据应用需求和功耗要求来配置ADC的关闭状态，以最大程度地节省功耗并提高系统效率。*/


	Gtime_breakdead_handler.BreakState = TIM_BREAK_ENABLE;
	Gtime_breakdead_handler.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
	Gtime_breakdead_handler.LockLevel = TIM_LOCKLEVEL_OFF;
	Gtime_breakdead_handler.AutomaticOutput = TIM_AUTOMATICOUTPUT_ENABLE;
	HAL_TIMEx_ConfigBreakDeadTime(&GTime_Handler,&Gtime_breakdead_handler);
	
	HAL_TIM_PWM_Start(&GTime_Handler,TIM_CHANNEL_1);
	HAL_TIMEx_PWMN_Start(&GTime_Handler,TIM_CHANNEL_1);
}
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
    
    Gpio_Handler.Pin = GPIO_PIN_6;	
		HAL_GPIO_Init(GPIOA, &Gpio_Handler);
		
    Gpio_Handler.Pin = GPIO_PIN_7;	
		HAL_GPIO_Init(GPIOA, &Gpio_Handler);		
		
//	  HAL_NVIC_SetPriority(TIM1_UP_IRQn, 2 ,1);//更新中断：TIM1_UP_IRQn
//		HAL_NVIC_EnableIRQ(TIM1_UP_IRQn);
		
		__HAL_RCC_AFIO_CLK_ENABLE();//开启重映射
		__HAL_AFIO_REMAP_TIM1_PARTIAL();//选择重映射的模式
		
	}
}

void gtim_set_pwm_ccrx(u16 ccr,u8 dtg)//单独拿出来提供代码维护效率
{

  __HAL_TIM_SET_COMPARE(&GTime_Handler,TIM_CHANNEL_1,ccr);//该函数可以直接设置CCRX的值
	Gtime_breakdead_handler.DeadTime = dtg;
  HAL_TIMEx_ConfigBreakDeadTime(&GTime_Handler,&Gtime_breakdead_handler);

}



