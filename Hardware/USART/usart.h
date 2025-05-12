#ifndef __USART_H
#define __USART_H


#include "stm32f10x.h"
#include "stdio.h"
#include <stdarg.h>


#define TONGXIN_USART1 1
#define TONGXIN_USART2 0
#define TONGXIN_USART3 0
#define TONGXIN_USART4 0
#define TONGXIN_USART5 0
#define ROS_USART      1


#if TONGXIN_USART1
#define  TONGXIN_USARTx                   USART1
#define  TONGXIN_USART_CLK                RCC_APB2Periph_USART1
#define  TONGXIN_USART_APBxClkCmd         RCC_APB2PeriphClockCmd
#define  TONGXIN_USART_BAUDRATE           115200

// USART GPIO 引脚宏定义
#define  TONGXIN_USART_GPIO_CLK           (RCC_APB2Periph_GPIOA)
#define  TONGXIN_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
    
#define  TONGXIN_USART_TX_GPIO_PORT       GPIOA   
#define  TONGXIN_USART_TX_GPIO_PIN        GPIO_Pin_9
#define  TONGXIN_USART_RX_GPIO_PORT       GPIOA
#define  TONGXIN_USART_RX_GPIO_PIN        GPIO_Pin_10

#define  TONGXIN_USART_IRQ                USART1_IRQn
#define  TONGXIN_USART_IRQHandler         USART1_IRQHandler


#elif TONGXIN_USART2
 //串口2-USART2
#define  TONGXIN_USARTx                   USART2
#define  TONGXIN_USART_CLK                RCC_APB1Periph_USART2
#define  TONGXIN_USART_APBxClkCmd         RCC_APB1PeriphClockCmd
#define  TONGXIN_USART_BAUDRATE           115200

// USART GPIO 引脚宏定义
#define  TONGXIN_USART_GPIO_CLK           (RCC_APB2Periph_GPIOA)
#define  TONGXIN_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
    
#define  TONGXIN_USART_TX_GPIO_PORT       GPIOA   
#define  TONGXIN_USART_TX_GPIO_PIN        GPIO_Pin_2
#define  TONGXIN_USART_RX_GPIO_PORT       GPIOA
#define  TONGXIN_USART_RX_GPIO_PIN        GPIO_Pin_3

#define  TONGXIN_USART_IRQ                USART2_IRQn
#define  TONGXIN_USART_IRQHandler         USART2_IRQHandler


#elif TONGXIN_USART3
 //串口3-USART3
#define  TONGXIN_USARTx                   USART3
#define  TONGXIN_USART_CLK                RCC_APB1Periph_USART3
#define  TONGXIN_USART_APBxClkCmd         RCC_APB1PeriphClockCmd
#define  TONGXIN_USART_BAUDRATE           115200

// USART GPIO 引脚宏定义
#define  TONGXIN_USART_GPIO_CLK           (RCC_APB2Periph_GPIOB)
#define  TONGXIN_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
    
#define  TONGXIN_USART_TX_GPIO_PORT       GPIOB   
#define  TONGXIN_USART_TX_GPIO_PIN        GPIO_Pin_10
#define  TONGXIN_USART_RX_GPIO_PORT       GPIOB
#define  TONGXIN_USART_RX_GPIO_PIN        GPIO_Pin_11

#define  TONGXIN_USART_IRQ                USART3_IRQn
#define  TONGXIN_USART_IRQHandler         USART3_IRQHandler


#elif TONGXIN_USART4
 //串口4-UART4
#define  TONGXIN_USARTx                   UART4
#define  TONGXIN_USART_CLK                RCC_APB1Periph_UART4
#define  TONGXIN_USART_APBxClkCmd         RCC_APB1PeriphClockCmd
#define  TONGXIN_USART_BAUDRATE           115200

// USART GPIO 引脚宏定义
#define  TONGXIN_USART_GPIO_CLK           (RCC_APB2Periph_GPIOC)
#define  TONGXIN_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
    
#define  TONGXIN_USART_TX_GPIO_PORT       GPIOC   
#define  TONGXIN_USART_TX_GPIO_PIN        GPIO_Pin_10
#define  TONGXIN_USART_RX_GPIO_PORT       GPIOC
#define  TONGXIN_USART_RX_GPIO_PIN        GPIO_Pin_11

#define  TONGXIN_USART_IRQ                UART4_IRQn
#define  TONGXIN_USART_IRQHandler         UART4_IRQHandler


#elif TONGXIN_USART5
 //串口5-UART5
#define  TONGXIN_USARTx                   UART5
#define  TONGXIN_USART_CLK                RCC_APB1Periph_UART5
#define  TONGXIN_USART_APBxClkCmd         RCC_APB1PeriphClockCmd
#define  TONGXIN_USART_BAUDRATE           115200

// USART GPIO 引脚宏定义
#define  TONGXIN_USART_GPIO_CLK           (RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD)
#define  TONGXIN_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
    
#define  TONGXIN_USART_TX_GPIO_PORT       GPIOC   
#define  TONGXIN_USART_TX_GPIO_PIN        GPIO_Pin_12
#define  TONGXIN_USART_RX_GPIO_PORT       GPIOD
#define  TONGXIN_USART_RX_GPIO_PIN        GPIO_Pin_2

#define  TONGXIN_USART_IRQ                UART5_IRQn
#define  TONGXIN_USART_IRQHandler         UART5_IRQHandler


#endif

void USART_Config(void);
void Usart_Send8Bit(USART_TypeDef *pUSARTx, uint8_t data);
void Usart_Send16Bit(USART_TypeDef *pUSARTx, uint16_t data);
void Usart_SendArray(USART_TypeDef *pUSARTx, uint8_t *array,uint8_t num);
void Usart_SendStr(USART_TypeDef *pUSARTx, uint8_t *str);

#endif




