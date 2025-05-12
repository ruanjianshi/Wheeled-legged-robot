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
	//输出限幅
	U_a = __constrain(U_a,0.0f,Voltage_limit);
	U_b = __constrain(U_b,0.0f,Voltage_limit);
	U_c = __constrain(U_c,0.0f,Voltage_limit);
	//占空比计算+输出限幅
	u_a_dutyfactor = __constrain(U_a / Voltage_powerSupply,0.0f,1.0f);
	u_b_dutyfactor = __constrain(U_b / Voltage_powerSupply,0.0f,1.0f);
	u_c_dutyfactor = __constrain(U_c / Voltage_powerSupply,0.0f,1.0f);
	//PWM赋值
	//PWM_SetDutyfactor(u_a_dutyfactor*(GENERAL_TIM_Period_PWM + 1),0,u_c_dutyfactor*(GENERAL_TIM_Period_PWM + 1),u_b_dutyfactor*(GENERAL_TIM_Period_PWM + 1));
	//PWM_SetDutyfactor(0,0,0,0);
	PWM_SetCompare2(u_a_dutyfactor*(GENERAL_TIM_Period_PWM + 1));
	PWM_SetCompare3(u_b_dutyfactor*(GENERAL_TIM_Period_PWM + 1));
	PWM_SetCompare4(u_c_dutyfactor*(GENERAL_TIM_Period_PWM + 1));
}
//力矩环
// 设置电机的力矩，通过给定的q轴电流分量、b轴电流分量和电气角度来计算并输出PWM信号
void SetMoment(float U_q,float U_b,float electrical_angle)
{
 float u_a_dutyfactor = 0,u_b_dutyfactor = 0,u_c_dutyfactor = 0;
 float Ualpha = 0,Ubeta = 0;

 //力矩限幅
 // 对q轴电流分量进行限幅处理，确保其在电源电压的一半范围内
 U_q = __constrain(U_q,-Voltage_powerSupply/2,Voltage_powerSupply/2);
 /*float Ud=0;*/
 // 标准化电气角度，使其在0到2π之间
 electrical_angle = Normalize_angle(electrical_angle);
 //帕克逆变换
 // 将q轴和d轴电流分量转换为αβ坐标系
 Ualpha = -U_q * sin(electrical_angle);
 Ubeta  = U_q * cos(electrical_angle); 
 //克拉克逆变换
 // 将αβ坐标系下的电流分量转换为三相PWM占空比
 u_a_dutyfactor = Ualpha + Voltage_powerSupply / 2;
 u_b_dutyfactor = (sqrt(3) * Ubeta-Ualpha) / 2 + Voltage_powerSupply / 2;
 u_c_dutyfactor = (-Ualpha-sqrt(3) * Ubeta) / 2 + Voltage_powerSupply / 2;
 
 // 输出PWM信号到电机驱动器
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

//开始计算时间的初始化SysTick_Computation_timeBegin()
float Calculate_Velocity(SoftI2C_HandleTypeDef *IICWHERE)
{
	float Ts, vel=0.0;
	static float angle_prev_Velocity = 0;				//当前角度(用于速度环)
	static float vel_angle_prev_Velocity = 0;			//上次角度(用于速度环)
	static long angle_prev_ts = 0;			//当前的运行时间
	static long vel_angle_prev_ts = 0;		//上次的运行时间

	//SysTick_Computation_timeBegin();
	//计算采样时间
	angle_prev_ts = SysTick_Computation_time();
	//记录当前角度
	angle_prev_Velocity = Get_Angle_Radian_Accumulation(IICWHERE);
	//SysTick_Computation_timeEnd();
	if(angle_prev_ts<vel_angle_prev_ts)Ts = (float)(vel_angle_prev_ts - angle_prev_ts)/SYSTICK_CLK_HZ;
	else
		Ts = (float)(0xFFFFFF - angle_prev_ts + vel_angle_prev_ts)/SYSTICK_CLK_HZ;
	//快速修复微小溢出
	if(Ts == 0 || Ts > 0.5) Ts = 1e-3f;
	//变量存储
	vel = (angle_prev_Velocity - vel_angle_prev_Velocity) / Ts;
	vel_angle_prev_Velocity = angle_prev_Velocity;
	vel_angle_prev_ts = angle_prev_ts;
	return vel;
}

float Calculate_Velocity_Optimized(SoftI2C_HandleTypeDef *IICWHERE)
{
	// 全局变量用于保存状态（需处理多线程安全）
	static uint32_t prev_time = 0;
	static float prev_angle = 0.0f;
	SysTick_Computation_timeBegin();
    // 1. 获取当前时间和角度
    uint32_t curr_time = SysTick_Computation_time();
    float curr_angle = Get_Angle_Radian_Accumulation(IICWHERE);
	SysTick_Computation_timeEnd();
    // 3. 计算时间差（考虑24位计数器溢出）
    uint32_t dt_ticks = 0xFFFFFF - curr_time;   // 自动处理无符号溢出
    float Ts = TICKS_TO_SEC((float)dt_ticks);
	//快速修复微小溢出
	//if(Ts == 0 || Ts > 0.5) Ts = 1e-6f;
    // 4. 约束时间差范围（避免除零或噪声）
    Ts = __constrain(Ts, 0.000001f, 1.0f); // 时间范围: 0.001ms ~ 1s
    // 5. 计算角速度（单位：rad/s）
    float delta_angle = curr_angle - prev_angle;
    float vel = delta_angle / Ts;
    // 6. 更新历史值
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



