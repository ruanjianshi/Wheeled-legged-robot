#ifndef __SYSTICK_H
#define __SYSTICK_H


#include "stm32f10x.h"
#include "core_cm3.h"

#if 0
void SysTick_Delay_us(uint32_t us);
void SysTick_Delay_ms(uint32_t ms);
#endif
void SysTick_Computation_timeBegin(void);
uint32_t ysTick_Computation_time(void);
void SysTick_Computation_timeEnd(void);

void Delay_us(uint32_t us);
void Delay_ms(uint32_t ms);
void Delay_s(uint32_t s);
//软件延时，效果不准确，只用于模糊延时。
void Delay(uint32_t count);


#endif

