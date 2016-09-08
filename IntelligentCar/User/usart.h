#ifndef _usart_H_
#define _usart_H_

#include "stm32f10x.h"
#include <stdio.h>

extern uint8_t rxbuf[64];
extern uint16_t rxptr;
void USART1_Init(uint32_t Baud);

#endif

