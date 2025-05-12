#ifndef IIC_H
#define IIC_H
 
#include "stm32f10x.h"
#include <inttypes.h>
#include <stdlib.h>  // �ṩ malloc() �� free()

 
#define OLED_IIC    	  1
#define MPU6050_IIC 	  1
#define AS5600Left_IIC  0
#define AS5600Right_IIC 0
 
typedef struct {
    GPIO_TypeDef *GPIO_PORT_I2C_SDA;  // SDA���Ŷ˿�
    uint16_t I2C_SDA_PIN;        // SDA���ű��
    GPIO_TypeDef *GPIO_PORT_I2C_SCL;  // SCL���Ŷ˿�
    uint16_t I2C_SCL_PIN;        // SCL���ű��
    uint32_t RCC_I2C_PORT;       // ʱ���ӳ٣�΢�룩
} SoftI2C_HandleTypeDef;

 
#define SUB_I2CNUM 2
#define I2C_Select 0
#define I2C_WR	0		/* д����bit */
#define I2C_RD	1		/* ������bit */

#if I2C_Select /* �������룺 1 ѡ��Ӳ��I2C, 0 ѡ�����I2C */
	#define IIC_NUM                  I2C1
	#define IIC_GPIO_CLK_ENABLE      RCC_APB2PeriphClockCmd
	#define IIC_GPIO_CLK             RCC_APB2Periph_GPIOB
	#define IIC_CLK_ENABLE           RCC_APB1PeriphClockCmd
	#define IIC_CLK                  RCC_APB1Periph_I2C1
	#define IIC_GPIO_SCL_PORT        GPIOB
	#define IIC_GPIO_SDA_PORT        GPIOB
	#define IIC_SDA_GPIO_PIN         GPIO_Pin_7
	#define IIC_SCL_GPIO_PIN         GPIO_Pin_6
	/* STM32 I2C ����ģʽ */
  #define I2C_Speed              400000  
	/*�ȴ���ʱʱ��*/
	#define I2CT_FLAG_TIMEOUT         ((uint32_t)0x1000)
	#define I2CT_LONG_TIMEOUT         ((uint32_t)(10 * I2CT_FLAG_TIMEOUT))
	static __IO uint32_t  I2CTimeout = I2CT_LONG_TIMEOUT; 
	/* �û��Զ��峬ʱ�ص����� */
  static uint32_t I2C_TIMEOUT_UserCallback(uint8_t errorCode);
	/* �����ַֻҪ��STM32��ҵ�I2C������ַ��һ������ */
  #define I2Cx_OWN_ADDRESS7      0X0A 
	
	/* ����I2C�������ӵ�GPIO�˿�, �û�ֻ��Ҫ�޸�����4�д��뼴������ı�SCL��SDA������ */
//	#define GPIO_PORT_I2C	GPIOB			/* GPIO�˿� */
//	#define RCC_I2C_PORT 	RCC_APB2Periph_GPIOB		/* GPIO�˿�ʱ�� */
//	#define I2C_SCL_PIN		GPIO_Pin_8			/* ���ӵ�SCLʱ���ߵ�GPIO */
//	#define I2C_SDA_PIN		GPIO_Pin_9  		/* ���ӵ�SDA�����ߵ�GPIO */
	// 1. ����һ���ṹ���������ʼ��
	

//	#define GPIO_PORT_I2C	GPIOB			/* GPIO�˿� */
//	#define RCC_I2C_PORT 	RCC_APB2Periph_GPIOB		/* GPIO�˿�ʱ�� */
//	#define I2C_SCL_PIN		GPIO_Pin_6			/* ���ӵ�SCLʱ���ߵ�GPIO */
//	#define I2C_SDA_PIN		GPIO_Pin_7  		/* ���ӵ�SDA�����ߵ�GPIO */


/* �����дSCL��SDA�ĺ꣬�����Ӵ���Ŀ���ֲ�ԺͿ��Ķ��� */
#if 0	/* �������룺 1 ѡ��GPIO�Ŀ⺯��ʵ��IO��д */
 #define I2C_SCL_1()  GPIO_SetBits(GPIO_PORT_I2C, I2C_SCL_PIN)		/* SCL = 1 */
 #define I2C_SCL_0()  GPIO_ResetBits(GPIO_PORT_I2C, I2C_SCL_PIN)		/* SCL = 0 */
  
 #define I2C_SDA_1()  GPIO_SetBits(GPIO_PORT_I2C, I2C_SDA_PIN)		/* SDA = 1 */
 #define I2C_SDA_0()  GPIO_ResetBits(GPIO_PORT_I2C, I2C_SDA_PIN)		/* SDA = 0 */
  
 #define I2C_SDA_READ()  GPIO_ReadInputDataBit(GPIO_PORT_I2C, I2C_SDA_PIN)	/* ��SDA����״̬ */
#endif
#if 0	/* �����֧ѡ��ֱ�ӼĴ�������ʵ��IO��д */
    /*��ע�⣺����д������IAR��߼����Ż�ʱ���ᱻ�����������Ż� */
 #define I2C_SCL_1()  GPIO_PORT_I2C->BSRR = I2C_SCL_PIN				/* SCL = 1 */
 #define I2C_SCL_0()  GPIO_PORT_I2C->BRR = I2C_SCL_PIN				/* SCL = 0 */
  
 #define I2C_SDA_1()  GPIO_PORT_I2C->BSRR = I2C_SDA_PIN				/* SDA = 1 */
 #define I2C_SDA_0()  GPIO_PORT_I2C->BRR = I2C_SDA_PIN				/* SDA = 0 */
  
 #define I2C_SDA_READ()  ((GPIO_PORT_I2C->IDR & I2C_SDA_PIN) != 0)	/* ��SDA����״̬ */
#endif
#endif
 
/*GPIO ����IIC*/
void I2C_Config(void);  //Ӳ��I2C����
void i2c_CfgGpio(SoftI2C_HandleTypeDef *IICWHERE); //���I2C����
void i2c_Start(SoftI2C_HandleTypeDef *IICWHERE);  // ����I2C��ʼ�ź�
void i2c_Stop(SoftI2C_HandleTypeDef *IICWHERE);   // ����I2Cֹͣ�ź�
void i2c_SendByte(SoftI2C_HandleTypeDef *IICWHERE,uint8_t _ucByte);  // ����һ���ֽ�����
uint8_t i2c_ReadByte(SoftI2C_HandleTypeDef *IICWHERE,u8 ack);  // ��ȡһ���ֽ�����
uint8_t i2c_WaitAck(SoftI2C_HandleTypeDef *IICWHERE);  // �ȴ��ӻ�Ӧ��
void i2c_Ack(SoftI2C_HandleTypeDef *IICWHERE);  // ����Ӧ���ź�
void i2c_NAck(SoftI2C_HandleTypeDef *IICWHERE);  // ���ͷ�Ӧ���ź�
uint8_t i2c_CheckDevice(SoftI2C_HandleTypeDef *IICWHERE,uint8_t _Address);  // ����豸�Ƿ����
uint8_t i2c_ReadByte_no(SoftI2C_HandleTypeDef *IICWHERE);
#endif


