#ifndef _CSB_H
#define _CSB_H

#include "stm32f10x.h"
//#include "sys.h"

extern float Distance;

extern u8 On_Measure;         //1ÕıÔÚ²âÁ¿
extern u8 Data_New; 
void CSB_Init(void);
void Measure(void);

#endif

