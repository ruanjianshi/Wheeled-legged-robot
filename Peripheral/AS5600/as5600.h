#ifndef __AS5600_H
#define __AS5600_H

#include "stm32f10x.h"
#include "math.h"
#include "systick.h"
#include "iic.h"

#define  RAW_Angle_Hi    0x0C
#define  RAW_Angle_Lo    0x0D
#define  AS5600_Address  0x36

void AS5600_INIT(SoftI2C_HandleTypeDef *IICWHERE);
u8 AS5600_ReadOneByte(SoftI2C_HandleTypeDef *IICWHERE,u8 addr);
float AS5600_ReadRawAngleTwo(SoftI2C_HandleTypeDef *IICWHERE);
float Get_Angle_Degree(SoftI2C_HandleTypeDef *IICWHERE);
float Get_Angle_Radian(SoftI2C_HandleTypeDef *IICWHERE);
float Get_Angle_Radian_Accumulation(SoftI2C_HandleTypeDef *IICWHERE);
unsigned short noAS5600_ReadRawAngleTwo(SoftI2C_HandleTypeDef *IICWHERE);
#endif

