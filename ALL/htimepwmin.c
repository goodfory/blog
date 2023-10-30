#include "htimepwmin.h"

TIM_HandleTypeDef Tim8_Handler;

u8 g_timchy_pwmin_sta = 0; //PWM输入状态判断，是否捕获上升沿和下降沿以及是否在捕获状态
u16 g_timchy_pwmin_psc = 0;//PWM输入分频系数，下面固定为0
u32 g_timchy_pwmin_hval = 0;//PWM的高电平脉宽，要测的值
u32 g_timchy_pwmin_cval = 0;//PWM的周期宽带，要测得值


void Htime_inpwm_init(void)
{
	TIM_SlaveConfigTypeDef TimSlave_handler;
	TIM_IC_InitTypeDef Timic_handler;
	
  Tim8_Handler.Instance = TIM8;
  Tim8_Handler.Init .Prescaler = 0;
  Tim8_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;
	Tim8_Handler.Init.Period = 65536;
//	Tim8_Handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	HAL_TIM_IC_Init(&Tim8_Handler);

	
	
	//配置从模式寄存器
	TimSlave_handler.SlaveMode = TIM_SLAVEMODE_RESET;//选择复位模式
	TimSlave_handler.TriggerPolarity = TIM_TRIGGERPOLARITY_RISING;
	TimSlave_handler.InputTrigger = TIM_TS_TI1FP1;//选择通道一的触发源
	TimSlave_handler.TriggerFilter = 0;//不滤波

  HAL_TIM_SlaveConfigSynchro(&Tim8_Handler, &TimSlave_handler);
  
	
  //IC1捕获：上升沿触发TI1FP1
	Timic_handler.ICPolarity = TIM_ICPOLARITY_RISING;//上升沿检测
	Timic_handler.ICPrescaler = TIM_ICPSC_DIV1;//不滤波
	Timic_handler.ICFilter = 0;
	Timic_handler.ICSelection = TIM_ICSELECTION_DIRECTTI;//选择输入端IC1映射到TI1
	HAL_TIM_IC_ConfigChannel(&Tim8_Handler, &Timic_handler, TIM_CHANNEL_1);
	
  //IC2捕获：下降沿触发TI1FP2
	Timic_handler.ICPolarity = TIM_ICPOLARITY_FALLING;//下降沿检测 	
	Timic_handler.ICSelection = TIM_ICSELECTION_INDIRECTTI;//选择输入端IC2映射到TI1
	HAL_TIM_IC_ConfigChannel(&Tim8_Handler, &Timic_handler, TIM_CHANNEL_2);
	

	HAL_TIM_IC_Start_IT(&Tim8_Handler, TIM_CHANNEL_1);//开启通道一捕获中段	
	HAL_TIM_IC_Start(&Tim8_Handler, TIM_CHANNEL_2);//开启通道二捕获

}
	

void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{
	GPIO_InitTypeDef key_init = {0};

	if(htim->Instance == TIM8)
	{
		__HAL_RCC_GPIOC_CLK_ENABLE();//对GPIOC进行时钟使能
		__HAL_RCC_TIM8_CLK_ENABLE();//使能定时器8时钟
		//时钟使能都在MSP函数
		
		
		key_init.Pin = GPIO_PIN_6;//选择引脚模式5脚
		key_init.Mode = GPIO_MODE_AF_INPUT;//选择输入模式
		key_init.Pull = GPIO_PULLDOWN;
//	  key_init.Speed = GPIO_SPEED_FREQ_HIGH;//选择输出速度，高速输出
		HAL_GPIO_Init(GPIOC, &key_init);
	  
		HAL_NVIC_SetPriority(TIM8_CC_IRQn,2,0);
		HAL_NVIC_EnableIRQ(TIM8_CC_IRQn);  
	}
}

void TIM8_CC_IRQHandler()
{
		HAL_TIM_IRQHandler(&Tim8_Handler);
}
//定时器8输入捕获 中断函数仅仅TIM1,8有，普通定时器没有





void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)//
{
	if(htim->Instance == TIM8)
	{
		if(g_timchy_pwmin_sta == 0)
		{
			if(htim->Channel  == HAL_TIM_ACTIVE_CHANNEL_1)
			{
				g_timchy_pwmin_hval = HAL_TIM_ReadCapturedValue(&Tim8_Handler,TIM_CHANNEL_2)+1+1;
				//+1是因为纠正系数（程序运行时间）+1从0开始的
				g_timchy_pwmin_hval = HAL_TIM_ReadCapturedValue(&Tim8_Handler,TIM_CHANNEL_1)+1+1;
				//周期  
        g_timchy_pwmin_sta = 1；
			}
			
			
			
		}
		
		
		
	}
	
}





