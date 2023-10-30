#include "hoctime.h"

TIM_HandleTypeDef g_htime_oc_handler;
void Htime_OC_Init(u16 arr,u16 psc)
{
	TIM_OC_InitTypeDef htime_oc_chy_handler;

	g_htime_oc_handler.Instance = TIM1;
	g_htime_oc_handler.Init.Period = arr;
	g_htime_oc_handler.Init.CounterMode = TIM_COUNTERMODE_UP;
	g_htime_oc_handler.Init.Prescaler= psc;

	HAL_TIM_OC_Init(&g_htime_oc_handler);
		
	htime_oc_chy_handler.OCMode = TIM_OCMODE_TOGGLE;
	htime_oc_chy_handler.OCPolarity =TIM_OCPOLARITY_HIGH;
//	htime_oc_chy_handler.Pulse =arr/2;
	//通过宏来设置比较值
	
	HAL_TIM_OC_ConfigChannel(&g_htime_oc_handler,&htime_oc_chy_handler, TIM_CHANNEL_1);
	HAL_TIM_OC_ConfigChannel(&g_htime_oc_handler,&htime_oc_chy_handler, TIM_CHANNEL_2);	
	HAL_TIM_OC_ConfigChannel(&g_htime_oc_handler,&htime_oc_chy_handler, TIM_CHANNEL_3);	
	HAL_TIM_OC_ConfigChannel(&g_htime_oc_handler,&htime_oc_chy_handler, TIM_CHANNEL_4);	
	
	__HAL_TIM_ENABLE_OCxPRELOAD(&g_htime_oc_handler,TIM_CHANNEL_1);
	__HAL_TIM_ENABLE_OCxPRELOAD(&g_htime_oc_handler,TIM_CHANNEL_2);
	__HAL_TIM_ENABLE_OCxPRELOAD(&g_htime_oc_handler,TIM_CHANNEL_3);
	__HAL_TIM_ENABLE_OCxPRELOAD(&g_htime_oc_handler,TIM_CHANNEL_4);

	
	HAL_TIM_OC_Start(&g_htime_oc_handler, TIM_CHANNEL_1);
  HAL_TIM_OC_Start(&g_htime_oc_handler, TIM_CHANNEL_2);
	HAL_TIM_OC_Start(&g_htime_oc_handler, TIM_CHANNEL_3);
	HAL_TIM_OC_Start(&g_htime_oc_handler, TIM_CHANNEL_4);


}

void HAL_TIM_OC_MspInit(TIM_HandleTypeDef *htim)
{
	GPIO_InitTypeDef gpio_init;
	
	
	if(htim->Instance ==TIM1)
	{
		__HAL_RCC_TIM1_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();
		
		gpio_init.Pin = GPIO_PIN_8;
		gpio_init.Mode = GPIO_MODE_AF_PP;
		gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;

		HAL_GPIO_Init(GPIOA, &gpio_init);
		
		gpio_init.Pin = GPIO_PIN_9;
		HAL_GPIO_Init(GPIOA, &gpio_init);
		
		gpio_init.Pin = GPIO_PIN_10;
		HAL_GPIO_Init(GPIOA, &gpio_init);		
		
		gpio_init.Pin = GPIO_PIN_11;
		HAL_GPIO_Init(GPIOA, &gpio_init);		
	}
	
}
	


