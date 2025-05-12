#include "stm32f10x.h"
#include "led.h"
#include "systick.h"
#include "usart.h"
#include "key.h"
#include "timer.h"
#include "vofa_JUSTfloat.h"
#include "iic.h"
#include "oled_show.h"
#include "as5600.h"
#include "pwm.h"
#include "foc.h"
#include "pid.h"
#include "mpu6050.h"
#include "sys.h"
#include "delay.h"
#include "servo.h"
#include "FreeRTOSHandle.h"
#include "bluetooth.h"
#include "MyProject.h"
#include "mbotLinuxusart.h"
/******************************************************�궨����****************************************************************/
// GPIOx(A,B,C)����ַ
#define GPIOB_ODR_Addr_bit (GPIOB_BASE + 0X0C)
#define GPIOA_IDR_Addr_bit (GPIOA_BASE + 0X08)
#define GPIOC_IDR_Addr_bit (GPIOC_BASE + 0X08)

// λ�����������ܹ�ʽ
#define PBout_bit(bitnum) *(unsigned int *)((GPIOB_ODR_Addr_bit & 0xF0000000) + 0x02000000 + ((GPIOB_ODR_Addr_bit & 0x00FFFFFF) << 5) + (bitnum << 2))
#define PAin_bit(bitnum) *(unsigned int *)((GPIOA_IDR_Addr_bit & 0xF0000000) + 0x02000000 + ((GPIOA_IDR_Addr_bit & 0x00FFFFFF) << 5) + (bitnum << 2))
#define PCin_bit(bitnum) *(unsigned int *)((GPIOC_IDR_Addr_bit & 0xF0000000) + 0x02000000 + ((GPIOC_IDR_Addr_bit & 0x00FFFFFF) << 5) + (bitnum << 2))

/******************************************************�궨����****************************************************************/
extern SoftI2C_HandleTypeDef *OLED_iic;
extern SoftI2C_HandleTypeDef *MPU6050_iic;
extern SoftI2C_HandleTypeDef *AS5600_iicLeft;
extern SoftI2C_HandleTypeDef *AS5600_iicRight;
/*��ʱ����ʱ*/
volatile uint16_t base_time = 0;
volatile uint16_t general_time = 0;
extern unsigned char BMP1[];
// u8 test[4] = {1,2,9,4};
// u8 ts = 8;
extern uint8_t ID;					   // �������ڴ��ID�ŵı���
extern int16_t AX, AY, AZ, GX, GY, GZ; // �������ڴ�Ÿ������ݵı���
int16_t receive_mpu6050[6] = {0};
uint8_t IDd; // �������ڴ��ID�ŵı���
float target;
extern long  angle_data_prev;

void vofa_send(int16_t *AccX, int16_t *AccY, int16_t *AccZ,
			   int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ)
{
	//	receive_mpu6050[0] = *AccX/32768*16;
	//	receive_mpu6050[1] = *AccY/32768*16;
	//	receive_mpu6050[2] = *AccZ/32768*16;
	//	receive_mpu6050[3] = *GyroX/32768*2000;
	//	receive_mpu6050[4] = *GyroY/32768*2000;
	//	receive_mpu6050[5] = *GyroZ/32768*2000;
	//	JustFloatTONGXIN_Array_uint16_t(receive_mpu6050,6);
	receive_mpu6050[0] = *AccX;
	receive_mpu6050[1] = *AccY;
	receive_mpu6050[2] = *AccZ;
	receive_mpu6050[3] = *GyroX;
	receive_mpu6050[4] = *GyroY;
	receive_mpu6050[5] = *GyroZ;
	JustFloatTONGXIN_Array_uint16_t(receive_mpu6050, 6);
}
void FOC_INIT_ALL(void)
{
	USART_Config();
	OLED_Init_C8T6();
	TIM2_PWM_Init();
	AS5600_INIT(AS5600_iicLeft);
	SysTick_Config(72);
	MagneticSensor_Init_soft(AS5600_iicLeft);     //AS5600 or TLE5012B
	
	voltage_power_supply=12;   //V����Դ��ѹ
	pole_pairs=7;              //���������������ʵ�����ã���Ȼ�����ϵ��⵫��ʧ�ܵĸ���
	voltage_limit=6;           //V�����ֵ��С��12/1.732=6.9
	velocity_limit=20;         //rad/s angleOpenloop() and PID_angle() use it
	voltage_sensor_align=2.5;  //V  ��Ҫ��������ģ�������0.5-1����̨���С����2-3
	torque_controller=Type_voltage;  //��ǰֻ�е�ѹģʽ
	controller=Type_angle;  //Type_angle; //Type_torque;    //Type_velocity
	target=10;
	
	Motor_init();
	//PWM_SetDutyfactor(0,0,0,0);
	Motor_initFOC(AS5600_iicLeft);
	PID_init();                //PID�������� ��init������
  printf("Motor ready.\r\n");
	
	SysTick_Computation_timeBegin();   //�����ٵ���delay_us()��delay_ms()����
}

int main(void)
{
#if IsLaunchFreeRTOS

	USART_Config();
	OLED_Init_C8T6();
  //OLED_Init(OLED_iic);
	
	//SysTick_Computation_timeBegin();
	
  MPU6050_Init(MPU6050_iic);
  IDd = MPU6050_GetID(MPU6050_iic); // ��ȡMPU6050��ID�� 
	
	AS5600_INIT(AS5600_iicRight);
	AS5600_INIT(AS5600_iicLeft);
	FOC_Init(AS5600_iicLeft);
	FOC_Init(AS5600_iicRight);

	bluetooth_init();
	
	
	printf("���ڲ���\n");
	SysTick_Init(72); /* �δ�ʱ����ʼ�������ж� */
	freertos_head();

#else
	// ϵͳ��ʱ���Ѿ����ó�72M��
	//USART_Config();
	//OLED_Init_C8T6();
	//GENERAL_TIM_Init_TIMING();
	//TIM2_PWM_Init();
	//AS5600_INIT(AS5600_iicLeft);
	// USART_Config();
	// LED_GPIO_Config();
	//SysTick_Config(72);
	// I2C_Config();
	// i2c_CfgGpio();
	//MPU6050_Init(MPU6050_iic);
	//OLED_Init(OLED_iic);
	// EXTI_Key_Config();
	// ADVANCE_TIM_Init_CDZ();

	// PWM_SetDutyfactor(2,0,4,2);
	//AS5600_INIT(AS5600_iicLeft);
	//FOC_Init(AS5600_iicLeft);
	// SysTick_Computation_timeBegin()
	//Servo_Init();
	// OLED_ShowString(OLED_iic,1, 1, "ID:");
	//ID = MPU6050_GetID(MPU6050_iic); // ��ȡMPU6050��ID��
	// OLED_ShowHexNum(OLED_iic,1, 4, ID, 2);		//OLED��ʾID��
	// OLED_CLS(OLED_iic);
	FOC_INIT_ALL();
	while (1)
	{
		ROS_USART_TEST();
		move(AS5600_iicLeft,target);
		loopFOC(AS5600_iicLeft);
		OLED_ShowNum_C8T6(1, 6, angle_data_prev, 4);
		OLED_ShowHexNum_C8T6(1, 3, IDd, 2);
		// Servo_SetAngle1(0.0);
		// delay_ms(2000);
		//Servo_SetAngle1(270.0);
		//MPU6050_GetData(MPU6050_iic, &AX, &AY, &AZ, &GX, &GY, &GZ);
		// printf("AX=%d,AY=%d,AZ=%d,GX=%d,GY=%d,GZ=%d\r\n",AX,AY,AZ,GX,GY,GZ);
		// vofa_send(&AX, &AY, &AZ, &GX, &GY, &GZ);
		// t =(u16)AS5600_ReadRawAngleTwo();
		// SetAngle(AS5600_iicLeft,Angle_target);
		// SetSpeed(Speed_target);
		// SetMoment(1,0,Switch_electricalAngle());
		// Delay_ms(20);

		// MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ);
		// printf("AX=%d,AY=%d,AZ=%d,GX=%d,GY=%d,GZ=%d\r\n",AX,AY,AZ,GX,GY,GZ);

		// if(general_time > 200)
		// {
		//	 general_time = 0;
		//	 printf("Angle_I2C=%.4f\r\n",Calculate_Velocity_Optimized());
		//	 //JustFloatTONGXIN_Array(&t,1);
		// }

		// printf("Angle_I2C=%.4f\r\n",Calculate_Velocity());
		// for (uint8_t i = 0; i < 3; i++)
		// {
		//   OLED_ShowCN(40 + i * 16, 0, i); // ������ʾ����
		// }
		// OLED_ShowString(OLED_iic,1, 1, "ID:");		//��ʾ��̬�ַ���
		// OLED_ShowStr(OLED_iic,0, 3, (unsigned char *)"MPU6050X:", OLED_8x16);  // ����6*8�ַ�
		// OLED_ShowStr(OLED_iic,0, 5, (unsigned char *)"MPU6050Y:", OLED_8x16);  // ����6*8�ַ�
		// OLED_ShowDynamic_FNum(OLED_iic,72,5,general_time/1000,OLED_8x16);
		// OLED_ShowDynamic_FNumInt(OLED_iic,72,7,ID,OLED_8x16);
//		OLED_ShowDynamic_FNum(OLED_iic, 72, 1, AX, OLED_8x16);
//		OLED_ShowDynamic_FNum(OLED_iic, 72, 3, AY, OLED_8x16);
//		OLED_ShowDynamic_FNum(OLED_iic, 72, 5, AZ, OLED_8x16);
//		OLED_ShowDynamic_FNum(OLED_iic, 0, 1, GX, OLED_8x16);
//		OLED_ShowDynamic_FNum(OLED_iic, 0, 3, GY, OLED_8x16);
//		OLED_ShowDynamic_FNum(OLED_iic, 0, 5, GZ, OLED_8x16);
		// OLED_ShowStr(OLED_iic,0, 3, (unsigned char *)"MPU6050X:", OLED_8x16);
		// OLED_Refresh(OLED_iic,2);
		// OLED_CLS(OLED_iic);
	}
#endif
}
