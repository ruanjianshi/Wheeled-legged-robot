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


typedef struct//定义pid结构体
{
	float Target_value;//定义目标值
	float Actual_value;//定义真实值
	float Err_value;//定义偏差值
	float Err_last_value;//定义上一个偏差值
	float Kp,Ki,Kd;//定义比例，积分，微分
	float Voltage_value;//定义电压值
	float Integral_value;//定义积分值
}PID;


void PID_Init(void);
float pid_siteControl(float angle_Err_value);
float PID_speedControl(float speed_Err_value);
#endif


