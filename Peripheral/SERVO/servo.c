#include "servo.h"
#include "pwm.h"


/**
  * ��    ���������ʼ��
  * ��    ������
  * �� �� ֵ����
  */
void Servo_Init(void)
{
	Advance_TIM_PWM_Init();									//��ʼ������ĵײ�PWM
}

/**
  * ��    ����������ýǶ�
  * ��    ����Angle Ҫ���õĶ���Ƕȣ���Χ��0~180
  * �� �� ֵ����
  */
void Servo_SetAngle1(float Angle)
{
	Advance_PWM_SetCompare1(Angle / 270 * 2000 + 500);	//����ռ�ձ�
												//���Ƕ����Ա任����Ӧ�����Ҫ���ռ�ձȷ�Χ��
}

uint16_t ADVANCE_TIM_PULSE_Servo_SetAngle1(uint16_t Angle)
{
	return Angle / 270 * 2000 + 500;
}
