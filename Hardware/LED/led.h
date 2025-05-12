#ifndef _LED_H
#define _LED_H

#include "stm32f10x.h"

//GPIO�˿ڣ�GPIO��ţ�GPIO����ʼ���ߺ궨��
#define LED_G_GPIO_PORT GPIOB
#define LED_G_GPIO_PIN GPIO_Pin_0
#define LED_G_GPIO_CLK RCC_APB2Periph_GPIOB

#define LED_R_GPIO_PORT GPIOB
#define LED_R_GPIO_PIN GPIO_Pin_5
#define LED_R_GPIO_CLK RCC_APB2Periph_GPIOB

#define LED_B_GPIO_PORT GPIOB
#define LED_B_GPIO_PIN GPIO_Pin_1
#define LED_B_GPIO_CLK RCC_APB2Periph_GPIOB
//ֻ��������ļ������޸ļ���

#define ON 1
#define OFF 0

// \ c���������з������治�����κζ���
#define LED_G(a) if(a) \
												GPIO_ResetBits(LED_G_GPIO_PORT, LED_G_GPIO_PIN);\
								 else GPIO_SetBits(LED_G_GPIO_PORT, LED_G_GPIO_PIN);

#define LED_R(a) if(a) \
												GPIO_ResetBits(LED_R_GPIO_PORT, LED_R_GPIO_PIN);\
								 else GPIO_SetBits(LED_R_GPIO_PORT, LED_R_GPIO_PIN);

#define LED_B(a) if(a) \
												GPIO_ResetBits(LED_B_GPIO_PORT, LED_B_GPIO_PIN);\
								 else GPIO_SetBits(LED_B_GPIO_PORT, LED_B_GPIO_PIN);




//^���C���Ե�һ�������������
// ʵ����#define LED_G_TOGGLE {LED_G_GPIO_PORT->ODR ^= LED_G_GPIO_PIN;}

/* ֱ�Ӳ����Ĵ����ķ�������IO */
#define	digitalHigh(p,i)		 {p->BSRR=i;}	 //���Ϊ�ߵ�ƽ		
#define digitalLow(p,i)		 {p->BRR=i;}	 //����͵�ƽ
#define digitalToggle(p,i) {p->ODR ^=i;} //�����ת״̬


/* �������IO�ĺ� */
#define LED_G_TOGGLE		 digitalToggle(LED_G_GPIO_PORT,LED_G_GPIO_PIN)
#define LED_G_OFF		     digitalHigh(LED_G_GPIO_PORT,LED_G_GPIO_PIN)
#define LED_G_ON			   digitalLow(LED_G_GPIO_PORT,LED_G_GPIO_PIN)

#define LED_R_TOGGLE		 digitalToggle(LED_R_GPIO_PORT,LED_R_GPIO_PIN)
#define LED_R_OFF		     digitalHigh(LED_R_GPIO_PORT,LED_R_GPIO_PIN)
#define LED_R_ON			   digitalLow(LED_R_GPIO_PORT,LED_R_GPIO_PIN)

#define LED_B_TOGGLE		 digitalToggle(LED_B_GPIO_PORT,LED_B_GPIO_PIN)
#define LED_B_OFF		     digitalHigh(LED_B_GPIO_PORT,LED_B_GPIO_PIN)
#define LED_B_ON			   digitalLow(LED_B_GPIO_PORT,LED_B_GPIO_PIN)


/* ����RGB��ɫ */
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



