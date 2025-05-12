#ifndef __PID_H
#define __PID_H

#include "stm32f10x.h"
extern float C_Kp_site;
extern float C_Ki_site;
extern float C_Kd_site;

#define Kp_site C_Kp_site
#define Ki_site C_Ki_site
#define Kd_site C_Kd_site
#define Kp_speed 0.11
#define Ki_speed 0.005
#define kd_speed 0.0008
#define Voltage_value_zero  0
#define Integral_value_zero 0


typedef struct//����pid�ṹ��
{
	float Target_value;//����Ŀ��ֵ
	float Actual_value;//������ʵֵ
	float Err_value;//����ƫ��ֵ
	float Err_last_value;//������һ��ƫ��ֵ
	float Kp,Ki,Kd;//������������֣�΢��
	float Voltage_value;//�����ѹֵ
	float Integral_value;//�������ֵ
}PID;


void PID_Init(void);
float pid_siteControl(float angle_Err_value);
float PID_speedControl(float speed_Err_value);
#endif


