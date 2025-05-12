#include "iic.h"
#include <stm32f10x_gpio.h>

SoftI2C_HandleTypeDef hi2c = {  
		.GPIO_PORT_I2C_SDA = GPIOB,
		.I2C_SDA_PIN = GPIO_Pin_9,
		.GPIO_PORT_I2C_SCL = GPIOB,
		.I2C_SCL_PIN = GPIO_Pin_8,
		.RCC_I2C_PORT = RCC_APB2Periph_GPIOB
};

// 1. ����һ��ָ��ָ��ýṹ��--OLED
SoftI2C_HandleTypeDef *OLED_iic = &hi2c;


SoftI2C_HandleTypeDef Mhi2c = {  
		.GPIO_PORT_I2C_SDA = GPIOB,
		.I2C_SDA_PIN = GPIO_Pin_7,
		.GPIO_PORT_I2C_SCL = GPIOB,
		.I2C_SCL_PIN = GPIO_Pin_6,
		.RCC_I2C_PORT = RCC_APB2Periph_GPIOB
};

// 2. ����һ��ָ��ָ��ýṹ��--MPU6050
SoftI2C_HandleTypeDef *MPU6050_iic = &Mhi2c;
	
SoftI2C_HandleTypeDef Ahi2cL = {  
		.GPIO_PORT_I2C_SDA = GPIOA,
		.I2C_SDA_PIN = GPIO_Pin_4,
		.GPIO_PORT_I2C_SCL = GPIOA,
		.I2C_SCL_PIN = GPIO_Pin_5,
		.RCC_I2C_PORT = RCC_APB2Periph_GPIOA
};

// 3. ����һ��ָ��ָ��ýṹ��--AS5600-left
SoftI2C_HandleTypeDef *AS5600_iicLeft = &Ahi2cL;

SoftI2C_HandleTypeDef Ahi2cR = {  
		.GPIO_PORT_I2C_SDA = GPIOA,
		.I2C_SDA_PIN = GPIO_Pin_6,
		.GPIO_PORT_I2C_SCL = GPIOA,
		.I2C_SCL_PIN = GPIO_Pin_0,
		.RCC_I2C_PORT = RCC_APB2Periph_GPIOA
};

// 4. ����һ��ָ��ָ��ýṹ��--AS5600-right
SoftI2C_HandleTypeDef *AS5600_iicRight = &Ahi2cR;


static void I2C_SCL_1(SoftI2C_HandleTypeDef *IICWHERE)
{
	GPIO_SetBits(IICWHERE->GPIO_PORT_I2C_SCL, IICWHERE->I2C_SCL_PIN);
}

static void I2C_SDA_1(SoftI2C_HandleTypeDef *IICWHERE)
{
	GPIO_SetBits(IICWHERE->GPIO_PORT_I2C_SDA, IICWHERE->I2C_SDA_PIN);
}
static void I2C_SCL_0(SoftI2C_HandleTypeDef *IICWHERE)
{
	GPIO_ResetBits(IICWHERE->GPIO_PORT_I2C_SCL, IICWHERE->I2C_SCL_PIN);
}
static void I2C_SDA_0(SoftI2C_HandleTypeDef *IICWHERE)
{
	GPIO_ResetBits(IICWHERE->GPIO_PORT_I2C_SDA, IICWHERE->I2C_SDA_PIN);
}

static uint8_t I2C_SDA_READ(SoftI2C_HandleTypeDef *IICWHERE)
{
	return GPIO_ReadInputDataBit(IICWHERE->GPIO_PORT_I2C_SDA, IICWHERE->I2C_SDA_PIN);
	
}
//��ʼ��IIC��ص�GPIO
//����IIC����Ĺ���ģʽ
//��дIICд��EEPROM��BYTE WRITE
//��дIIC��ȡEEPROM��RANDOM Read

//��дpage write�� seq read��У��
//����I2C�ӿ�
//��д��ͷΪӲ��I2C����ʼ��ֹͣ�����ͣ����ܵ�ʹ�ù̼��⺯����̡�stm32f10x_i2c.c
#if I2C_Select
void I2C_Config(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure; 
    I2C_InitTypeDef  I2C_InitStructure; 

    /* ʹ���� I2C �йص�ʱ�� */
    IIC_CLK_ENABLE (IIC_CLK, ENABLE );
    IIC_GPIO_CLK_ENABLE ( IIC_GPIO_CLK, ENABLE );
    
    /* I2C_SCL��I2C_SDA*/
    GPIO_InitStructure.GPIO_Pin = IIC_SCL_GPIO_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;       // ��©���
    GPIO_Init(IIC_GPIO_SCL_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = IIC_SDA_GPIO_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;       // ��©���
    GPIO_Init(IIC_GPIO_SDA_PORT, &GPIO_InitStructure);    

    /* I2C ���� */
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    
    /* �ߵ�ƽ�����ȶ����͵�ƽ���ݱ仯 SCL ʱ���ߵ�ռ�ձ� */
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    
    I2C_InitStructure.I2C_OwnAddress1 =I2Cx_OWN_ADDRESS7; 
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable ;
     
    /* I2C��Ѱַģʽ */
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    
    /* ͨ������ */
    I2C_InitStructure.I2C_ClockSpeed = I2C_Speed;
    
    /* I2C ��ʼ�� */
    I2C_Init(IIC_NUM, &I2C_InitStructure);
    
    /* ʹ�� I2C */
    I2C_Cmd(IIC_NUM, ENABLE);  
	}

void IIC_Write_Byte(uint8_t ID,uint8_t addr, uint8_t data)
{
    while (I2C_GetFlagStatus(IIC_NUM, I2C_FLAG_BUSY));

    I2C_GenerateSTART(IIC_NUM, ENABLE); /* IIC_Start�ź� */
    while (!I2C_CheckEvent(IIC_NUM, I2C_EVENT_MASTER_MODE_SELECT)); /*EV5,��ģʽ*/

    I2C_Send7bitAddress(IIC_NUM, ID, I2C_Direction_Transmitter);  /* ���дӻ� */
    while (!I2C_CheckEvent(IIC_NUM, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    I2C_SendData(IIC_NUM, addr);  /* ��������д���ַ */
    while (!I2C_CheckEvent(IIC_NUM, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    I2C_SendData(IIC_NUM, data);  /* �������� */
    while (!I2C_CheckEvent(IIC_NUM, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    I2C_GenerateSTOP(IIC_NUM, ENABLE); /* IIC_Stop�ź� */
}

#else 
	/*
*********************************************************************************************************
*	�� �� ��: i2c_CfgGpio
*	����˵��: ����I2C���ߵ�GPIO������ģ��IO�ķ�ʽʵ��
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void i2c_CfgGpio(SoftI2C_HandleTypeDef *IICWHERE)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(IICWHERE->RCC_I2C_PORT, ENABLE);	/* ��GPIOʱ�� */

	GPIO_InitStructure.GPIO_Pin = IICWHERE->I2C_SCL_PIN | IICWHERE->I2C_SDA_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  	/* ��©��� */
	GPIO_Init(IICWHERE->GPIO_PORT_I2C_SDA, &GPIO_InitStructure);
	GPIO_Init(IICWHERE->GPIO_PORT_I2C_SCL, &GPIO_InitStructure);
	/* ��һ��ֹͣ�ź�, ��λI2C�����ϵ������豸������ģʽ */
	i2c_Stop(IICWHERE);
}


/*
*********************************************************************************************************
*	�� �� ��: i2c_Delay
*	����˵��: I2C����λ�ӳ٣����400KHz
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void i2c_Delay(void)
{
	uint8_t i;

	/*��
	 	�����ʱ����ͨ���߼������ǲ��Եõ��ġ�
    ����������CPU��Ƶ72MHz ��MDK���뻷����1���Ż�
  
		ѭ������Ϊ10ʱ��SCLƵ�� = 205KHz 
		ѭ������Ϊ7ʱ��SCLƵ�� = 347KHz�� SCL�ߵ�ƽʱ��1.5us��SCL�͵�ƽʱ��2.87us 
	 	ѭ������Ϊ5ʱ��SCLƵ�� = 421KHz�� SCL�ߵ�ƽʱ��1.25us��SCL�͵�ƽʱ��2.375us 
	*/
	for (i = 0; i < 10; i++);
}

/*
*********************************************************************************************************
*	�� �� ��: i2c_Start
*	����˵��: CPU����I2C���������ź�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void i2c_Start(SoftI2C_HandleTypeDef *IICWHERE)
{
	/* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C���������ź� */
	I2C_SDA_1(IICWHERE);
	I2C_SCL_1(IICWHERE);
	i2c_Delay();
	I2C_SDA_0(IICWHERE);
	i2c_Delay();
	I2C_SCL_0(IICWHERE);
	i2c_Delay();
}

/*
*********************************************************************************************************
*	�� �� ��: i2c_Stop
*	����˵��: CPU����I2C����ֹͣ�ź�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void i2c_Stop(SoftI2C_HandleTypeDef *IICWHERE)
{
	/* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C����ֹͣ�ź� */
	I2C_SDA_0(IICWHERE);
	I2C_SCL_1(IICWHERE);
	i2c_Delay();
	I2C_SDA_1(IICWHERE);
}

/*
*********************************************************************************************************
*	�� �� ��: i2c_SendByte
*	����˵��: CPU��I2C�����豸����8bit����
*	��    �Σ�_ucByte �� �ȴ����͵��ֽ�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void i2c_SendByte(SoftI2C_HandleTypeDef *IICWHERE,uint8_t _ucByte)
{
	uint8_t i;

	/* �ȷ����ֽڵĸ�λbit7 */
	for (i = 0; i < 8; i++)
	{		
		if (_ucByte & 0x80)
		{
			I2C_SDA_1(IICWHERE);
		}
		else
		{
			I2C_SDA_0(IICWHERE);
		}
		i2c_Delay();
		I2C_SCL_1(IICWHERE);
		i2c_Delay();	
		I2C_SCL_0(IICWHERE);
		if (i == 7)
		{
			 I2C_SDA_1(IICWHERE); // �ͷ�����
		}
		_ucByte <<= 1;	/* ����һ��bit */
		i2c_Delay();
	}
}

/*
*********************************************************************************************************
*	�� �� ��: i2c_ReadByte
*	����˵��: CPU��I2C�����豸��ȡ8bit����
*	��    �Σ���
*	�� �� ֵ: ����������
*********************************************************************************************************
*/
uint8_t i2c_ReadByte(SoftI2C_HandleTypeDef *IICWHERE,u8 ack)
{
	uint8_t i;
	uint8_t value;

	/* ������1��bitΪ���ݵ�bit7 */
	value = 0;
	for (i = 0; i < 8; i++)
	{
		value <<= 1;
		I2C_SCL_1(IICWHERE);
		i2c_Delay();
		if (I2C_SDA_READ(IICWHERE))
		{
			value++;
		}
		I2C_SCL_0(IICWHERE);
		i2c_Delay();
	}
	if(!ack) i2c_NAck(IICWHERE);
	else
		i2c_Ack(IICWHERE);
	return value;
}

uint8_t i2c_ReadByte_no(SoftI2C_HandleTypeDef *IICWHERE)
{
	uint8_t i;
	uint8_t value;

	/* ������1��bitΪ���ݵ�bit7 */
	value = 0;
	for (i = 0; i < 8; i++)
	{
		value <<= 1;
		I2C_SCL_1(IICWHERE);
		i2c_Delay();
		if (I2C_SDA_READ(IICWHERE))
		{
			value++;
		}
		I2C_SCL_0(IICWHERE);
		i2c_Delay();
	}
	return value;
}

/*
*********************************************************************************************************
*	�� �� ��: i2c_WaitAck
*	����˵��: CPU����һ��ʱ�ӣ�����ȡ������ACKӦ���ź�
*	��    �Σ���
*	�� �� ֵ: ����0��ʾ��ȷӦ��1��ʾ��������Ӧ
*********************************************************************************************************
*/
uint8_t i2c_WaitAck(SoftI2C_HandleTypeDef *IICWHERE)
{
	uint8_t re;

	I2C_SDA_1(IICWHERE);	/* CPU�ͷ�SDA���� */
	i2c_Delay();
	I2C_SCL_1(IICWHERE);	/* CPU����SCL = 1, ��ʱ�����᷵��ACKӦ�� */
	i2c_Delay();
	if (I2C_SDA_READ(IICWHERE))	/* CPU��ȡSDA����״̬ */
	{
		re = 1;
	}
	else
	{
		re = 0;
	}
	I2C_SCL_0(IICWHERE);
	i2c_Delay();
	return re;
}

/*
*********************************************************************************************************
*	�� �� ��: i2c_Ack
*	����˵��: CPU����һ��ACK�ź�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void i2c_Ack(SoftI2C_HandleTypeDef *IICWHERE)
{
	I2C_SDA_0(IICWHERE);	/* CPU����SDA = 0 */
	i2c_Delay();
	I2C_SCL_1(IICWHERE);	/* CPU����1��ʱ�� */
	i2c_Delay();
	I2C_SCL_0(IICWHERE);
	i2c_Delay();
	I2C_SDA_1(IICWHERE);	/* CPU�ͷ�SDA���� */
}

/*
*********************************************************************************************************
*	�� �� ��: i2c_NAck
*	����˵��: CPU����1��NACK�ź�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void i2c_NAck(SoftI2C_HandleTypeDef *IICWHERE)
{
	I2C_SDA_1(IICWHERE);	/* CPU����SDA = 1 */
	i2c_Delay();
	I2C_SCL_1(IICWHERE);	/* CPU����1��ʱ�� */
	i2c_Delay();
	I2C_SCL_0(IICWHERE);
	i2c_Delay();	
}

/*
*********************************************************************************************************
*	�� �� ��: i2c_CheckDevice
*	����˵��: ���I2C�����豸��CPU�����豸��ַ��Ȼ���ȡ�豸Ӧ�����жϸ��豸�Ƿ����
*	��    �Σ�_Address���豸��I2C���ߵ�ַ
*	�� �� ֵ: ����ֵ 0 ��ʾ��ȷ�� ����1��ʾδ̽�⵽
*********************************************************************************************************
*/
uint8_t i2c_CheckDevice(SoftI2C_HandleTypeDef *IICWHERE,uint8_t _Address)
{
	uint8_t ucAck;

	i2c_CfgGpio(IICWHERE);		/* ����GPIO */

	
	i2c_Start(IICWHERE);		/* ���������ź� */

	/* �����豸��ַ+��д����bit��0 = w�� 1 = r) bit7 �ȴ� */
	i2c_SendByte(IICWHERE,_Address | I2C_WR);
	ucAck = i2c_WaitAck(IICWHERE);	/* ����豸��ACKӦ�� */

	i2c_Stop(IICWHERE);			/* ����ֹͣ�ź� */

	return ucAck;
}

#endif
