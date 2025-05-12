#ifndef MYPROJECT_H
#define MYPROJECT_H


/* Includes ------------------------------------------------------------------*/

#include "stm32f10x_it.h" 
#include "usart.h"
#include "delay.h"
#include "timer.h"
#include "iic.h"
#include "pwm.h"
#include "as5600.h"

#include "MagneticSensor.h" 
#include "foc_utils.h" 
#include "FOCMotor.h" 
#include "BLDCmotor.h" 
#include "lowpass_filter.h" 
#include "FOCpid.h"


extern SoftI2C_HandleTypeDef *OLED_iic;
extern SoftI2C_HandleTypeDef *MPU6050_iic;
extern SoftI2C_HandleTypeDef *AS5600_iicLeft;
extern SoftI2C_HandleTypeDef *AS5600_iicRight;
/******************************************************************************/
#define M1_Enable    GPIO_SetBits(GPIOB,GPIO_Pin_9);          //高电平使能
#define M1_Disable   GPIO_ResetBits(GPIOB,GPIO_Pin_9);        //低电平失能
/******************************************************************************/
//编码器类型，二者只能选一。设置使用的编码器为1，不使用的为0
#define M1_AS5600    1
#define M1_TLE5012B  0
//本节代码只支持2种编码器，下一节代码支持更多编码器
/******************************************************************************/
#define I2C_soft 1

#endif

