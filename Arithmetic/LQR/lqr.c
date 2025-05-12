#include "lqr.h"
// 循环下采样计数器
long loop_count = 0;
float target_voltage;

float adjust_ang= 60;
float adjust_p_vel= 3;
float adjust_motor_vel= 0.18;
float adjust_total_ang= 0;   //微调角度

// 初始化电机编码器硬件
// 设置要使用的控制回路类型
// 将电机连接到编码器
// 把电机连接到驱动器上
// 初始化运动
// 校准编码器并启动FOC

void loop() {
   // 控制回路每次~25ms
  if(loop_count++ > 25){
    // 倒立摆传感器读取

    // 计算摆角
    float pendulum_angle = constrainAngle(pendulum.getAngle()-3.83-adjust_total_ang + M_PI);
    Serial.println(pendulum.getAngle());
    //Serial.println(sensor0.getAngle());
    if( abs(pendulum_angle) < 0.5){ // 如果角度足够小稳定
      target_voltage = controllerLQR(pendulum_angle, pendulum.getVelocity(), motor.shaftVelocity());}
    else{ // 倒立摆
      //Serial.println("swing-up");
       // 设置100%的最大电压到电机，以便摆动
      //target_voltage = -_sign(pendulum.getVelocity())*motor.voltage_limit*1;
      target_voltage = -_sign(pendulum.getVelocity())*16.8;
      //Serial.println(target_voltage);
      //target_voltage=0;
    }
     // 将目标电压设置到电机上
    motor.move(target_voltage);
    // 重置计数器
    loop_count=0;
  }
   

}

// 函数限制-和之间的夹角，以-180度和180度表示
float constrainAngle(float x){
    x = fmod(x + M_PI, _2PI);
    if (x < 0)
        x += _2PI;
    return x - M_PI;
}

// LQR稳定控制器功能
// 计算需要设置电机的电压，以稳定摆
float controllerLQR(float p_angle, float p_vel, float m_vel){
  // 如果角度可控
  // 计算控制律 
  // LQR controller u = k*x
  //  - k = [40, 7, 0.3]
  //  - x = [摆角，摆速度，电机速度]' 
  float u =  adjust_ang*p_angle + adjust_p_vel*p_vel + adjust_motor_vel*m_vel;
  // 限制设定给电机的电压
  //if(abs(u) > motor.voltage_limit*1) u = _sign(u)*motor.voltage_limit*1;
  if(abs(u) > voltage_power_supply*0.7) u = _sign(u)*voltage_power_supply*0.7;
  return u;
}