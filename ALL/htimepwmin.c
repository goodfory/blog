#include "htimepwmin.h"

TIM_HandleTypeDef Tim8_Handler;

u8 g_timchy_pwmin_sta = 0; //PWM����״̬�жϣ��Ƿ񲶻������غ��½����Լ��Ƿ��ڲ���״̬
u16 g_timchy_pwmin_psc = 0;//PWM�����Ƶϵ��������̶�Ϊ0
u32 g_timchy_pwmin_hval = 0;//PWM�ĸߵ�ƽ����Ҫ���ֵ
u32 g_timchy_pwmin_cval = 0;//PWM�����ڿ����Ҫ���ֵ


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

	
	
	//���ô�ģʽ�Ĵ���
	TimSlave_handler.SlaveMode = TIM_SLAVEMODE_RESET;//ѡ��λģʽ
	TimSlave_handler.TriggerPolarity = TIM_TRIGGERPOLARITY_RISING;
	TimSlave_handler.InputTrigger = TIM_TS_TI1FP1;//ѡ��ͨ��һ�Ĵ���Դ
	TimSlave_handler.TriggerFilter = 0;//���˲�

  HAL_TIM_SlaveConfigSynchro(&Tim8_Handler, &TimSlave_handler);
  
	
  //IC1���������ش���TI1FP1
	Timic_handler.ICPolarity = TIM_ICPOLARITY_RISING;//�����ؼ��
	Timic_handler.ICPrescaler = TIM_ICPSC_DIV1;//���˲�
	Timic_handler.ICFilter = 0;
	Timic_handler.ICSelection = TIM_ICSELECTION_DIRECTTI;//ѡ�������IC1ӳ�䵽TI1
	HAL_TIM_IC_ConfigChannel(&Tim8_Handler, &Timic_handler, TIM_CHANNEL_1);
	
  //IC2�����½��ش���TI1FP2
	Timic_handler.ICPolarity = TIM_ICPOLARITY_FALLING;//�½��ؼ�� 	
	Timic_handler.ICSelection = TIM_ICSELECTION_INDIRECTTI;//ѡ�������IC2ӳ�䵽TI1
	HAL_TIM_IC_ConfigChannel(&Tim8_Handler, &Timic_handler, TIM_CHANNEL_2);
	

	HAL_TIM_IC_Start_IT(&Tim8_Handler, TIM_CHANNEL_1);//����ͨ��һ�����ж�	
	HAL_TIM_IC_Start(&Tim8_Handler, TIM_CHANNEL_2);//����ͨ��������

}
	

void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{
	GPIO_InitTypeDef key_init = {0};

	if(htim->Instance == TIM8)
	{
		__HAL_RCC_GPIOC_CLK_ENABLE();//��GPIOC����ʱ��ʹ��
		__HAL_RCC_TIM8_CLK_ENABLE();//ʹ�ܶ�ʱ��8ʱ��
		//ʱ��ʹ�ܶ���MSP����
		
		
		key_init.Pin = GPIO_PIN_6;//ѡ������ģʽ5��
		key_init.Mode = GPIO_MODE_AF_INPUT;//ѡ������ģʽ
		key_init.Pull = GPIO_PULLDOWN;
//	  key_init.Speed = GPIO_SPEED_FREQ_HIGH;//ѡ������ٶȣ��������
		HAL_GPIO_Init(GPIOC, &key_init);
	  
		HAL_NVIC_SetPriority(TIM8_CC_IRQn,2,0);
		HAL_NVIC_EnableIRQ(TIM8_CC_IRQn);  
	}
}

void TIM8_CC_IRQHandler()
{
		HAL_TIM_IRQHandler(&Tim8_Handler);
}
//��ʱ��8���벶�� �жϺ�������TIM1,8�У���ͨ��ʱ��û��





void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)//
{
	if(htim->Instance == TIM8)
	{
		if(g_timchy_pwmin_sta == 0)
		{
			if(htim->Channel  == HAL_TIM_ACTIVE_CHANNEL_1)
			{
				g_timchy_pwmin_hval = HAL_TIM_ReadCapturedValue(&Tim8_Handler,TIM_CHANNEL_2)+1+1;
				//+1����Ϊ����ϵ������������ʱ�䣩+1��0��ʼ��
				g_timchy_pwmin_hval = HAL_TIM_ReadCapturedValue(&Tim8_Handler,TIM_CHANNEL_1)+1+1;
				//����  
        g_timchy_pwmin_sta = 1��
			}
			
			
			
		}
		
		
		
	}
	
}





