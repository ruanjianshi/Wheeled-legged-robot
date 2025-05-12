#ifndef __TIMER_H
#define __TIMER_H
/********************��ʱ�����ܣ���ʱ�����벶������Ƚϡ��������������߼���ʱ�����У���PWM************/
//stm32f103c8t6��û�л�����ʱ����ֻ��TIM1-TIM4���ֱ��Ǹ߼���ʱ����ͨ�ö�ʱ��

/*
����	       		�ͺ�ʾ��									λ��									��Ҫ����																																							������;
������ʱ��			TIM6, TIM7							16-bit				- �������ϼ���				- ����ʱ�����ܣ�������/���ͨ��																- ���� DAC ͬ�����	�򵥶�ʱ���񡢲��������жϡ����� DAC
ͨ�ö�ʱ��			TIM2, TIM3, TIM4, TIM5	16/32-bit			- ����/���¼���- ���벶������Ƚ�- PWM ���ɣ����ض��롢���Ķ��룩- �������ӿ�			- �ⲿ�¼�����������ͨ��	������ơ����������PWM �ź����ɡ��������ӿ�
�߼����ƶ�ʱ��	TIM1, TIM8							16-bit				- ����ͨ�ö�ʱ������- �����������������- ����ֹͣ����- �߼��������ӿ�							- ͬ�����ȸ���	������ FOC ���ơ����� PWM ������������� H �ſ��ƣ����߾��� PWM

��ʱ��ʱ��ԴƵ�� (TIM_CLK)		Ԥ��Ƶϵ�� (PSC) 		�Զ�����ֵ (ARR) = TIM_Period 
��ʱʱ�� = ((PSC + 1) x (ARR + 1)) / TIM_CLK  (s)
*/

#include "stm32f10x.h"
#include "PWM.h"
#include "servo.h"

#define Basic_TIM 				0
#define General_TIM 			1
#define General_TIM_PWM		1
#define General_TIM_INPUT 0
#define General_TIM_Synchronization 0
#define Advance_TIM       0
#define Advance_TIM_PWM   1
#define Advance_TIM_Complementary_DeadZone 0
#define Advance_TIM_PWM_INPUT 0
#define Advance_TIM_DMA		0

#if Basic_TIM
/********************������ʱ��TIM�������壬ֻ��TIM6��7************/
#define BASIC_TIM6 // ���ʹ��TIM7��ע�͵�����꼴��
/*72M/(71+1) = 1MHz ARR = 999,ÿ����һǧ���Ͳ���һ���ж�*/
#ifdef  BASIC_TIM6 // ʹ�û�����ʱ��TIM6
#define            BASIC_TIM                   TIM6
#define            BASIC_TIM_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define            BASIC_TIM_CLK               RCC_APB1Periph_TIM6
#define            BASIC_TIM_Period            1000-1
#define            BASIC_TIM_Prescaler         71
#define            BASIC_TIM_IRQ               TIM6_IRQn
#define            BASIC_TIM_IRQHandler        TIM6_IRQHandler

#else  // ʹ�û�����ʱ��TIM7
#define            BASIC_TIM                   TIM7
#define            BASIC_TIM_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define            BASIC_TIM_CLK               RCC_APB1Periph_TIM7
#define            BASIC_TIM_Period            1000-1
#define            BASIC_TIM_Prescaler         71
#define            BASIC_TIM_IRQ               TIM7_IRQn
#define            BASIC_TIM_IRQHandler        TIM7_IRQHandler

#endif

#endif

#if General_TIM
/**************ͨ�ö�ʱ��TIM�������壬ֻ��TIM2��3��4��5************/
// ����Ҫ�ĸ���ʱ����ʱ��ֻ��Ҫ������ĺ궨��ĳ�1����
#define GENERAL_TIM2    1
#define GENERAL_TIM3    0
#define GENERAL_TIM4    0
#define GENERAL_TIM5    0

#if  GENERAL_TIM2
#define            GENERAL_TIM                   TIM2
#define            GENERAL_TIM_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define            GENERAL_TIM_CLK               RCC_APB1Periph_TIM2
#define            GENERAL_TIM_Period            (1000-1)
#define            GENERAL_TIM_Prescaler         71
#define            GENERAL_TIM_IRQ               TIM2_IRQn
#define            GENERAL_TIM_IRQHandler        TIM2_IRQHandler

#elif  GENERAL_TIM3
#define            GENERAL_TIM                   TIM3
#define            GENERAL_TIM_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define            GENERAL_TIM_CLK               RCC_APB1Periph_TIM3
#define            GENERAL_TIM_Period            (1000-1)
#define            GENERAL_TIM_Prescaler         71
#define            GENERAL_TIM_IRQ               TIM3_IRQn
#define            GENERAL_TIM_IRQHandler        TIM3_IRQHandler

#elif   GENERAL_TIM4
#define            GENERAL_TIM                   TIM4
#define            GENERAL_TIM_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define            GENERAL_TIM_CLK               RCC_APB1Periph_TIM4
#define            GENERAL_TIM_Period            (1000-1)
#define            GENERAL_TIM_Prescaler         71
#define            GENERAL_TIM_IRQ               TIM4_IRQn
#define            GENERAL_TIM_IRQHandler        TIM4_IRQHandler

#elif   GENERAL_TIM5
#define            GENERAL_TIM                   TIM5
#define            GENERAL_TIM_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define            GENERAL_TIM_CLK               RCC_APB1Periph_TIM5
#define            GENERAL_TIM_Period            (1000-1)
#define            GENERAL_TIM_Prescaler         71
#define            GENERAL_TIM_IRQ               TIM5_IRQn
#define            GENERAL_TIM_IRQHandler        TIM5_IRQHandler

#endif

#endif

#if General_TIM_PWM
/************ͨ�ö�ʱ��TIM�������壬ֻ��TIM2��3��4��5************/
// ��ʹ�ò�ͬ�Ķ�ʱ����ʱ�򣬶�Ӧ��GPIO�ǲ�һ���ģ����Ҫע��
// ��������Ĭ��ʹ��TIM3

#define            GENERAL_TIM_PWM                   TIM3
#define            GENERAL_TIM_APBxClock_FUN_PWM     RCC_APB1PeriphClockCmd
#define            GENERAL_TIM_CLK_PWM               RCC_APB1Periph_TIM3
#define            GENERAL_TIM_Period_PWM            0      //Ƶ������50hz -> ARR
#define            GENERAL_TIM_Prescaler_PWM         1440
// TIM3 ����Ƚ�ͨ��1
#define            GENERAL_TIM_CH1_GPIO_CLK      RCC_APB2Periph_GPIOA
#define            GENERAL_TIM_CH1_PORT          GPIOA
#define            GENERAL_TIM_CH1_PIN           GPIO_Pin_6

// TIM3 ����Ƚ�ͨ��2
#define            GENERAL_TIM_CH2_GPIO_CLK      RCC_APB2Periph_GPIOA
#define            GENERAL_TIM_CH2_PORT          GPIOA
#define            GENERAL_TIM_CH2_PIN           GPIO_Pin_7

// TIM3 ����Ƚ�ͨ��3
#define            GENERAL_TIM_CH3_GPIO_CLK      RCC_APB2Periph_GPIOB
#define            GENERAL_TIM_CH3_PORT          GPIOB
#define            GENERAL_TIM_CH3_PIN           GPIO_Pin_0

// TIM3 ����Ƚ�ͨ��4
#define            GENERAL_TIM_CH4_GPIO_CLK      RCC_APB2Periph_GPIOB
#define            GENERAL_TIM_CH4_PORT          GPIOB
#define            GENERAL_TIM_CH4_PIN           GPIO_Pin_1


#define            GENERAL_TIM_IRQ_PWM              TIM3_IRQn
#define            GENERAL_TIM_IRQHandler_PWM       TIM3_IRQHandler

#define PWM_Period  1440

#endif


#if General_TIM_INPUT
/************ͨ�ö�ʱ��TIM�������壬ֻ��TIM2��3��4��5************/
// ��ʹ�ò�ͬ�Ķ�ʱ����ʱ�򣬶�Ӧ��GPIO�ǲ�һ���ģ����Ҫע��
// ��������Ĭ��ʹ��TIM5

#define            GENERAL_TIM                   TIM5
#define            GENERAL_TIM_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define            GENERAL_TIM_CLK               RCC_APB1Periph_TIM5
#define            GENERAL_TIM_PERIOD            0XFFFF
#define            GENERAL_TIM_PSC              (72-1)

// TIM ���벶��ͨ��GPIO��غ궨��
#define            GENERAL_TIM_CH1_GPIO_CLK      RCC_APB2Periph_GPIOA
#define            GENERAL_TIM_CH1_PORT          GPIOA
#define            GENERAL_TIM_CH1_PIN           GPIO_Pin_0
#define            GENERAL_TIM_CHANNEL_x         TIM_Channel_1

// �ж���غ궨��
#define            GENERAL_TIM_IT_CCx            TIM_IT_CC1
#define            GENERAL_TIM_IRQ               TIM5_IRQn
#define            GENERAL_TIM_INT_FUN           TIM5_IRQHandler

// ��ȡ����Ĵ���ֵ�����궨��
#define            GENERAL_TIM_GetCapturex_FUN                 TIM_GetCapture1
// �����źż��Ժ����궨��
#define            GENERAL_TIM_OCxPolarityConfig_FUN           TIM_OC1PolarityConfig

// ��������ʼ����
#define            GENERAL_TIM_STRAT_ICPolarity                TIM_ICPolarity_Rising
// �����Ľ�������
#define            GENERAL_TIM_END_ICPolarity                  TIM_ICPolarity_Falling


// ��ʱ�����벶���û��Զ�������ṹ������
typedef struct
{   
	uint8_t   Capture_FinishFlag;   // ���������־λ
	uint8_t   Capture_StartFlag;    // ����ʼ��־λ
	uint16_t  Capture_CcrValue;     // ����Ĵ�����ֵ
	uint16_t  Capture_Period;       // �Զ���װ�ؼĴ������±�־ 
}TIM_ICUserValueTypeDef;

extern TIM_ICUserValueTypeDef TIM_ICUserValueStructure;

#endif

#if General_TIM_Synchronization

/* ��ʱ�� */
#define GENERAL_TIM_MASTER     		    TIM2
#define GENERAL_TIM_MASTER_CLK        RCC_APB1Periph_TIM2

#define GENERAL_TIM_SLAVE1     		    TIM3
#define GENERAL_TIM_SLAVE1_CLK        RCC_APB1Periph_TIM3

#define GENERAL_TIM_SLAVE11     		  TIM4
#define GENERAL_TIM_SLAVE11_CLK       RCC_APB1Periph_TIM4

/* TIM��ģʽPWM������� */
#define GENERAL_MASTER_PIN            GPIO_Pin_10             
#define GENERAL_MASTER_GPIO_PORT      GPIOB                      
#define GENERAL_MASTER_GPIO_CLK       RCC_APB2Periph_GPIOB
#define GENERAL_MASTER_PINSOURCE			GPIO_PinSource5
#define GENERAL_MASTER_AF							GPIO_AF_TIM2

/* TIM��ģʽPWM������� */
#define GENERAL_SLAVE1_PIN            GPIO_Pin_6             
#define GENERAL_SLAVE1_GPIO_PORT      GPIOC                      
#define GENERAL_SLAVE1_GPIO_CLK       RCC_APB2Periph_GPIOC
#define GENERAL_SLAVE1_PINSOURCE			GPIO_PinSource6
#define GENERAL_SLAVE1_AF							GPIO_AF_TIM3

/* TIM��ģʽ�Ĵ�ģʽPWM������� */
#define GENERAL_SLAVE11_PIN           GPIO_Pin_6             
#define GENERAL_SLAVE11_GPIO_PORT     GPIOB                      
#define GENERAL_SLAVE11_GPIO_CLK      RCC_APB2Periph_GPIOB
#define GENERAL_SLAVE11_PINSOURCE			GPIO_PinSource6
#define GENERAL_SLAVE11_AF						GPIO_AF_TIM4


#endif

#if Avance_TIM

/********************�߼���ʱ��TIM�������壬ֻ��TIM1��8************/
#define ADVANCE_TIM1 // ���ʹ��TIM8��ע�͵�����꼴��

#ifdef  ADVANCE_TIM1 // ʹ�ø߼���ʱ��TIM1

#define            ADVANCE_TIM                   TIM1
#define            ADVANCE_TIM_APBxClock_FUN     RCC_APB2PeriphClockCmd
#define            ADVANCE_TIM_CLK               RCC_APB2Periph_TIM1
#define            ADVANCE_TIM_Period            (1000-1)
#define            ADVANCE_TIM_Prescaler         72-1
#define            ADVANCE_TIM_IRQ               TIM1_UP_IRQn
#define            ADVANCE_TIM_IRQHandler        TIM1_UP_IRQHandler

#else  // ʹ�ø߼���ʱ��TIM8
#define            ADVANCE_TIM                   TIM8
#define            ADVANCE_TIM_APBxClock_FUN     RCC_APB2PeriphClockCmd
#define            ADVANCE_TIM_CLK               RCC_APB2Periph_TIM8
#define            ADVANCE_TIM_Period            (1000-1)
#define            ADVANCE_TIM_Prescaler         71
#define            ADVANCE_TIM_IRQ               TIM8_UP_IRQn
#define            ADVANCE_TIM_IRQHandler        TIM8_UP_IRQHandler

#endif

#endif

#if Advance_TIM_PWM

#define            ADVANCE_TIM                   TIM1
#define            ADVANCE_TIM_APBxClock_FUN     RCC_APB2PeriphClockCmd
#define            ADVANCE_TIM_CLK               RCC_APB2Periph_TIM1
// PWM �źŵ�Ƶ�� F = TIM_CLK/{(ARR+1)*(PSC+1)}
#define            ADVANCE_TIM_PERIOD            (20000-1)
#define            ADVANCE_TIM_PSC               (72-1)
#define            ADVANCE_TIM_PULSE             ADVANCE_TIM_PULSE_Servo_SetAngle1()

#define            ADVANCE_TIM_IRQ               TIM1_UP_IRQn
#define            ADVANCE_TIM_IRQHandler        TIM1_UP_IRQHandler

// TIM1 ����Ƚ�ͨ��
#define            ADVANCE_TIM_CH1_GPIO_CLK      RCC_APB2Periph_GPIOA
#define            ADVANCE_TIM_CH1_PORT          GPIOA
#define            ADVANCE_TIM_CH1_PIN           GPIO_Pin_8

// TIM1 ����Ƚ�ͨ��
#define            ADVANCE_TIM_CH2_GPIO_CLK      RCC_APB2Periph_GPIOA
#define            ADVANCE_TIM_CH2_PORT          GPIOA
#define            ADVANCE_TIM_CH2_PIN           GPIO_Pin_9

// TIM1 ����Ƚ�ͨ��
#define            ADVANCE_TIM_CH4_GPIO_CLK      RCC_APB2Periph_GPIOA
#define            ADVANCE_TIM_CH4_PORT          GPIOA
#define            ADVANCE_TIM_CH4_PIN           GPIO_Pin_11

// TIM1 ����Ƚ�ͨ��
#define            ADVANCE_TIM_CH3_GPIO_CLK      RCC_APB2Periph_GPIOA
#define            ADVANCE_TIM_CH3_PORT          GPIOA
#define            ADVANCE_TIM_CH3_PIN           GPIO_Pin_10

#endif


#if Advance_TIM_Complementary_DeadZone

/************�߼���ʱ��TIM�������壬ֻ��TIM1��TIM8************/
// ��ʹ�ò�ͬ�Ķ�ʱ����ʱ�򣬶�Ӧ��GPIO�ǲ�һ���ģ����Ҫע��
// ��������ʹ�ø߼����ƶ�ʱ��TIM1

#define            ADVANCE_TIM                   TIM1
#define            ADVANCE_TIM_APBxClock_FUN     RCC_APB2PeriphClockCmd
#define            ADVANCE_TIM_CLK               RCC_APB2Periph_TIM1
// PWM �źŵ�Ƶ�� F = TIM_CLK/{(ARR+1)*(PSC+1)}
#define            ADVANCE_TIM_PERIOD            (8-1)
#define            ADVANCE_TIM_PSC               (9-1)
#define            ADVANCE_TIM_PULSE             4

#define            ADVANCE_TIM_IRQ               TIM1_UP_IRQn
#define            ADVANCE_TIM_IRQHandler        TIM1_UP_IRQHandler

// TIM1 ����Ƚ�ͨ��
#define            ADVANCE_TIM_CH1_GPIO_CLK      RCC_APB2Periph_GPIOA
#define            ADVANCE_TIM_CH1_PORT          GPIOA
#define            ADVANCE_TIM_CH1_PIN           GPIO_Pin_8

// TIM1 ����Ƚ�ͨ���Ļ���ͨ��
#define            ADVANCE_TIM_CH1N_GPIO_CLK      RCC_APB2Periph_GPIOB
#define            ADVANCE_TIM_CH1N_PORT          GPIOB
#define            ADVANCE_TIM_CH1N_PIN           GPIO_Pin_13

// TIM1 ����Ƚ�ͨ����ɲ��ͨ��
#define            ADVANCE_TIM_BKIN_GPIO_CLK      RCC_APB2Periph_GPIOB
#define            ADVANCE_TIM_BKIN_PORT          GPIOB
#define            ADVANCE_TIM_BKIN_PIN           GPIO_Pin_12

#endif

#if Advance_TIM_PWM_INPUT

/************�߼���ʱ��TIM�������壬ֻ��TIM1��TIM8************/
// ��ʹ�ò�ͬ�Ķ�ʱ����ʱ�򣬶�Ӧ��GPIO�ǲ�һ���ģ����Ҫע��
// ��������ʹ�ø߼����ƶ�ʱ��TIM1

#define            ADVANCE_TIM                   TIM1
#define            ADVANCE_TIM_APBxClock_FUN     RCC_APB2PeriphClockCmd
#define            ADVANCE_TIM_CLK               RCC_APB2Periph_TIM1

// ���벶���ܲ��񵽵���С��Ƶ��Ϊ 72M/{ (ARR+1)*(PSC+1) }
#define            ADVANCE_TIM_PERIOD            (1000-1)
#define            ADVANCE_TIM_PSC               (72-1)

// �ж���غ궨��
#define            ADVANCE_TIM_IRQ               TIM1_CC_IRQn
#define            ADVANCE_TIM_IRQHandler        TIM1_CC_IRQHandler

// TIM1 ���벶��ͨ��1
#define            ADVANCE_TIM_CH1_GPIO_CLK      RCC_APB2Periph_GPIOA
#define            ADVANCE_TIM_CH1_PORT          GPIOA
#define            ADVANCE_TIM_CH1_PIN           GPIO_Pin_8

#define            ADVANCE_TIM_IC1PWM_CHANNEL    TIM_Channel_1
#define            ADVANCE_TIM_IC2PWM_CHANNEL    TIM_Channel_2

#endif


#if Advance_TIM_DMA
/* ��ʱ�� */
#define ADVANCE_TIM           		    TIM1
#define ADVANCE_TIM_CLK       		    RCC_APB2Periph_TIM1
                           

// TIM1 ����Ƚ�ͨ��
#define ADVANCE_TIM_CH1_GPIO_CLK      RCC_APB2Periph_GPIOA
#define ADVANCE_TIM_CH1_PORT          GPIOA
#define ADVANCE_TIM_CH1_PIN           GPIO_Pin_8

/* DMA  */
#define TIM_DMA_ADDRESS               TIM1_BASE+0x34
#define TIM_DMA_BUFSIZE               3
#define TIM_DMA_CLK                   RCC_AHBPeriph_DMA1
#define TIM_DMA_STREAM                DMA1_Channel2

#endif

/**************************��������********************************/
void BASIC_TIM_Init(void);
void GENERAL_TIM_Init_TIMING(void);
void GENERAL_TIM_Init_PWM(void);
void TIM2_PWM_Init(void);
void TIM3_PWM_Init(void);
void GENERAL_TIM_Init_INPUT(void);
void GENERAL_TIMx_Configuration_SYN(void);
void ADVANCE_TIM_Init(void);
void Advance_TIM_PWM_Init(void);
void ADVANCE_TIM_Init_CDZ(void);
void ADVANCE_TIM_Init_PWM_INPUT(void);
void ADVANCE_TIMx_Configuration_DMA(void);
#endif



