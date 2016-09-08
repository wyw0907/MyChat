#ifndef _pwm_H
#define _pwm_H

#include "stm32f10x.h"

void MotoPwm_Init(void);
void Moto_PwmUpdate(uint16_t MOTO1_PWM, uint16_t MOTO2_PWM, uint16_t MOTO3_PWM, uint16_t MOTO4_PWM);


#endif 

