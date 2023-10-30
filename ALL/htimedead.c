#include "htimedead.h"

TIM_HandleTypeDef GTime_Handler;
TIM_OC_InitTypeDef GTIME_CHy_Handler;
TIM_BreakDeadTimeConfigTypeDef Gtime_breakdead_handler;
void Htime_dead_init(u16 arr,u16 psc)
{

	GTime_Handler.Instance = TIM1;
	GTime_Handler.Init .Period = arr;
	GTime_Handler.Init .ClockDivision = TIM_CLOCKDIVISION_DIV4;//����������Tdts��
	GTime_Handler.Init .CounterMode = TIM_COUNTERMODE_UP;
	GTime_Handler.Init .Prescaler = psc;
	GTime_Handler.Init .RepetitionCounter = 0;//�ظ���������ʼֵ
	
	HAL_TIM_PWM_Init(&GTime_Handler);
	
//	GTIME_CHy_Handler.OCFastMode = TIM_OCFAST_ENABLE;//����ģʽ״̬
//	GTIME_CHy_Handler.OCIdleState = ;
	GTIME_CHy_Handler.OCMode = TIM_OCMODE_PWM1;
//	GTIME_CHy_Handler.OCNIdleState = ;
	GTIME_CHy_Handler.OCPolarity = TIM_OCPOLARITY_HIGH;//����Ƚϵļ���Ϊ��
	GTIME_CHy_Handler.OCNIdleState =TIM_OCNPOLARITY_HIGH;
	GTIME_CHy_Handler.OCIdleState = TIM_OCIDLESTATE_RESET;
	GTIME_CHy_Handler.OCNIdleState = TIM_OCNIDLESTATE_RESET;
	
//	GTIME_CHy_Handler.Pulse = arr*(3/10);//��ֵ��������ռ�ձ�

	HAL_TIM_PWM_ConfigChannel(&GTime_Handler,&GTIME_CHy_Handler,TIM_CHANNEL_1);
	
	Gtime_breakdead_handler.OffStateRunMode = TIM_OSSR_DISABLE;	//ADC��ģ��
	Gtime_breakdead_handler.OffStateIDLEMode =TIM_OSSI_DISABLE ;//ADC��ģ��
	/*��STM32F1ϵ��΢�������У�OSSR��One Sample per Second Rate����OSSI��One Sample per Second Inactive��
	����ADC��ģ��ת��������ص�����ѡ�
OSSR������ģʽ�¡��ر�״̬��ѡ����������ADC������ģʽ�µĹر�״̬��
��OSSR��ʹ��ʱ��ADC����ת����ɺ��Զ��رա�����԰�����ʡ���ģ��ر�������Ҫ�������ж��ADCת��������¡�
OSSI������ģʽ�¡��ر�״̬��ѡ����������ADC�ڿ���ģʽ�µĹر�״̬��
��OSSI��ʹ��ʱ��ADC�����豸�������ģʽ���Զ��رա���Ҳ���԰�����ʡ���ģ��ر�������Ҫ���豸����ʱ����ADCת��������¡�
ͨ��ʹ��OSSR��OSSIѡ����Ը���Ӧ������͹���Ҫ��������ADC�Ĺر�״̬�������̶ȵؽ�ʡ���Ĳ����ϵͳЧ�ʡ�*/


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
		
//	  HAL_NVIC_SetPriority(TIM1_UP_IRQn, 2 ,1);//�����жϣ�TIM1_UP_IRQn
//		HAL_NVIC_EnableIRQ(TIM1_UP_IRQn);
		
		__HAL_RCC_AFIO_CLK_ENABLE();//������ӳ��
		__HAL_AFIO_REMAP_TIM1_PARTIAL();//ѡ����ӳ���ģʽ
		
	}
}

void gtim_set_pwm_ccrx(u16 ccr,u8 dtg)//�����ó����ṩ����ά��Ч��
{

  __HAL_TIM_SET_COMPARE(&GTime_Handler,TIM_CHANNEL_1,ccr);//�ú�������ֱ������CCRX��ֵ
	Gtime_breakdead_handler.DeadTime = dtg;
  HAL_TIMEx_ConfigBreakDeadTime(&GTime_Handler,&Gtime_breakdead_handler);

}



