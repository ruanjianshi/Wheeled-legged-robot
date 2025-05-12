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
/*--------------------PWM输出比较结构体初始化-------------------*/	

	// 配置为PWM模式1
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	// 输出使能
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	// 输出通道电平极性配置	
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	
	// 输出比较通道 1
	TIM_OCInitStructure.TIM_Pulse = dutyfactor_1;
	TIM_OC1Init(GENERAL_TIM_PWM, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(GENERAL_TIM_PWM, TIM_OCPreload_Enable);
	
	// 输出比较通道 2
	TIM_OCInitStructure.TIM_Pulse = dutyfactor_2;
	TIM_OC2Init(GENERAL_TIM_PWM, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(GENERAL_TIM_PWM, TIM_OCPreload_Enable);
	
	// 输出比较通道 3
	TIM_OCInitStructure.TIM_Pulse = dutyfactor_3;
	TIM_OC3Init(GENERAL_TIM_PWM, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(GENERAL_TIM_PWM, TIM_OCPreload_Enable);
	
	// 输出比较通道 4
	TIM_OCInitStructure.TIM_Pulse = dutyfactor_4;
	TIM_OC4Init(GENERAL_TIM_PWM, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(GENERAL_TIM_PWM, TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(GENERAL_TIM_PWM,ENABLE);//TIM1在ARR上预装载寄存器使能
	// 使能计数器
	TIM_Cmd(GENERAL_TIM_PWM, ENABLE);

}

void PWM_SetCompare1(uint16_t Compare)//写入GENERAL_TIM_PWM通道1的CCR(占空比)
{
	TIM_SetCompare1(GENERAL_TIM_PWM,Compare);
}
void PWM_SetCompare2(uint16_t Compare)//写入GENERAL_TIM_PWM通道2的CCR(占空比)
{
	TIM_SetCompare2(GENERAL_TIM_PWM,Compare);
}
void PWM_SetCompare3(uint16_t Compare)//写入GENERAL_TIM_PWM通道3的CCR(占空比)
{
	TIM_SetCompare3(GENERAL_TIM_PWM,Compare);
}
void PWM_SetCompare4(uint16_t Compare)//写入GENERAL_TIM_PWM通道4的CCR(占空比)
{
	TIM_SetCompare4(GENERAL_TIM_PWM,Compare);
}

/**********************************************************************************/

/**
  * 函    数：PWM设置CCR
  * 参    数：Compare 要写入的CCR的值，范围：0~100
  * 返 回 值：无
  * 注意事项：CCR和ARR共同决定占空比，此函数仅设置CCR的值，并不直接是占空比
  *           占空比Duty = CCR / (ARR + 1)
  */
void Advance_PWM_SetCompare1(uint16_t Compare)
{
	TIM_SetCompare1(ADVANCE_TIM, Compare);		//设置CCR2的值
}


void Advance_PWM_SetCompare2(uint16_t Compare)
{
	TIM_SetCompare1(ADVANCE_TIM, Compare);		//设置CCR2的值
}



