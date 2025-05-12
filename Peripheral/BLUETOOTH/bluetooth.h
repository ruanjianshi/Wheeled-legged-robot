#ifndef __BLUETOOTH_H
#define __BLUETOOTH_H 
#include "stm32f10x.h"	  	
#include "usart.h"
#define  BLUETOOTH_USARTx                   USART3
#define  BLUETOOTH_USART_CLK                RCC_APB1Periph_USART3
#define  BLUETOOTH_USART_APBxClkCmd         RCC_APB1PeriphClockCmd
#define  BLUETOOTH_USART_BAUDRATE           9600

// USART GPIO Òý½Åºê¶¨Òå
#define  BLUETOOTH_USART_GPIO_CLK           (RCC_APB2Periph_GPIOB)
#define  BLUETOOTH_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
    
#define  BLUETOOTH_USART_TX_GPIO_PORT       GPIOB   
#define  BLUETOOTH_USART_TX_GPIO_PIN        GPIO_Pin_10
#define  BLUETOOTH_USART_RX_GPIO_PORT       GPIOB
#define  BLUETOOTH_USART_RX_GPIO_PIN        GPIO_Pin_11

#define  BLUETOOTH_USART_IRQ                USART3_IRQn
#define  BLUETOOTH_USART_IRQHandler         USART3_IRQHandler

#define flagMode 1
#define Kp_site_add 0.1
#define Ki_site_add 0.01
#define Kd_site_add 0.01

void bluetooth_init();
void BLUETOOTH_USART_IRQHandler(void);
#endif



