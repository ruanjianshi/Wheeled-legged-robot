#include "MyProject.h"


/******************************************************************************/
float shaft_angle;//!< current motor angle
float electrical_angle;
float shaft_velocity;
float current_sp;
float shaft_velocity_sp;
float shaft_angle_sp;
DQVoltage_s voltage;
DQCurrent_s current;

TorqueControlType torque_controller;
MotionControlType controller;

float sensor_offset=0;
float zero_electric_angle;
/******************************************************************************/
// shaft angle calculation
float shaftAngle(SoftI2C_HandleTypeDef *IICWHERE)
{
  // if no sensor linked return previous value ( for open loop )
  //if(!sensor) return shaft_angle;
  return sensor_direction*getAngle_soft(IICWHERE) - sensor_offset;
}
// shaft velocity calculation
float shaftVelocity(SoftI2C_HandleTypeDef *IICWHERE)
{
  // if no sensor linked return previous value ( for open loop )
  //if(!sensor) return shaft_velocity;
  return sensor_direction*LPF_velocity(getVelocity(IICWHERE));
}
/******************************************************************************/
float electricalAngle(void)
{
  return _normalizeAngle((shaft_angle + sensor_offset) * pole_pairs - zero_electric_angle);
}
/******************************************************************************/


