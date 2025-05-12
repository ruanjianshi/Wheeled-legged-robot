#include "lqr.h"
// ѭ���²���������
long loop_count = 0;
float target_voltage;

float adjust_ang= 60;
float adjust_p_vel= 3;
float adjust_motor_vel= 0.18;
float adjust_total_ang= 0;   //΢���Ƕ�

// ��ʼ�����������Ӳ��
// ����Ҫʹ�õĿ��ƻ�·����
// ��������ӵ�������
// �ѵ�����ӵ���������
// ��ʼ���˶�
// У׼������������FOC

void loop() {
   // ���ƻ�·ÿ��~25ms
  if(loop_count++ > 25){
    // �����ڴ�������ȡ

    // ����ڽ�
    float pendulum_angle = constrainAngle(pendulum.getAngle()-3.83-adjust_total_ang + M_PI);
    Serial.println(pendulum.getAngle());
    //Serial.println(sensor0.getAngle());
    if( abs(pendulum_angle) < 0.5){ // ����Ƕ��㹻С�ȶ�
      target_voltage = controllerLQR(pendulum_angle, pendulum.getVelocity(), motor.shaftVelocity());}
    else{ // ������
      //Serial.println("swing-up");
       // ����100%������ѹ��������Ա�ڶ�
      //target_voltage = -_sign(pendulum.getVelocity())*motor.voltage_limit*1;
      target_voltage = -_sign(pendulum.getVelocity())*16.8;
      //Serial.println(target_voltage);
      //target_voltage=0;
    }
     // ��Ŀ���ѹ���õ������
    motor.move(target_voltage);
    // ���ü�����
    loop_count=0;
  }
   

}

// ��������-��֮��ļнǣ���-180�Ⱥ�180�ȱ�ʾ
float constrainAngle(float x){
    x = fmod(x + M_PI, _2PI);
    if (x < 0)
        x += _2PI;
    return x - M_PI;
}

// LQR�ȶ�����������
// ������Ҫ���õ���ĵ�ѹ�����ȶ���
float controllerLQR(float p_angle, float p_vel, float m_vel){
  // ����Ƕȿɿ�
  // ��������� 
  // LQR controller u = k*x
  //  - k = [40, 7, 0.3]
  //  - x = [�ڽǣ����ٶȣ�����ٶ�]' 
  float u =  adjust_ang*p_angle + adjust_p_vel*p_vel + adjust_motor_vel*m_vel;
  // �����趨������ĵ�ѹ
  //if(abs(u) > motor.voltage_limit*1) u = _sign(u)*motor.voltage_limit*1;
  if(abs(u) > voltage_power_supply*0.7) u = _sign(u)*voltage_power_supply*0.7;
  return u;
}