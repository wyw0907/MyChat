#include "CSB.h"
#include "systick.h"

u32 CSB_Timer=0; 
u8 On_Measure=0;         //1正在测量
u8 Data_New;             
float Distance=0;  

/*************************************************************
函数功能：超声波的初始化函数，PB10为输出,配置PB11为上升沿中断，
注：配置外部中断，配置定时器
**************************************************************/
void CSB_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	/*打开时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	 
	/*选择管脚，设置为推挽输出*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIOB->BRR = GPIO_Pin_10;	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource11);	
	/*将按键配置为中断模式，上升沿触发*/ 
	EXTI_InitStructure.EXTI_Line = EXTI_Line11; 
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; 
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE; 
	EXTI_Init(&EXTI_InitStructure);
	 
	/* TIM5 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	/*计数器最大值计到65534*/
	TIM_TimeBaseStructure.TIM_Period = 65534; 
	/*预分频系数为7200-1*/
	TIM_TimeBaseStructure.TIM_Prescaler = (72-1); 
	/*时钟分割为0*/
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0; 
	/*配置为向下计数模式*/
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
	/*初始化TIMER5,*/
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	/*先不使能*/
	TIM_Cmd(TIM4, ENABLE);
	
	/*中断分组为2,4个抢占优先级，4个从优先级*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
  /*选中按键1对应的外部中断通道,优先级如下*/
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);	
}

/******************************************************************
函数功能：启动一次超声波的测量
注：测量成功返回1，失败返回0
******************************************************************/
void Measure(void)
{
	static uint32_t lastTm;
	if(get_timer()-lastTm>100) //前后两次测量时间间隔不得大于100ms
	{
		if(!On_Measure)
		{
			GPIOB->BSRR = GPIO_Pin_10;	
			udelay(20);
			GPIOB->BRR = GPIO_Pin_10;	
			CSB_Timer=get_timer(); 
			On_Measure = 1;	
		}
		else if((get_timer()-CSB_Timer)>1500)   
		{
				On_Measure = 0;			
		}	
		lastTm=get_timer();
	}
}

void EXTI15_10_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line11)==SET)
	{
		//假如是下降沿中断
		if(EXTI->FTSR&0x0800)
		{
		  Distance = TIM4->CNT*0.017;         //计算出距离
		  //由下降沿沿触发改为上升沿触发
		  EXTI->RTSR |= (1<<11);
		  EXTI->FTSR &=~(1<<11);
			Data_New=1;
      On_Measure = 0;				
		}
		else
		{
			TIM4->CNT = 0x00;
		  //由上升沿触发改为下降沿触发
		  EXTI->RTSR &=~(1<<11);
		  EXTI->FTSR |= (1<<11);
		}
		EXTI_ClearITPendingBit(EXTI_Line11);
	}    
}





