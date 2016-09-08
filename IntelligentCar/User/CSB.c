#include "CSB.h"
#include "systick.h"

u32 CSB_Timer=0; 
u8 On_Measure=0;         //1���ڲ���
u8 Data_New;             
float Distance=0;  

/*************************************************************
�������ܣ��������ĳ�ʼ��������PB10Ϊ���,����PB11Ϊ�������жϣ�
ע�������ⲿ�жϣ����ö�ʱ��
**************************************************************/
void CSB_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	/*��ʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	 
	/*ѡ��ܽţ�����Ϊ�������*/
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
	/*����������Ϊ�ж�ģʽ�������ش���*/ 
	EXTI_InitStructure.EXTI_Line = EXTI_Line11; 
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; 
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE; 
	EXTI_Init(&EXTI_InitStructure);
	 
	/* TIM5 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	/*���������ֵ�Ƶ�65534*/
	TIM_TimeBaseStructure.TIM_Period = 65534; 
	/*Ԥ��Ƶϵ��Ϊ7200-1*/
	TIM_TimeBaseStructure.TIM_Prescaler = (72-1); 
	/*ʱ�ӷָ�Ϊ0*/
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0; 
	/*����Ϊ���¼���ģʽ*/
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
	/*��ʼ��TIMER5,*/
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	/*�Ȳ�ʹ��*/
	TIM_Cmd(TIM4, ENABLE);
	
	/*�жϷ���Ϊ2,4����ռ���ȼ���4�������ȼ�*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
  /*ѡ�а���1��Ӧ���ⲿ�ж�ͨ��,���ȼ�����*/
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);	
}

/******************************************************************
�������ܣ�����һ�γ������Ĳ���
ע�������ɹ�����1��ʧ�ܷ���0
******************************************************************/
void Measure(void)
{
	static uint32_t lastTm;
	if(get_timer()-lastTm>100) //ǰ�����β���ʱ�������ô���100ms
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
		//�������½����ж�
		if(EXTI->FTSR&0x0800)
		{
		  Distance = TIM4->CNT*0.017;         //���������
		  //���½����ش�����Ϊ�����ش���
		  EXTI->RTSR |= (1<<11);
		  EXTI->FTSR &=~(1<<11);
			Data_New=1;
      On_Measure = 0;				
		}
		else
		{
			TIM4->CNT = 0x00;
		  //�������ش�����Ϊ�½��ش���
		  EXTI->RTSR &=~(1<<11);
		  EXTI->FTSR |= (1<<11);
		}
		EXTI_ClearITPendingBit(EXTI_Line11);
	}    
}





