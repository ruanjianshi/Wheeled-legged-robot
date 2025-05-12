#include "servo.h"
#include "pwm.h"


/**
  * 函    数：舵机初始化
  * 参    数：无
  * 返 回 值：无
  */
void Servo_Init(void)
{
	Advance_TIM_PWM_Init();									//初始化舵机的底层PWM
}

/**
  * 函    数：舵机设置角度
  * 参    数：Angle 要设置的舵机角度，范围：0~180
  * 返 回 值：无
  */
void Servo_SetAngle1(float Angle)
{
	Advance_PWM_SetCompare1(Angle / 270 * 2000 + 500);	//设置占空比
												//将角度线性变换，对应到舵机要求的占空比范围上
}

uint16_t ADVANCE_TIM_PULSE_Servo_SetAngle1(uint16_t Angle)
{
	return Angle / 270 * 2000 + 500;
}
