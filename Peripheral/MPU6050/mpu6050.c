#include "mpu6050.h"
uint8_t ID;								//�������ڴ��ID�ŵı���
int16_t AX, AY, AZ, GX, GY, GZ;			//�������ڴ�Ÿ������ݵı���

/**
  * ��    ����MPU6050д�Ĵ���
  * ��    ����RegAddress �Ĵ�����ַ����Χ���ο�MPU6050�ֲ�ļĴ�������
  * ��    ����Data Ҫд��Ĵ��������ݣ���Χ��0x00~0xFF
  * �� �� ֵ����
  */
static void MPU6050_WriteReg(SoftI2C_HandleTypeDef *IICWHERE,uint8_t RegAddress, uint8_t Data)
{
	i2c_Start(IICWHERE);						//I2C��ʼ
	i2c_SendByte(IICWHERE,MPU6050_ADDRESS);	//���ʹӻ���ַ����дλΪ0����ʾ����д��
	while(i2c_WaitAck(IICWHERE));				//����Ӧ��
	i2c_SendByte(IICWHERE,RegAddress);			//���ͼĴ�����ַ
	while(i2c_WaitAck(IICWHERE));				//����Ӧ��
	i2c_SendByte(IICWHERE,Data);				//����Ҫд��Ĵ���������
	while(i2c_WaitAck(IICWHERE));				//����Ӧ��
	i2c_Stop(IICWHERE);						//I2C��ֹ
}

/**
  * ��    ����MPU6050���Ĵ���
  * ��    ����RegAddress �Ĵ�����ַ����Χ���ο�MPU6050�ֲ�ļĴ�������
  * �� �� ֵ����ȡ�Ĵ��������ݣ���Χ��0x00~0xFF
  */
static uint8_t MPU6050_ReadReg(SoftI2C_HandleTypeDef *IICWHERE,uint8_t RegAddress)
{
	uint8_t Data;
	
	i2c_Start(IICWHERE);						//I2C��ʼ
	i2c_SendByte(IICWHERE,MPU6050_ADDRESS);	//���ʹӻ���ַ����дλΪ0����ʾ����д��
	while(i2c_WaitAck(IICWHERE));				//����Ӧ��
	i2c_SendByte(IICWHERE,RegAddress);			//���ͼĴ�����ַ
	while(i2c_WaitAck(IICWHERE));				//����Ӧ��
	i2c_Stop(IICWHERE);						//I2C��ֹ
	
	i2c_Start(IICWHERE);						//I2C�ظ���ʼ
	i2c_SendByte(IICWHERE,MPU6050_ADDRESS | 0x01);	//���ʹӻ���ַ����дλΪ1����ʾ������ȡ
	while(i2c_WaitAck(IICWHERE));				//����Ӧ��
	Data = i2c_ReadByte_no(IICWHERE);			//����ָ���Ĵ���������
	i2c_Ack(IICWHERE);					//����Ӧ�𣬸��ӻ���Ӧ����ֹ�ӻ����������
	i2c_Stop(IICWHERE);						//I2C��ֹ
	
	return Data;
}

/**
  * ��    ����MPU6050��ʼ��
  * ��    ������
  * �� �� ֵ����
  */
void MPU6050_Init(SoftI2C_HandleTypeDef *IICWHERE)
{
	i2c_CfgGpio(IICWHERE);
	/*MPU6050�Ĵ�����ʼ������Ҫ����MPU6050�ֲ�ļĴ����������ã��˴��������˲�����Ҫ�ļĴ���*/
	MPU6050_WriteReg(IICWHERE,MPU6050_PWR_MGMT_1, 0x01);		//��Դ����Ĵ���1��ȡ��˯��ģʽ��ѡ��ʱ��ԴΪX��������
	MPU6050_WriteReg(IICWHERE,MPU6050_PWR_MGMT_2, 0x00);		//��Դ����Ĵ���2������Ĭ��ֵ0���������������
	MPU6050_WriteReg(IICWHERE,MPU6050_SMPLRT_DIV, 0x09);		//�����ʷ�Ƶ�Ĵ��������ò�����
	MPU6050_WriteReg(IICWHERE,MPU6050_CONFIG, 0x06);			//���üĴ���������DLPF
	MPU6050_WriteReg(IICWHERE,MPU6050_GYRO_CONFIG, 0x18);	//���������üĴ�����ѡ��������Ϊ��2000��/s
	MPU6050_WriteReg(IICWHERE,MPU6050_ACCEL_CONFIG, 0x18);	//���ٶȼ����üĴ�����ѡ��������Ϊ��16g
}

/**
  * ��    ����MPU6050��ȡID��
  * ��    ������
  * �� �� ֵ��MPU6050��ID��
  */
uint8_t MPU6050_GetID(SoftI2C_HandleTypeDef *IICWHERE)
{
	return MPU6050_ReadReg(IICWHERE,MPU6050_WHO_AM_I);		//����WHO_AM_I�Ĵ�����ֵ
}

/**
  * ��    ����MPU6050��ȡ����
  * ��    ����AccX AccY AccZ ���ٶȼ�X��Y��Z������ݣ�ʹ�������������ʽ���أ���Χ��-32768~32767
  * ��    ����GyroX GyroY GyroZ ������X��Y��Z������ݣ�ʹ�������������ʽ���أ���Χ��-32768~32767
  * �� �� ֵ����
  */
void MPU6050_GetData(SoftI2C_HandleTypeDef *IICWHERE,int16_t *AccX, int16_t *AccY, int16_t *AccZ, 
						int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ)
{
	uint8_t DataH, DataL;								//�������ݸ�8λ�͵�8λ�ı���
	
	DataH = MPU6050_ReadReg(IICWHERE,MPU6050_ACCEL_XOUT_H);		//��ȡ���ٶȼ�X��ĸ�8λ����
	DataL = MPU6050_ReadReg(IICWHERE,MPU6050_ACCEL_XOUT_L);		//��ȡ���ٶȼ�X��ĵ�8λ����
	*AccX = (DataH << 8) | DataL;						//����ƴ�ӣ�ͨ�������������
	
	DataH = MPU6050_ReadReg(IICWHERE,MPU6050_ACCEL_YOUT_H);		//��ȡ���ٶȼ�Y��ĸ�8λ����
	DataL = MPU6050_ReadReg(IICWHERE,MPU6050_ACCEL_YOUT_L);		//��ȡ���ٶȼ�Y��ĵ�8λ����
	*AccY = (DataH << 8) | DataL;						//����ƴ�ӣ�ͨ�������������
	
	DataH = MPU6050_ReadReg(IICWHERE,MPU6050_ACCEL_ZOUT_H);		//��ȡ���ٶȼ�Z��ĸ�8λ����
	DataL = MPU6050_ReadReg(IICWHERE,MPU6050_ACCEL_ZOUT_L);		//��ȡ���ٶȼ�Z��ĵ�8λ����
	*AccZ = (DataH << 8) | DataL;						//����ƴ�ӣ�ͨ�������������
	
	DataH = MPU6050_ReadReg(IICWHERE,MPU6050_GYRO_XOUT_H);		//��ȡ������X��ĸ�8λ����
	DataL = MPU6050_ReadReg(IICWHERE,MPU6050_GYRO_XOUT_L);		//��ȡ������X��ĵ�8λ����
	*GyroX = (DataH << 8) | DataL;						//����ƴ�ӣ�ͨ�������������
	
	DataH = MPU6050_ReadReg(IICWHERE,MPU6050_GYRO_YOUT_H);		//��ȡ������Y��ĸ�8λ����
	DataL = MPU6050_ReadReg(IICWHERE,MPU6050_GYRO_YOUT_L);		//��ȡ������Y��ĵ�8λ����
	*GyroY = (DataH << 8) | DataL;						//����ƴ�ӣ�ͨ�������������
	
	DataH = MPU6050_ReadReg(IICWHERE,MPU6050_GYRO_ZOUT_H);		//��ȡ������Z��ĸ�8λ����
	DataL = MPU6050_ReadReg(IICWHERE,MPU6050_GYRO_ZOUT_L);		//��ȡ������Z��ĵ�8λ����
	*GyroZ = (DataH << 8) | DataL;						//����ƴ�ӣ�ͨ�������������
}
