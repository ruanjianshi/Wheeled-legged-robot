#include "iic.h"
#include <stm32f10x_gpio.h>

SoftI2C_HandleTypeDef hi2c = {  
		.GPIO_PORT_I2C_SDA = GPIOB,
		.I2C_SDA_PIN = GPIO_Pin_9,
		.GPIO_PORT_I2C_SCL = GPIOB,
		.I2C_SCL_PIN = GPIO_Pin_8,
		.RCC_I2C_PORT = RCC_APB2Periph_GPIOB
};

// 1. 定义一个指针指向该结构体--OLED
SoftI2C_HandleTypeDef *OLED_iic = &hi2c;


SoftI2C_HandleTypeDef Mhi2c = {  
		.GPIO_PORT_I2C_SDA = GPIOB,
		.I2C_SDA_PIN = GPIO_Pin_7,
		.GPIO_PORT_I2C_SCL = GPIOB,
		.I2C_SCL_PIN = GPIO_Pin_6,
		.RCC_I2C_PORT = RCC_APB2Periph_GPIOB
};

// 2. 定义一个指针指向该结构体--MPU6050
SoftI2C_HandleTypeDef *MPU6050_iic = &Mhi2c;
	
SoftI2C_HandleTypeDef Ahi2cL = {  
		.GPIO_PORT_I2C_SDA = GPIOA,
		.I2C_SDA_PIN = GPIO_Pin_4,
		.GPIO_PORT_I2C_SCL = GPIOA,
		.I2C_SCL_PIN = GPIO_Pin_5,
		.RCC_I2C_PORT = RCC_APB2Periph_GPIOA
};

// 3. 定义一个指针指向该结构体--AS5600-left
SoftI2C_HandleTypeDef *AS5600_iicLeft = &Ahi2cL;

SoftI2C_HandleTypeDef Ahi2cR = {  
		.GPIO_PORT_I2C_SDA = GPIOA,
		.I2C_SDA_PIN = GPIO_Pin_6,
		.GPIO_PORT_I2C_SCL = GPIOA,
		.I2C_SCL_PIN = GPIO_Pin_0,
		.RCC_I2C_PORT = RCC_APB2Periph_GPIOA
};

// 4. 定义一个指针指向该结构体--AS5600-right
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
//初始化IIC相关的GPIO
//配置IIC外设的工作模式
//编写IIC写入EEPROM的BYTE WRITE
//编写IIC读取EEPROM的RANDOM Read

//编写page write及 seq read并校验
//配置I2C接口
//大写开头为硬件I2C，开始，停止，发送，接受等使用固件库函数编程。stm32f10x_i2c.c
#if I2C_Select
void I2C_Config(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure; 
    I2C_InitTypeDef  I2C_InitStructure; 

    /* 使能与 I2C 有关的时钟 */
    IIC_CLK_ENABLE (IIC_CLK, ENABLE );
    IIC_GPIO_CLK_ENABLE ( IIC_GPIO_CLK, ENABLE );
    
    /* I2C_SCL、I2C_SDA*/
    GPIO_InitStructure.GPIO_Pin = IIC_SCL_GPIO_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;       // 开漏输出
    GPIO_Init(IIC_GPIO_SCL_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = IIC_SDA_GPIO_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;       // 开漏输出
    GPIO_Init(IIC_GPIO_SDA_PORT, &GPIO_InitStructure);    

    /* I2C 配置 */
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    
    /* 高电平数据稳定，低电平数据变化 SCL 时钟线的占空比 */
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    
    I2C_InitStructure.I2C_OwnAddress1 =I2Cx_OWN_ADDRESS7; 
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable ;
     
    /* I2C的寻址模式 */
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    
    /* 通信速率 */
    I2C_InitStructure.I2C_ClockSpeed = I2C_Speed;
    
    /* I2C 初始化 */
    I2C_Init(IIC_NUM, &I2C_InitStructure);
    
    /* 使能 I2C */
    I2C_Cmd(IIC_NUM, ENABLE);  
	}

void IIC_Write_Byte(uint8_t ID,uint8_t addr, uint8_t data)
{
    while (I2C_GetFlagStatus(IIC_NUM, I2C_FLAG_BUSY));

    I2C_GenerateSTART(IIC_NUM, ENABLE); /* IIC_Start信号 */
    while (!I2C_CheckEvent(IIC_NUM, I2C_EVENT_MASTER_MODE_SELECT)); /*EV5,主模式*/

    I2C_Send7bitAddress(IIC_NUM, ID, I2C_Direction_Transmitter);  /* 呼叫从机 */
    while (!I2C_CheckEvent(IIC_NUM, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    I2C_SendData(IIC_NUM, addr);  /* 配置数据写入地址 */
    while (!I2C_CheckEvent(IIC_NUM, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    I2C_SendData(IIC_NUM, data);  /* 发送数据 */
    while (!I2C_CheckEvent(IIC_NUM, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    I2C_GenerateSTOP(IIC_NUM, ENABLE); /* IIC_Stop信号 */
}

#else 
	/*
*********************************************************************************************************
*	函 数 名: i2c_CfgGpio
*	功能说明: 配置I2C总线的GPIO，采用模拟IO的方式实现
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_CfgGpio(SoftI2C_HandleTypeDef *IICWHERE)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(IICWHERE->RCC_I2C_PORT, ENABLE);	/* 打开GPIO时钟 */

	GPIO_InitStructure.GPIO_Pin = IICWHERE->I2C_SCL_PIN | IICWHERE->I2C_SDA_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  	/* 开漏输出 */
	GPIO_Init(IICWHERE->GPIO_PORT_I2C_SDA, &GPIO_InitStructure);
	GPIO_Init(IICWHERE->GPIO_PORT_I2C_SCL, &GPIO_InitStructure);
	/* 给一个停止信号, 复位I2C总线上的所有设备到待机模式 */
	i2c_Stop(IICWHERE);
}


/*
*********************************************************************************************************
*	函 数 名: i2c_Delay
*	功能说明: I2C总线位延迟，最快400KHz
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void i2c_Delay(void)
{
	uint8_t i;

	/*　
	 	下面的时间是通过逻辑分析仪测试得到的。
    工作条件：CPU主频72MHz ，MDK编译环境，1级优化
  
		循环次数为10时，SCL频率 = 205KHz 
		循环次数为7时，SCL频率 = 347KHz， SCL高电平时间1.5us，SCL低电平时间2.87us 
	 	循环次数为5时，SCL频率 = 421KHz， SCL高电平时间1.25us，SCL低电平时间2.375us 
	*/
	for (i = 0; i < 10; i++);
}

/*
*********************************************************************************************************
*	函 数 名: i2c_Start
*	功能说明: CPU发起I2C总线启动信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_Start(SoftI2C_HandleTypeDef *IICWHERE)
{
	/* 当SCL高电平时，SDA出现一个下跳沿表示I2C总线启动信号 */
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
*	函 数 名: i2c_Stop
*	功能说明: CPU发起I2C总线停止信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_Stop(SoftI2C_HandleTypeDef *IICWHERE)
{
	/* 当SCL高电平时，SDA出现一个上跳沿表示I2C总线停止信号 */
	I2C_SDA_0(IICWHERE);
	I2C_SCL_1(IICWHERE);
	i2c_Delay();
	I2C_SDA_1(IICWHERE);
}

/*
*********************************************************************************************************
*	函 数 名: i2c_SendByte
*	功能说明: CPU向I2C总线设备发送8bit数据
*	形    参：_ucByte ： 等待发送的字节
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_SendByte(SoftI2C_HandleTypeDef *IICWHERE,uint8_t _ucByte)
{
	uint8_t i;

	/* 先发送字节的高位bit7 */
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
			 I2C_SDA_1(IICWHERE); // 释放总线
		}
		_ucByte <<= 1;	/* 左移一个bit */
		i2c_Delay();
	}
}

/*
*********************************************************************************************************
*	函 数 名: i2c_ReadByte
*	功能说明: CPU从I2C总线设备读取8bit数据
*	形    参：无
*	返 回 值: 读到的数据
*********************************************************************************************************
*/
uint8_t i2c_ReadByte(SoftI2C_HandleTypeDef *IICWHERE,u8 ack)
{
	uint8_t i;
	uint8_t value;

	/* 读到第1个bit为数据的bit7 */
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

	/* 读到第1个bit为数据的bit7 */
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
*	函 数 名: i2c_WaitAck
*	功能说明: CPU产生一个时钟，并读取器件的ACK应答信号
*	形    参：无
*	返 回 值: 返回0表示正确应答，1表示无器件响应
*********************************************************************************************************
*/
uint8_t i2c_WaitAck(SoftI2C_HandleTypeDef *IICWHERE)
{
	uint8_t re;

	I2C_SDA_1(IICWHERE);	/* CPU释放SDA总线 */
	i2c_Delay();
	I2C_SCL_1(IICWHERE);	/* CPU驱动SCL = 1, 此时器件会返回ACK应答 */
	i2c_Delay();
	if (I2C_SDA_READ(IICWHERE))	/* CPU读取SDA口线状态 */
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
*	函 数 名: i2c_Ack
*	功能说明: CPU产生一个ACK信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_Ack(SoftI2C_HandleTypeDef *IICWHERE)
{
	I2C_SDA_0(IICWHERE);	/* CPU驱动SDA = 0 */
	i2c_Delay();
	I2C_SCL_1(IICWHERE);	/* CPU产生1个时钟 */
	i2c_Delay();
	I2C_SCL_0(IICWHERE);
	i2c_Delay();
	I2C_SDA_1(IICWHERE);	/* CPU释放SDA总线 */
}

/*
*********************************************************************************************************
*	函 数 名: i2c_NAck
*	功能说明: CPU产生1个NACK信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_NAck(SoftI2C_HandleTypeDef *IICWHERE)
{
	I2C_SDA_1(IICWHERE);	/* CPU驱动SDA = 1 */
	i2c_Delay();
	I2C_SCL_1(IICWHERE);	/* CPU产生1个时钟 */
	i2c_Delay();
	I2C_SCL_0(IICWHERE);
	i2c_Delay();	
}

/*
*********************************************************************************************************
*	函 数 名: i2c_CheckDevice
*	功能说明: 检测I2C总线设备，CPU向发送设备地址，然后读取设备应答来判断该设备是否存在
*	形    参：_Address：设备的I2C总线地址
*	返 回 值: 返回值 0 表示正确， 返回1表示未探测到
*********************************************************************************************************
*/
uint8_t i2c_CheckDevice(SoftI2C_HandleTypeDef *IICWHERE,uint8_t _Address)
{
	uint8_t ucAck;

	i2c_CfgGpio(IICWHERE);		/* 配置GPIO */

	
	i2c_Start(IICWHERE);		/* 发送启动信号 */

	/* 发送设备地址+读写控制bit（0 = w， 1 = r) bit7 先传 */
	i2c_SendByte(IICWHERE,_Address | I2C_WR);
	ucAck = i2c_WaitAck(IICWHERE);	/* 检测设备的ACK应答 */

	i2c_Stop(IICWHERE);			/* 发送停止信号 */

	return ucAck;
}

#endif
