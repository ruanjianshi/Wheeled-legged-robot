#ifndef __FOC_H
#define __FOC_H

#include "stm32f10x.h"
#include "as5600.h"
#include "timer.h"
#include "pid.h"
#include "systick.h"
#include "filter.h"
#include <string.h> 
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "core_cm3.h"
#include "systick.h"
#include "delay.h"

//1.     电角度      =   机械角度  *  极对数
//2.Electrical_angle =Mechanical_angle * Pole_pairs
#define Angle_target 180.0
#define Speed_target 5.0
#define PI_angle 180
#define __3PI_2 4.71238898038f
#define PI 3.14159265359
#define Voltage_limit 		 	10
#define Voltage_powerSupply 12.6	
#define Mechanical_angle_zero 0.0
#define Electrical_angel_zero 0.0
#define Ualpha_zero 0
#define Ubeta_zero  0
#define U_a_zero 0
#define U_b_zero 0
#define U_c_zero 0
#define U_q_zero 0
#define U_d_zero 0
#define u_a_dutyfactor_zero 0
#define u_b_dutyfactor_zero 0
#define u_c_dutyfactor_zero 0
#define Pole_pairs 7
#define Correction_direction 1
#define Sensor_direction 1
#define Speed_sensor_direction 1
#define __constrain(value,low,high) ((value)<(low)?(low):((value)>(high)?(high):(value)))
// 时间转换宏
#define SYSTICK_CLK_HZ      1000000  // 假设SysTick配置为1?s/tick
#define TICKS_TO_SEC(ticks) ((float)(ticks) / SYSTICK_CLK_HZ)
// 定义系统参数
#define SYSTICK_MAX_TICKS   0x00FFFFFF    // 24位最大值

float Calculate_time(void);
float Calculate_Velocity_Optimized(SoftI2C_HandleTypeDef *IICWHERE);
float Calculate_Velocity(SoftI2C_HandleTypeDef *IICWHERE);
void OutputPWM(float U_a,float U_b,float U_c);
void SetMoment(float U_q,float U_b,float electrical_angle);
void SetAngle(SoftI2C_HandleTypeDef *IICWHERE,float angle);
void SetSpeed(SoftI2C_HandleTypeDef *IICWHERE,float angle);
void FOC_Init(SoftI2C_HandleTypeDef *IICWHERE);
float Switch_electricalAngle(SoftI2C_HandleTypeDef *IICWHERE);
float Switch_electricalAngle_reference(float shaft_angle,int pole_pairs);
#endif

