#ifndef __FILTER_H
#define __FILTER_H

#include "stm32f10x.h"
#include "foc.h"
// ��ͨ�˲�����ԭʼ�Ƕ�
#define FILTER_COEFF 0.2f  // ����ϵ����0~1��Խ���˲�Խǿ��


float LowPass_Filter(float x);
float Get_LowPass_Filter_Speed(SoftI2C_HandleTypeDef *IICWHERE);
float Get_Filtered_Angle(SoftI2C_HandleTypeDef *IICWHERE);
void kalman_filter(float z, float u);
#endif

