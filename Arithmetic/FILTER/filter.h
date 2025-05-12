#ifndef __FILTER_H
#define __FILTER_H

#include "stm32f10x.h"
#include "foc.h"
// 低通滤波处理原始角度
#define FILTER_COEFF 0.2f  // 调整系数（0~1，越大滤波越强）


float LowPass_Filter(float x);
float Get_LowPass_Filter_Speed(SoftI2C_HandleTypeDef *IICWHERE);
float Get_Filtered_Angle(SoftI2C_HandleTypeDef *IICWHERE);
void kalman_filter(float z, float u);
#endif

