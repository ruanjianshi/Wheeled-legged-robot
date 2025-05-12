#include "pid.h"
PID pid_site;//定义位置环pid
PID pid_speed;//定义距离环pid


/**********************
PID配置函数:
**********************/
void PID_Init()//初始化pid变量
{
	//位置环pid
	pid_site.Target_value=0.0;
	pid_site.Actual_value=0.0;
	pid_site.Err_value=0.0;
	pid_site.Err_last_value=0.0;
	pid_site.Voltage_value=0.0;
	pid_site.Integral_value=0.0;
	pid_site.Kp=Kp_site;
	pid_site.Ki=Ki_site;
	pid_site.Kd=Kd_site;
	
	//速度环pid
	pid_speed.Target_value=0.0;
	pid_speed.Actual_value=0.0;
	pid_speed.Err_value=0.0;
	pid_speed.Err_last_value=0.0;
	pid_speed.Voltage_value=0.0;
	pid_speed.Integral_value=0.0;	
	pid_speed.Kp=Kp_speed;
	pid_speed.Ki=Ki_speed;
	pid_speed.Kd=kd_speed;
}


float pid_siteControl(float angle_Err_value)
{
	float angle_output;
	pid_site.Err_value=angle_Err_value;
	
	pid_site.Integral_value+=pid_site.Err_value;
	
	angle_output=pid_site.Kp * pid_site.Err_value + pid_site.Ki * pid_site.Integral_value + pid_site.Kd * (pid_site.Err_value-pid_site.Err_last_value);

	pid_site.Integral_value=pid_site.Integral_value>2000?2000:(pid_site.Integral_value<(-2000)?(-2000):pid_site.Integral_value);//积分限幅
	
	pid_site.Err_last_value=pid_site.Err_value;

	return angle_output;
}

float PID_speedControl(float speed_Err_value)
{
	float speed_output;
	pid_speed.Err_value=speed_Err_value;
	
	pid_speed.Integral_value+=pid_speed.Err_value;
	
	speed_output=pid_speed.Kp * pid_speed.Err_value + pid_speed.Ki * pid_speed.Integral_value + pid_speed.Kd * (pid_speed.Err_value-pid_speed.Err_last_value);

	pid_speed.Integral_value=pid_speed.Integral_value>3000?3000:(pid_speed.Integral_value<(-3000)?(-3000):pid_speed.Integral_value);//积分限幅
	
	pid_speed.Err_last_value=pid_speed.Err_value;

	return speed_output;
}


