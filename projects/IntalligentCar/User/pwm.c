#include "pwm.h"

/***************************************************************
�������ܣ��Զ�ʱ��4�Ļ������ã����Ƶ��Ϊ400Hz����ʼ��ռ�ձ�Ϊ0
          �����4��ͨ���ֱ�ΪPA6,7(CH1,CH2)
		                     PB0,1(CH3,CH4)
**************************************************************/
void MotoPwm_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	//PA6,7
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 |GPIO_Pin_7 ; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	//PB0,1
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 |GPIO_Pin_1 ; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	 
    /*��װ��ֵΪ1000��ʱ�ӷ�ƵΪ1440��Ƶ�����¼���*/
	/*��Ƶ��ʱ����Ҫ��һ������1��ʱ����2��Ƶ*/
	TIM_TimeBaseStructure.TIM_Period = 1000; 
    TIM_TimeBaseStructure.TIM_Prescaler = (144-1); 
    TIM_TimeBaseStructure.TIM_ClockDivision = 0x0; 
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down; 
    TIM_TimeBaseInit(TIM3, & TIM_TimeBaseStructure);
	 
    /*ʹ��PWM1ģʽ����4��ͨ�����ȳ�ʼ��ռ�ձ�Ϊ0���Ƚ�ƥ���������Ը�*/
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0; 
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	/*��ʼ��4��ͨ��*/
	TIM_OC1Init(TIM3, & TIM_OCInitStructure);
	TIM_OC2Init(TIM3, & TIM_OCInitStructure);
	TIM_OC3Init(TIM3, & TIM_OCInitStructure); 
    TIM_OC4Init(TIM3, & TIM_OCInitStructure);
	/*ʹ��4��ͨ���������װ��*/
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(TIM3, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
	TIM_Cmd(TIM3, ENABLE);    			//�򿪶�ʱ��
}

/*****************************************************
�������ܣ�����PWM
PWM��ʼֵΪ1000��
�ʣ����ó�ʼֵΪ500��ռ�ձ�Ϊ50%
******************************************************/
void Moto_PwmUpdate(uint16_t MOTO1_PWM, uint16_t MOTO2_PWM, uint16_t MOTO3_PWM, uint16_t MOTO4_PWM)
{
	TIM_SetCompare1(TIM3, MOTO1_PWM);
	TIM_SetCompare2(TIM3, MOTO2_PWM);
	TIM_SetCompare3(TIM3, MOTO3_PWM);
	TIM_SetCompare4(TIM3, MOTO4_PWM);
}



