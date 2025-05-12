#include "foc.h"
float zero_electric_angle_F = 0;


float Switch_electricalAngle(SoftI2C_HandleTypeDef *IICWHERE)
{
  return (float)((Correction_direction * Pole_pairs) * Get_Angle_Radian(IICWHERE) - Electrical_angel_zero);
}

//float Switch_electricalAngle_reference(float shaft_angle,int pole_pairs)
//{
//	return (shaft_angle * pole_pairs);
//}

static float Normalize_angle(float angle)
{
	float temp_angle = fmod(angle,2*PI);
	return temp_angle >= 0 ? temp_angle : (temp_angle+ 2*PI);
}

static float Normalize_Switch_electricalAngle(SoftI2C_HandleTypeDef *IICWHERE)
{
	//float Correct_electric_angle;
	//Correct_electric_angle = Switch_electricalAngle();
	return Normalize_angle((float)((Correction_direction * Pole_pairs) * Get_Angle_Radian(IICWHERE))) - zero_electric_angle_F;
}

//static float NoCorrect_Switch_electricalAngle(SoftI2C_HandleTypeDef *IICWHERE,float angle)
//{
//	float Correct_electric_angle = 0;
//	Correct_electric_angle = Switch_electricalAngle(IICWHERE);
//	return Normalize_angle((float)(Correction_direction * Pole_pairs) * angle - Correct_electric_angle);
//}

void OutputPWM(float U_a,float U_b,float U_c)
{
	float u_a_dutyfactor = 0,u_b_dutyfactor = 0,u_c_dutyfactor = 0;
	//����޷�
	U_a = __constrain(U_a,0.0f,Voltage_limit);
	U_b = __constrain(U_b,0.0f,Voltage_limit);
	U_c = __constrain(U_c,0.0f,Voltage_limit);
	//ռ�ձȼ���+����޷�
	u_a_dutyfactor = __constrain(U_a / Voltage_powerSupply,0.0f,1.0f);
	u_b_dutyfactor = __constrain(U_b / Voltage_powerSupply,0.0f,1.0f);
	u_c_dutyfactor = __constrain(U_c / Voltage_powerSupply,0.0f,1.0f);
	//PWM��ֵ
	//PWM_SetDutyfactor(u_a_dutyfactor*(GENERAL_TIM_Period_PWM + 1),0,u_c_dutyfactor*(GENERAL_TIM_Period_PWM + 1),u_b_dutyfactor*(GENERAL_TIM_Period_PWM + 1));
	//PWM_SetDutyfactor(0,0,0,0);
	PWM_SetCompare2(u_a_dutyfactor*(GENERAL_TIM_Period_PWM + 1));
	PWM_SetCompare3(u_b_dutyfactor*(GENERAL_TIM_Period_PWM + 1));
	PWM_SetCompare4(u_c_dutyfactor*(GENERAL_TIM_Period_PWM + 1));
}
//���ػ�
// ���õ�������أ�ͨ��������q�����������b����������͵����Ƕ������㲢���PWM�ź�
void SetMoment(float U_q,float U_b,float electrical_angle)
{
 float u_a_dutyfactor = 0,u_b_dutyfactor = 0,u_c_dutyfactor = 0;
 float Ualpha = 0,Ubeta = 0;

 //�����޷�
 // ��q��������������޷�����ȷ�����ڵ�Դ��ѹ��һ�뷶Χ��
 U_q = __constrain(U_q,-Voltage_powerSupply/2,Voltage_powerSupply/2);
 /*float Ud=0;*/
 // ��׼�������Ƕȣ�ʹ����0��2��֮��
 electrical_angle = Normalize_angle(electrical_angle);
 //������任
 // ��q���d���������ת��Ϊ��������ϵ
 Ualpha = -U_q * sin(electrical_angle);
 Ubeta  = U_q * cos(electrical_angle); 
 //��������任
 // ����������ϵ�µĵ�������ת��Ϊ����PWMռ�ձ�
 u_a_dutyfactor = Ualpha + Voltage_powerSupply / 2;
 u_b_dutyfactor = (sqrt(3) * Ubeta-Ualpha) / 2 + Voltage_powerSupply / 2;
 u_c_dutyfactor = (-Ualpha-sqrt(3) * Ubeta) / 2 + Voltage_powerSupply / 2;
 
 // ���PWM�źŵ����������
 OutputPWM(u_a_dutyfactor,u_b_dutyfactor,u_c_dutyfactor);
}

void FOC_Init(SoftI2C_HandleTypeDef *IICWHERE)
{
	GENERAL_TIM_Init_PWM();
	PWM_SetDutyfactor(0,0,0,0);
	//SetMoment(3,0,Normalize_angle((float)(Correction_direction * Pole_pairs) * 30));
  //Delay_ms(3000);
	PID_Init();
	SetMoment(3,0, __3PI_2);
  Delay_ms(2000);
  zero_electric_angle_F = Normalize_Switch_electricalAngle(IICWHERE);
  SetMoment(0,0, __3PI_2);
}

//��ʼ����ʱ��ĳ�ʼ��SysTick_Computation_timeBegin()
float Calculate_Velocity(SoftI2C_HandleTypeDef *IICWHERE)
{
	float Ts, vel=0.0;
	static float angle_prev_Velocity = 0;				//��ǰ�Ƕ�(�����ٶȻ�)
	static float vel_angle_prev_Velocity = 0;			//�ϴνǶ�(�����ٶȻ�)
	static long angle_prev_ts = 0;			//��ǰ������ʱ��
	static long vel_angle_prev_ts = 0;		//�ϴε�����ʱ��

	//SysTick_Computation_timeBegin();
	//�������ʱ��
	angle_prev_ts = SysTick_Computation_time();
	//��¼��ǰ�Ƕ�
	angle_prev_Velocity = Get_Angle_Radian_Accumulation(IICWHERE);
	//SysTick_Computation_timeEnd();
	if(angle_prev_ts<vel_angle_prev_ts)Ts = (float)(vel_angle_prev_ts - angle_prev_ts)/SYSTICK_CLK_HZ;
	else
		Ts = (float)(0xFFFFFF - angle_prev_ts + vel_angle_prev_ts)/SYSTICK_CLK_HZ;
	//�����޸�΢С���
	if(Ts == 0 || Ts > 0.5) Ts = 1e-3f;
	//�����洢
	vel = (angle_prev_Velocity - vel_angle_prev_Velocity) / Ts;
	vel_angle_prev_Velocity = angle_prev_Velocity;
	vel_angle_prev_ts = angle_prev_ts;
	return vel;
}

float Calculate_Velocity_Optimized(SoftI2C_HandleTypeDef *IICWHERE)
{
	// ȫ�ֱ������ڱ���״̬���账����̰߳�ȫ��
	static uint32_t prev_time = 0;
	static float prev_angle = 0.0f;
	SysTick_Computation_timeBegin();
    // 1. ��ȡ��ǰʱ��ͽǶ�
    uint32_t curr_time = SysTick_Computation_time();
    float curr_angle = Get_Angle_Radian_Accumulation(IICWHERE);
	SysTick_Computation_timeEnd();
    // 3. ����ʱ������24λ�����������
    uint32_t dt_ticks = 0xFFFFFF - curr_time;   // �Զ������޷������
    float Ts = TICKS_TO_SEC((float)dt_ticks);
	//�����޸�΢С���
	//if(Ts == 0 || Ts > 0.5) Ts = 1e-6f;
    // 4. Լ��ʱ��Χ����������������
    Ts = __constrain(Ts, 0.000001f, 1.0f); // ʱ�䷶Χ: 0.001ms ~ 1s
    // 5. ������ٶȣ���λ��rad/s��
    float delta_angle = curr_angle - prev_angle;
    float vel = delta_angle / Ts;
    // 6. ������ʷֵ
    prev_time = curr_time;
    prev_angle = curr_angle;
    return vel;
}

float Calculate_time(void)
{
	SysTick_Computation_timeBegin();
	float temp = TICKS_TO_SEC(0xFFFFFF - SysTick_Computation_time()) * 1000;
	SysTick_Computation_timeEnd();
	return temp;
}


void SetAngle(SoftI2C_HandleTypeDef *IICWHERE,float angle)
{
	float As5600_angle = Get_Filtered_Angle(IICWHERE);
	float Angle_output = pid_siteControl(angle - (Sensor_direction * As5600_angle)*180/PI);
	Angle_output = __constrain(Angle_output,-6,6);
	SetMoment(Angle_output,0,Switch_electricalAngle(IICWHERE));
	printf("%.2f,%.2f\n",As5600_angle*180/PI,Angle_target);
}


void SetSpeed(SoftI2C_HandleTypeDef *IICWHERE,float speed)
{
	float As5600_speed = Calculate_Velocity(IICWHERE);
	float Speed_output = PID_speedControl(speed - Speed_sensor_direction * As5600_speed);
	Speed_output = __constrain(Speed_output,-6,6);
	SetMoment(Speed_output,0,Switch_electricalAngle(IICWHERE));
	printf("%.2f,%.2f\n",As5600_speed,Speed_target);
	
}



