#ifndef __CONTROL_H
#define __CONTROL_H

#include "stm32f10x.h"

void Stop(void);

void MotoEnablePin_Init(void);

void MotoCtrl(int left, int right);

void GoAhead(void);

void GoBack(void);

void TurnLeft(void);

void TurnRight(void);

void AutoRun(float distance);

void CtrlRun(float pitch, float roll, float yaw);

#endif
