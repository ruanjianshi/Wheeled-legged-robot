#ifndef __SERVO_H
#define __SERVO_H

#include "stm32f10x.h"
#include "timer.h"

void Servo_Init(void);
void Servo_SetAngle1(float Angle);
uint16_t ADVANCE_TIM_PULSE_Servo_SetAngle1(uint16_t Angle);
#endif



