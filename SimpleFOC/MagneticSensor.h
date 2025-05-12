#ifndef MAGNETICSENSOR_LIB_H
#define MAGNETICSENSOR_LIB_H

/******************************************************************************/
extern long  cpr;
extern float full_rotation_offset;
extern long  angle_data_prev;
extern unsigned long velocity_calc_timestamp;
extern float angle_prev;
/******************************************************************************/
void MagneticSensor_Init(void);
void MagneticSensor_Init_soft(SoftI2C_HandleTypeDef *IICWHERE);
float getAngle(void);
float getAngle_soft(SoftI2C_HandleTypeDef *IICWHERE);
float getVelocity(SoftI2C_HandleTypeDef *IICWHERE);
/******************************************************************************/

#endif
