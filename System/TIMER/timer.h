#ifndef __TIMER_H
#define __TIMER_H
/********************定时器功能：定时、输入捕获、输出比较、死区、互补（高级定时器独有）、PWM************/
//stm32f103c8t6中没有基本定时器、只有TIM1-TIM4：分别是高级定时器和通用定时器

/*
类型	       		型号示例									位数									主要功能																																							典型用途
基本定时器			TIM6, TIM7							16-bit				- 基础向上计数				- 仅有时基功能，无输入/输出通道																- 触发 DAC 同步输出	简单定时任务、产生周期中断、触发 DAC
通用定时器			TIM2, TIM3, TIM4, TIM5	16/32-bit			- 向上/向下计数- 输入捕获、输出比较- PWM 生成（边沿对齐、中心对齐）- 编码器接口			- 外部事件触发及级联通信	电机控制、脉冲测量、PWM 信号生成、编码器接口
高级控制定时器	TIM1, TIM8							16-bit				- 所有通用定时器功能- 互补输出带死区控制- 紧急停止输入- 高级编码器接口							- 同步精度更高	三相电机 FOC 控制、复杂 PWM 波（如带死区的 H 桥控制）、高精度 PWM

定时器时钟源频率 (TIM_CLK)		预分频系数 (PSC) 		自动重载值 (ARR) = TIM_Period 
定时时间 = ((PSC + 1) x (ARR + 1)) / TIM_CLK  (s)
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
/********************基本定时器TIM参数定义，只限TIM6、7************/
#define BASIC_TIM6 // 如果使用TIM7，注释掉这个宏即可
/*72M/(71+1) = 1MHz ARR = 999,每计数一千数就产生一次中断*/
#ifdef  BASIC_TIM6 // 使用基本定时器TIM6
#define            BASIC_TIM                   TIM6
#define            BASIC_TIM_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define            BASIC_TIM_CLK               RCC_APB1Periph_TIM6
#define            BASIC_TIM_Period            1000-1
#define            BASIC_TIM_Prescaler         71
#define            BASIC_TIM_IRQ               TIM6_IRQn
#define            BASIC_TIM_IRQHandler        TIM6_IRQHandler

#else  // 使用基本定时器TIM7
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
/**************通用定时器TIM参数定义，只限TIM2、3、4、5************/
// 当需要哪个定时器的时候，只需要把下面的宏定义改成1即可
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
/************通用定时器TIM参数定义，只限TIM2、3、4、5************/
// 当使用不同的定时器的时候，对应的GPIO是不一样的，这点要注意
// 我们这里默认使用TIM3

#define            GENERAL_TIM_PWM                   TIM3
#define            GENERAL_TIM_APBxClock_FUN_PWM     RCC_APB1PeriphClockCmd
#define            GENERAL_TIM_CLK_PWM               RCC_APB1Periph_TIM3
#define            GENERAL_TIM_Period_PWM            0      //频率配置50hz -> ARR
#define            GENERAL_TIM_Prescaler_PWM         1440
// TIM3 输出比较通道1
#define            GENERAL_TIM_CH1_GPIO_CLK      RCC_APB2Periph_GPIOA
#define            GENERAL_TIM_CH1_PORT          GPIOA
#define            GENERAL_TIM_CH1_PIN           GPIO_Pin_6

// TIM3 输出比较通道2
#define            GENERAL_TIM_CH2_GPIO_CLK      RCC_APB2Periph_GPIOA
#define            GENERAL_TIM_CH2_PORT          GPIOA
#define            GENERAL_TIM_CH2_PIN           GPIO_Pin_7

// TIM3 输出比较通道3
#define            GENERAL_TIM_CH3_GPIO_CLK      RCC_APB2Periph_GPIOB
#define            GENERAL_TIM_CH3_PORT          GPIOB
#define            GENERAL_TIM_CH3_PIN           GPIO_Pin_0

// TIM3 输出比较通道4
#define            GENERAL_TIM_CH4_GPIO_CLK      RCC_APB2Periph_GPIOB
#define            GENERAL_TIM_CH4_PORT          GPIOB
#define            GENERAL_TIM_CH4_PIN           GPIO_Pin_1


#define            GENERAL_TIM_IRQ_PWM              TIM3_IRQn
#define            GENERAL_TIM_IRQHandler_PWM       TIM3_IRQHandler

#define PWM_Period  1440

#endif


#if General_TIM_INPUT
/************通用定时器TIM参数定义，只限TIM2、3、4、5************/
// 当使用不同的定时器的时候，对应的GPIO是不一样的，这点要注意
// 我们这里默认使用TIM5

#define            GENERAL_TIM                   TIM5
#define            GENERAL_TIM_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define            GENERAL_TIM_CLK               RCC_APB1Periph_TIM5
#define            GENERAL_TIM_PERIOD            0XFFFF
#define            GENERAL_TIM_PSC              (72-1)

// TIM 输入捕获通道GPIO相关宏定义
#define            GENERAL_TIM_CH1_GPIO_CLK      RCC_APB2Periph_GPIOA
#define            GENERAL_TIM_CH1_PORT          GPIOA
#define            GENERAL_TIM_CH1_PIN           GPIO_Pin_0
#define            GENERAL_TIM_CHANNEL_x         TIM_Channel_1

// 中断相关宏定义
#define            GENERAL_TIM_IT_CCx            TIM_IT_CC1
#define            GENERAL_TIM_IRQ               TIM5_IRQn
#define            GENERAL_TIM_INT_FUN           TIM5_IRQHandler

// 获取捕获寄存器值函数宏定义
#define            GENERAL_TIM_GetCapturex_FUN                 TIM_GetCapture1
// 捕获信号极性函数宏定义
#define            GENERAL_TIM_OCxPolarityConfig_FUN           TIM_OC1PolarityConfig

// 测量的起始边沿
#define            GENERAL_TIM_STRAT_ICPolarity                TIM_ICPolarity_Rising
// 测量的结束边沿
#define            GENERAL_TIM_END_ICPolarity                  TIM_ICPolarity_Falling


// 定时器输入捕获用户自定义变量结构体声明
typedef struct
{   
	uint8_t   Capture_FinishFlag;   // 捕获结束标志位
	uint8_t   Capture_StartFlag;    // 捕获开始标志位
	uint16_t  Capture_CcrValue;     // 捕获寄存器的值
	uint16_t  Capture_Period;       // 自动重装载寄存器更新标志 
}TIM_ICUserValueTypeDef;

extern TIM_ICUserValueTypeDef TIM_ICUserValueStructure;

#endif

#if General_TIM_Synchronization

/* 定时器 */
#define GENERAL_TIM_MASTER     		    TIM2
#define GENERAL_TIM_MASTER_CLK        RCC_APB1Periph_TIM2

#define GENERAL_TIM_SLAVE1     		    TIM3
#define GENERAL_TIM_SLAVE1_CLK        RCC_APB1Periph_TIM3

#define GENERAL_TIM_SLAVE11     		  TIM4
#define GENERAL_TIM_SLAVE11_CLK       RCC_APB1Periph_TIM4

/* TIM主模式PWM输出引脚 */
#define GENERAL_MASTER_PIN            GPIO_Pin_10             
#define GENERAL_MASTER_GPIO_PORT      GPIOB                      
#define GENERAL_MASTER_GPIO_CLK       RCC_APB2Periph_GPIOB
#define GENERAL_MASTER_PINSOURCE			GPIO_PinSource5
#define GENERAL_MASTER_AF							GPIO_AF_TIM2

/* TIM从模式PWM输出引脚 */
#define GENERAL_SLAVE1_PIN            GPIO_Pin_6             
#define GENERAL_SLAVE1_GPIO_PORT      GPIOC                      
#define GENERAL_SLAVE1_GPIO_CLK       RCC_APB2Periph_GPIOC
#define GENERAL_SLAVE1_PINSOURCE			GPIO_PinSource6
#define GENERAL_SLAVE1_AF							GPIO_AF_TIM3

/* TIM从模式的从模式PWM输出引脚 */
#define GENERAL_SLAVE11_PIN           GPIO_Pin_6             
#define GENERAL_SLAVE11_GPIO_PORT     GPIOB                      
#define GENERAL_SLAVE11_GPIO_CLK      RCC_APB2Periph_GPIOB
#define GENERAL_SLAVE11_PINSOURCE			GPIO_PinSource6
#define GENERAL_SLAVE11_AF						GPIO_AF_TIM4


#endif

#if Avance_TIM

/********************高级定时器TIM参数定义，只限TIM1、8************/
#define ADVANCE_TIM1 // 如果使用TIM8，注释掉这个宏即可

#ifdef  ADVANCE_TIM1 // 使用高级定时器TIM1

#define            ADVANCE_TIM                   TIM1
#define            ADVANCE_TIM_APBxClock_FUN     RCC_APB2PeriphClockCmd
#define            ADVANCE_TIM_CLK               RCC_APB2Periph_TIM1
#define            ADVANCE_TIM_Period            (1000-1)
#define            ADVANCE_TIM_Prescaler         72-1
#define            ADVANCE_TIM_IRQ               TIM1_UP_IRQn
#define            ADVANCE_TIM_IRQHandler        TIM1_UP_IRQHandler

#else  // 使用高级定时器TIM8
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
// PWM 信号的频率 F = TIM_CLK/{(ARR+1)*(PSC+1)}
#define            ADVANCE_TIM_PERIOD            (20000-1)
#define            ADVANCE_TIM_PSC               (72-1)
#define            ADVANCE_TIM_PULSE             ADVANCE_TIM_PULSE_Servo_SetAngle1()

#define            ADVANCE_TIM_IRQ               TIM1_UP_IRQn
#define            ADVANCE_TIM_IRQHandler        TIM1_UP_IRQHandler

// TIM1 输出比较通道
#define            ADVANCE_TIM_CH1_GPIO_CLK      RCC_APB2Periph_GPIOA
#define            ADVANCE_TIM_CH1_PORT          GPIOA
#define            ADVANCE_TIM_CH1_PIN           GPIO_Pin_8

// TIM1 输出比较通道
#define            ADVANCE_TIM_CH2_GPIO_CLK      RCC_APB2Periph_GPIOA
#define            ADVANCE_TIM_CH2_PORT          GPIOA
#define            ADVANCE_TIM_CH2_PIN           GPIO_Pin_9

// TIM1 输出比较通道
#define            ADVANCE_TIM_CH4_GPIO_CLK      RCC_APB2Periph_GPIOA
#define            ADVANCE_TIM_CH4_PORT          GPIOA
#define            ADVANCE_TIM_CH4_PIN           GPIO_Pin_11

// TIM1 输出比较通道
#define            ADVANCE_TIM_CH3_GPIO_CLK      RCC_APB2Periph_GPIOA
#define            ADVANCE_TIM_CH3_PORT          GPIOA
#define            ADVANCE_TIM_CH3_PIN           GPIO_Pin_10

#endif


#if Advance_TIM_Complementary_DeadZone

/************高级定时器TIM参数定义，只限TIM1和TIM8************/
// 当使用不同的定时器的时候，对应的GPIO是不一样的，这点要注意
// 这里我们使用高级控制定时器TIM1

#define            ADVANCE_TIM                   TIM1
#define            ADVANCE_TIM_APBxClock_FUN     RCC_APB2PeriphClockCmd
#define            ADVANCE_TIM_CLK               RCC_APB2Periph_TIM1
// PWM 信号的频率 F = TIM_CLK/{(ARR+1)*(PSC+1)}
#define            ADVANCE_TIM_PERIOD            (8-1)
#define            ADVANCE_TIM_PSC               (9-1)
#define            ADVANCE_TIM_PULSE             4

#define            ADVANCE_TIM_IRQ               TIM1_UP_IRQn
#define            ADVANCE_TIM_IRQHandler        TIM1_UP_IRQHandler

// TIM1 输出比较通道
#define            ADVANCE_TIM_CH1_GPIO_CLK      RCC_APB2Periph_GPIOA
#define            ADVANCE_TIM_CH1_PORT          GPIOA
#define            ADVANCE_TIM_CH1_PIN           GPIO_Pin_8

// TIM1 输出比较通道的互补通道
#define            ADVANCE_TIM_CH1N_GPIO_CLK      RCC_APB2Periph_GPIOB
#define            ADVANCE_TIM_CH1N_PORT          GPIOB
#define            ADVANCE_TIM_CH1N_PIN           GPIO_Pin_13

// TIM1 输出比较通道的刹车通道
#define            ADVANCE_TIM_BKIN_GPIO_CLK      RCC_APB2Periph_GPIOB
#define            ADVANCE_TIM_BKIN_PORT          GPIOB
#define            ADVANCE_TIM_BKIN_PIN           GPIO_Pin_12

#endif

#if Advance_TIM_PWM_INPUT

/************高级定时器TIM参数定义，只限TIM1和TIM8************/
// 当使用不同的定时器的时候，对应的GPIO是不一样的，这点要注意
// 这里我们使用高级控制定时器TIM1

#define            ADVANCE_TIM                   TIM1
#define            ADVANCE_TIM_APBxClock_FUN     RCC_APB2PeriphClockCmd
#define            ADVANCE_TIM_CLK               RCC_APB2Periph_TIM1

// 输入捕获能捕获到的最小的频率为 72M/{ (ARR+1)*(PSC+1) }
#define            ADVANCE_TIM_PERIOD            (1000-1)
#define            ADVANCE_TIM_PSC               (72-1)

// 中断相关宏定义
#define            ADVANCE_TIM_IRQ               TIM1_CC_IRQn
#define            ADVANCE_TIM_IRQHandler        TIM1_CC_IRQHandler

// TIM1 输入捕获通道1
#define            ADVANCE_TIM_CH1_GPIO_CLK      RCC_APB2Periph_GPIOA
#define            ADVANCE_TIM_CH1_PORT          GPIOA
#define            ADVANCE_TIM_CH1_PIN           GPIO_Pin_8

#define            ADVANCE_TIM_IC1PWM_CHANNEL    TIM_Channel_1
#define            ADVANCE_TIM_IC2PWM_CHANNEL    TIM_Channel_2

#endif


#if Advance_TIM_DMA
/* 定时器 */
#define ADVANCE_TIM           		    TIM1
#define ADVANCE_TIM_CLK       		    RCC_APB2Periph_TIM1
                           

// TIM1 输出比较通道
#define ADVANCE_TIM_CH1_GPIO_CLK      RCC_APB2Periph_GPIOA
#define ADVANCE_TIM_CH1_PORT          GPIOA
#define ADVANCE_TIM_CH1_PIN           GPIO_Pin_8

/* DMA  */
#define TIM_DMA_ADDRESS               TIM1_BASE+0x34
#define TIM_DMA_BUFSIZE               3
#define TIM_DMA_CLK                   RCC_AHBPeriph_DMA1
#define TIM_DMA_STREAM                DMA1_Channel2

#endif

/**************************函数声明********************************/
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



