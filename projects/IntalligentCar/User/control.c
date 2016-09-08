#include "control.h"
#include "pwm.h"

void MotoEnablePin_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);    

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIOB->BRR = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15; 
}

/*左右电机PWM控制，PWM范围-1000到1000*/
void MotoCtrl(int left, int right)
{
	uint16_t pwm3,pwm4;
	if(left >= 0)
	{
		GPIOB->BRR = GPIO_Pin_12;
		GPIOB->BSRR = GPIO_Pin_13;
		pwm3 = left;
	}
	else
	{
		GPIOB->BSRR = GPIO_Pin_12;
		GPIOB->BRR = GPIO_Pin_13;
		pwm3 = -left;
	}
	
	if(right >= 0)
	{
		GPIOB->BRR = GPIO_Pin_14;
		GPIOB->BSRR = GPIO_Pin_15;
		pwm4 = right;
	}
	else
	{
		GPIOB->BSRR = GPIO_Pin_14;
		GPIOB->BRR = GPIO_Pin_15;
		pwm4 = -right;
	}
	
	Moto_PwmUpdate(0, 0, pwm3, pwm4);
}

void GoAhead(void)
{
	MotoCtrl(450,450);
}

void GoBack(void)
{
	MotoCtrl(-450,-450);
}

void TurnLeft(void)
{
	MotoCtrl(-300,300);
}

void TurnRight(void)
{
	MotoCtrl(300,-300);
}

void Stop(void)
{
	MotoCtrl(0,0);
}

void AutoRun(float distance)
{
	if(distance < 30)
	{
		TurnLeft();
	}
	else
	{
		GoAhead();
	}
}

void CtrlRun(float pitch, float roll, float yaw)
{
	int left, right;
	left = pitch*20;
	right = pitch*20;
	left += roll*20;
	right -= roll*20;
	yaw = yaw;
	if(left > 1000) left = 1000;
	if(left < -1000) left = -1000;
	if(right > 1000) right = 1000;
	if(right < -1000) right = -1000;
	MotoCtrl(left, right);
}
