#include "pwm.h"

/***************************************************************
函数功能：对定时器4的基本配置，输出频率为400Hz，初始化占空比为0
          输出的4个通道分别为PA6,7(CH1,CH2)
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
	 
    /*重装初值为1000，时钟分频为1440分频，向下计数*/
	/*分频的时候是要减一，比如1的时候是2分频*/
	TIM_TimeBaseStructure.TIM_Period = 1000; 
    TIM_TimeBaseStructure.TIM_Prescaler = (144-1); 
    TIM_TimeBaseStructure.TIM_ClockDivision = 0x0; 
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down; 
    TIM_TimeBaseInit(TIM3, & TIM_TimeBaseStructure);
	 
    /*使能PWM1模式，打开4个通道，先初始化占空比为0，比较匹配后输出极性高*/
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0; 
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	/*初始化4个通道*/
	TIM_OC1Init(TIM3, & TIM_OCInitStructure);
	TIM_OC2Init(TIM3, & TIM_OCInitStructure);
	TIM_OC3Init(TIM3, & TIM_OCInitStructure); 
    TIM_OC4Init(TIM3, & TIM_OCInitStructure);
	/*使能4个通道的溢出重装载*/
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(TIM3, ENABLE); //使能TIMx在ARR上的预装载寄存器
	TIM_Cmd(TIM3, ENABLE);    			//打开定时器
}

/*****************************************************
函数功能：更新PWM
PWM初始值为1000，
故，设置初始值为500即占空比为50%
******************************************************/
void Moto_PwmUpdate(uint16_t MOTO1_PWM, uint16_t MOTO2_PWM, uint16_t MOTO3_PWM, uint16_t MOTO4_PWM)
{
	TIM_SetCompare1(TIM3, MOTO1_PWM);
	TIM_SetCompare2(TIM3, MOTO2_PWM);
	TIM_SetCompare3(TIM3, MOTO3_PWM);
	TIM_SetCompare4(TIM3, MOTO4_PWM);
}



