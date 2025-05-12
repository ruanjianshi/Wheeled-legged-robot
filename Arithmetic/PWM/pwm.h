#ifndef __PWM_H
#define __PWM_H

#include "stm32f10x.h"
#include "timer.h"

// 占空比P=CCR/(ARR+1)
// 占空比配置
#define PWM_CCR1_Val_A6 20
#define PWM_CCR2_Val_A7 50
#define PWM_CCR3_Val_B0 40
#define PWM_CCR4_Val_B1 50


uint16_t PWM_SetDutyfactor_1(uint16_t dutyfactor);
uint16_t PWM_SetDutyfactor_2(uint16_t dutyfactor);
uint16_t PWM_SetDutyfactor_3(uint16_t dutyfactor);
uint16_t PWM_SetDutyfactor_4(uint16_t dutyfactor);
void PWM_SetDutyfactor(uint16_t dutyfactor_1,uint16_t dutyfactor_2,uint16_t dutyfactor_3,uint16_t dutyfactor_4);
void PWM_SetCompare1(uint16_t Compare);
void PWM_SetCompare2(uint16_t Compare);
void PWM_SetCompare3(uint16_t Compare);
void PWM_SetCompare4(uint16_t Compare);
/*********************************************/
void Advance_PWM_SetCompare1(uint16_t Compare);
void Advance_PWM_SetCompare2(uint16_t Compare);
void Advance_PWM_SetCompare2(uint16_t Compare);
#endif

