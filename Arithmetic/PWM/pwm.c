#include "pwm.h"
#include <stm32f10x_tim.h>


uint16_t PWM_SetDutyfactor_1(uint16_t dutyfactor)
{
	return dutyfactor;
}

uint16_t PWM_SetDutyfactor_2(uint16_t dutyfactor)
{
	return dutyfactor;
}

uint16_t PWM_SetDutyfactor_3(uint16_t dutyfactor)
{
	return dutyfactor;
}

uint16_t PWM_SetDutyfactor_4(uint16_t dutyfactor)
{
	return dutyfactor;
}

void PWM_SetDutyfactor(uint16_t dutyfactor_1,uint16_t dutyfactor_2,uint16_t dutyfactor_3,uint16_t dutyfactor_4)
{
  TIM_OCInitTypeDef  TIM_OCInitStructure;
/*--------------------PWM����ȽϽṹ���ʼ��-------------------*/	

	// ����ΪPWMģʽ1
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	// ���ʹ��
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	// ���ͨ����ƽ��������	
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	
	// ����Ƚ�ͨ�� 1
	TIM_OCInitStructure.TIM_Pulse = dutyfactor_1;
	TIM_OC1Init(GENERAL_TIM_PWM, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(GENERAL_TIM_PWM, TIM_OCPreload_Enable);
	
	// ����Ƚ�ͨ�� 2
	TIM_OCInitStructure.TIM_Pulse = dutyfactor_2;
	TIM_OC2Init(GENERAL_TIM_PWM, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(GENERAL_TIM_PWM, TIM_OCPreload_Enable);
	
	// ����Ƚ�ͨ�� 3
	TIM_OCInitStructure.TIM_Pulse = dutyfactor_3;
	TIM_OC3Init(GENERAL_TIM_PWM, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(GENERAL_TIM_PWM, TIM_OCPreload_Enable);
	
	// ����Ƚ�ͨ�� 4
	TIM_OCInitStructure.TIM_Pulse = dutyfactor_4;
	TIM_OC4Init(GENERAL_TIM_PWM, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(GENERAL_TIM_PWM, TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(GENERAL_TIM_PWM,ENABLE);//TIM1��ARR��Ԥװ�ؼĴ���ʹ��
	// ʹ�ܼ�����
	TIM_Cmd(GENERAL_TIM_PWM, ENABLE);

}

void PWM_SetCompare1(uint16_t Compare)//д��GENERAL_TIM_PWMͨ��1��CCR(ռ�ձ�)
{
	TIM_SetCompare1(GENERAL_TIM_PWM,Compare);
}
void PWM_SetCompare2(uint16_t Compare)//д��GENERAL_TIM_PWMͨ��2��CCR(ռ�ձ�)
{
	TIM_SetCompare2(GENERAL_TIM_PWM,Compare);
}
void PWM_SetCompare3(uint16_t Compare)//д��GENERAL_TIM_PWMͨ��3��CCR(ռ�ձ�)
{
	TIM_SetCompare3(GENERAL_TIM_PWM,Compare);
}
void PWM_SetCompare4(uint16_t Compare)//д��GENERAL_TIM_PWMͨ��4��CCR(ռ�ձ�)
{
	TIM_SetCompare4(GENERAL_TIM_PWM,Compare);
}

/**********************************************************************************/

/**
  * ��    ����PWM����CCR
  * ��    ����Compare Ҫд���CCR��ֵ����Χ��0~100
  * �� �� ֵ����
  * ע�����CCR��ARR��ͬ����ռ�ձȣ��˺���������CCR��ֵ������ֱ����ռ�ձ�
  *           ռ�ձ�Duty = CCR / (ARR + 1)
  */
void Advance_PWM_SetCompare1(uint16_t Compare)
{
	TIM_SetCompare1(ADVANCE_TIM, Compare);		//����CCR2��ֵ
}


void Advance_PWM_SetCompare2(uint16_t Compare)
{
	TIM_SetCompare1(ADVANCE_TIM, Compare);		//����CCR2��ֵ
}



