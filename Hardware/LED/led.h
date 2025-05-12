#ifndef _LED_H
#define _LED_H

#include "stm32f10x.h"

//GPIO端口，GPIO编号，GPIO挂载始终线宏定义
#define LED_G_GPIO_PORT GPIOB
#define LED_G_GPIO_PIN GPIO_Pin_0
#define LED_G_GPIO_CLK RCC_APB2Periph_GPIOB

#define LED_R_GPIO_PORT GPIOB
#define LED_R_GPIO_PIN GPIO_Pin_5
#define LED_R_GPIO_CLK RCC_APB2Periph_GPIOB

#define LED_B_GPIO_PORT GPIOB
#define LED_B_GPIO_PIN GPIO_Pin_1
#define LED_B_GPIO_CLK RCC_APB2Periph_GPIOB
//只需对以上文件进行修改即可

#define ON 1
#define OFF 0

// \ c语言中续行符，后面不能有任何东西
#define LED_G(a) if(a) \
												GPIO_ResetBits(LED_G_GPIO_PORT, LED_G_GPIO_PIN);\
								 else GPIO_SetBits(LED_G_GPIO_PORT, LED_G_GPIO_PIN);

#define LED_R(a) if(a) \
												GPIO_ResetBits(LED_R_GPIO_PORT, LED_R_GPIO_PIN);\
								 else GPIO_SetBits(LED_R_GPIO_PORT, LED_R_GPIO_PIN);

#define LED_B(a) if(a) \
												GPIO_ResetBits(LED_B_GPIO_PORT, LED_B_GPIO_PIN);\
								 else GPIO_SetBits(LED_B_GPIO_PORT, LED_B_GPIO_PIN);




//^异或，C语言的一个二进制运算符
// 实例：#define LED_G_TOGGLE {LED_G_GPIO_PORT->ODR ^= LED_G_GPIO_PIN;}

/* 直接操作寄存器的方法控制IO */
#define	digitalHigh(p,i)		 {p->BSRR=i;}	 //输出为高电平		
#define digitalLow(p,i)		 {p->BRR=i;}	 //输出低电平
#define digitalToggle(p,i) {p->ODR ^=i;} //输出反转状态


/* 定义控制IO的宏 */
#define LED_G_TOGGLE		 digitalToggle(LED_G_GPIO_PORT,LED_G_GPIO_PIN)
#define LED_G_OFF		     digitalHigh(LED_G_GPIO_PORT,LED_G_GPIO_PIN)
#define LED_G_ON			   digitalLow(LED_G_GPIO_PORT,LED_G_GPIO_PIN)

#define LED_R_TOGGLE		 digitalToggle(LED_R_GPIO_PORT,LED_R_GPIO_PIN)
#define LED_R_OFF		     digitalHigh(LED_R_GPIO_PORT,LED_R_GPIO_PIN)
#define LED_R_ON			   digitalLow(LED_R_GPIO_PORT,LED_R_GPIO_PIN)

#define LED_B_TOGGLE		 digitalToggle(LED_B_GPIO_PORT,LED_B_GPIO_PIN)
#define LED_B_OFF		     digitalHigh(LED_B_GPIO_PORT,LED_B_GPIO_PIN)
#define LED_B_ON			   digitalLow(LED_B_GPIO_PORT,LED_B_GPIO_PIN)


/* 定义RGB颜色 */
#define LED_GREEN  LED_G_ON;LED_R_OFF;LED_B_OFF
#define LED_RED    LED_R_ON;LED_G_OFF;LED_B_OFF
#define LED_BLUE   LED_B_ON;LED_G_OFF;LED_R_OFF
#define LED_YELLOW LED_G_ON;LED_R_ON;LED_B_OFF
#define LED_PURPLE LED_R_ON;LED_B_ON;LED_G_OFF
#define LED_CYAN   LED_G_ON;LED_B_ON;LED_R_OFF
#define LED_BLACK  LED_G_OFF;LED_R_OFF;LED_B_OFF
#define LED_WHITE  LED_G_ON;LED_R_ON;LED_B_ON


void LED_GPIO_Config(void);

#endif



