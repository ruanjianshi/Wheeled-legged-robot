#ifndef IIC_H
#define IIC_H
 
#include "stm32f10x.h"
#include <inttypes.h>
#include <stdlib.h>  // 提供 malloc() 和 free()

 
#define OLED_IIC    	  1
#define MPU6050_IIC 	  1
#define AS5600Left_IIC  0
#define AS5600Right_IIC 0
 
typedef struct {
    GPIO_TypeDef *GPIO_PORT_I2C_SDA;  // SDA引脚端口
    uint16_t I2C_SDA_PIN;        // SDA引脚编号
    GPIO_TypeDef *GPIO_PORT_I2C_SCL;  // SCL引脚端口
    uint16_t I2C_SCL_PIN;        // SCL引脚编号
    uint32_t RCC_I2C_PORT;       // 时序延迟（微秒）
} SoftI2C_HandleTypeDef;

 
#define SUB_I2CNUM 2
#define I2C_Select 0
#define I2C_WR	0		/* 写控制bit */
#define I2C_RD	1		/* 读控制bit */

#if I2C_Select /* 条件编译： 1 选择硬件I2C, 0 选择软件I2C */
	#define IIC_NUM                  I2C1
	#define IIC_GPIO_CLK_ENABLE      RCC_APB2PeriphClockCmd
	#define IIC_GPIO_CLK             RCC_APB2Periph_GPIOB
	#define IIC_CLK_ENABLE           RCC_APB1PeriphClockCmd
	#define IIC_CLK                  RCC_APB1Periph_I2C1
	#define IIC_GPIO_SCL_PORT        GPIOB
	#define IIC_GPIO_SDA_PORT        GPIOB
	#define IIC_SDA_GPIO_PIN         GPIO_Pin_7
	#define IIC_SCL_GPIO_PIN         GPIO_Pin_6
	/* STM32 I2C 快速模式 */
  #define I2C_Speed              400000  
	/*等待超时时间*/
	#define I2CT_FLAG_TIMEOUT         ((uint32_t)0x1000)
	#define I2CT_LONG_TIMEOUT         ((uint32_t)(10 * I2CT_FLAG_TIMEOUT))
	static __IO uint32_t  I2CTimeout = I2CT_LONG_TIMEOUT; 
	/* 用户自定义超时回调函数 */
  static uint32_t I2C_TIMEOUT_UserCallback(uint8_t errorCode);
	/* 这个地址只要与STM32外挂的I2C器件地址不一样即可 */
  #define I2Cx_OWN_ADDRESS7      0X0A 
	
	/* 定义I2C总线连接的GPIO端口, 用户只需要修改下面4行代码即可任意改变SCL和SDA的引脚 */
//	#define GPIO_PORT_I2C	GPIOB			/* GPIO端口 */
//	#define RCC_I2C_PORT 	RCC_APB2Periph_GPIOB		/* GPIO端口时钟 */
//	#define I2C_SCL_PIN		GPIO_Pin_8			/* 连接到SCL时钟线的GPIO */
//	#define I2C_SDA_PIN		GPIO_Pin_9  		/* 连接到SDA数据线的GPIO */
	// 1. 定义一个结构体变量并初始化
	

//	#define GPIO_PORT_I2C	GPIOB			/* GPIO端口 */
//	#define RCC_I2C_PORT 	RCC_APB2Periph_GPIOB		/* GPIO端口时钟 */
//	#define I2C_SCL_PIN		GPIO_Pin_6			/* 连接到SCL时钟线的GPIO */
//	#define I2C_SDA_PIN		GPIO_Pin_7  		/* 连接到SDA数据线的GPIO */


/* 定义读写SCL和SDA的宏，已增加代码的可移植性和可阅读性 */
#if 0	/* 条件编译： 1 选择GPIO的库函数实现IO读写 */
 #define I2C_SCL_1()  GPIO_SetBits(GPIO_PORT_I2C, I2C_SCL_PIN)		/* SCL = 1 */
 #define I2C_SCL_0()  GPIO_ResetBits(GPIO_PORT_I2C, I2C_SCL_PIN)		/* SCL = 0 */
  
 #define I2C_SDA_1()  GPIO_SetBits(GPIO_PORT_I2C, I2C_SDA_PIN)		/* SDA = 1 */
 #define I2C_SDA_0()  GPIO_ResetBits(GPIO_PORT_I2C, I2C_SDA_PIN)		/* SDA = 0 */
  
 #define I2C_SDA_READ()  GPIO_ReadInputDataBit(GPIO_PORT_I2C, I2C_SDA_PIN)	/* 读SDA口线状态 */
#endif
#if 0	/* 这个分支选择直接寄存器操作实现IO读写 */
    /*　注意：如下写法，在IAR最高级别优化时，会被编译器错误优化 */
 #define I2C_SCL_1()  GPIO_PORT_I2C->BSRR = I2C_SCL_PIN				/* SCL = 1 */
 #define I2C_SCL_0()  GPIO_PORT_I2C->BRR = I2C_SCL_PIN				/* SCL = 0 */
  
 #define I2C_SDA_1()  GPIO_PORT_I2C->BSRR = I2C_SDA_PIN				/* SDA = 1 */
 #define I2C_SDA_0()  GPIO_PORT_I2C->BRR = I2C_SDA_PIN				/* SDA = 0 */
  
 #define I2C_SDA_READ()  ((GPIO_PORT_I2C->IDR & I2C_SDA_PIN) != 0)	/* 读SDA口线状态 */
#endif
#endif
 
/*GPIO 配置IIC*/
void I2C_Config(void);  //硬件I2C配置
void i2c_CfgGpio(SoftI2C_HandleTypeDef *IICWHERE); //软件I2C配置
void i2c_Start(SoftI2C_HandleTypeDef *IICWHERE);  // 发送I2C开始信号
void i2c_Stop(SoftI2C_HandleTypeDef *IICWHERE);   // 发送I2C停止信号
void i2c_SendByte(SoftI2C_HandleTypeDef *IICWHERE,uint8_t _ucByte);  // 发送一个字节数据
uint8_t i2c_ReadByte(SoftI2C_HandleTypeDef *IICWHERE,u8 ack);  // 读取一个字节数据
uint8_t i2c_WaitAck(SoftI2C_HandleTypeDef *IICWHERE);  // 等待从机应答
void i2c_Ack(SoftI2C_HandleTypeDef *IICWHERE);  // 发送应答信号
void i2c_NAck(SoftI2C_HandleTypeDef *IICWHERE);  // 发送非应答信号
uint8_t i2c_CheckDevice(SoftI2C_HandleTypeDef *IICWHERE,uint8_t _Address);  // 检查设备是否存在
uint8_t i2c_ReadByte_no(SoftI2C_HandleTypeDef *IICWHERE);
#endif


