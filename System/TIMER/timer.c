#include "timer.h"

/*********************************************基本定时器**********************/
// 基本定时器TIMx,x[6,7]定时初始化函数
// 中断优先级配置
#if Basic_TIM
static void BASIC_TIM_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    // 设置中断组为0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);		
		// 设置中断来源
    NVIC_InitStructure.NVIC_IRQChannel = BASIC_TIM_IRQ ;	
		// 设置主优先级为 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
	  // 设置抢占优先级为3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*
 * 注意：TIM_TimeBaseInitTypeDef结构体里面有5个成员，TIM6和TIM7的寄存器里面只有
 * TIM_Prescaler和TIM_Period，所以使用TIM6和TIM7的时候只需初始化这两个成员即可，
 * 另外三个成员是通用定时器和高级定时器才有.
 *-----------------------------------------------------------------------------
 *typedef struct
 *{ TIM_Prescaler            都有
 *	TIM_CounterMode			     TIMx,x[6,7]没有，其他都有
 *  TIM_Period               都有
 *  TIM_ClockDivision        TIMx,x[6,7]没有，其他都有
 *  TIM_RepetitionCounter    TIMx,x[1,8,15,16,17]才有
 *}TIM_TimeBaseInitTypeDef; 
 *-----------------------------------------------------------------------------
 */


static void BASIC_TIM_Mode_Config(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		
		// 开启定时器时钟,即内部时钟CK_INT=72M
    BASIC_TIM_APBxClock_FUN(BASIC_TIM_CLK, ENABLE);
	
		// 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
    TIM_TimeBaseStructure.TIM_Period = BASIC_TIM_Period;	

	  // 时钟预分频数为
    TIM_TimeBaseStructure.TIM_Prescaler= BASIC_TIM_Prescaler;
	
		// 时钟分频因子 ，基本定时器没有，不用管
    //TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
		
		// 计数器计数模式，基本定时器只能向上计数，没有计数模式的设置
    //TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
		
		// 重复计数器的值，基本定时器没有，不用管
		//TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
	
	  // 初始化定时器
    TIM_TimeBaseInit(BASIC_TIM, &TIM_TimeBaseStructure);
		
		// 清除计数器中断标志位
    TIM_ClearFlag(BASIC_TIM, TIM_FLAG_Update);
	  
		// 开启计数器中断
    TIM_ITConfig(BASIC_TIM,TIM_IT_Update,ENABLE);
		
		// 使能计数器
    TIM_Cmd(BASIC_TIM, ENABLE);	
}

void BASIC_TIM_Init(void)
{
	BASIC_TIM_NVIC_Config();
	BASIC_TIM_Mode_Config();
}
#endif
/*********************************************基本定时器**********************/

/*********************************************高级定时器**********************/
#if General_TIM
// 中断优先级配置
static void GENERAL_TIM_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    // 设置中断组为0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);		
		// 设置中断来源
    NVIC_InitStructure.NVIC_IRQChannel = GENERAL_TIM_IRQ ;	
		// 设置主优先级为 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
	  // 设置抢占优先级为3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

///*
// * 注意：TIM_TimeBaseInitTypeDef结构体里面有5个成员，TIM6和TIM7的寄存器里面只有
// * TIM_Prescaler和TIM_Period，所以使用TIM6和TIM7的时候只需初始化这两个成员即可，
// * 另外三个成员是通用定时器和高级定时器才有.
// *-----------------------------------------------------------------------------
// *typedef struct
// *{ TIM_Prescaler            都有
// *	TIM_CounterMode			     TIMx,x[6,7]没有，其他都有
// *  TIM_Period               都有
// *  TIM_ClockDivision        TIMx,x[6,7]没有，其他都有
// *  TIM_RepetitionCounter    TIMx,x[1,8,15,16,17]才有
// *}TIM_TimeBaseInitTypeDef; 
// *-----------------------------------------------------------------------------
// */
static void GENERAL_TIM_Mode_Config_TIMING(void)
{
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;		
		// 开启定时器时钟,即内部时钟CK_INT=72M
    GENERAL_TIM_APBxClock_FUN(GENERAL_TIM_CLK, ENABLE);	
		// 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
    TIM_TimeBaseStructure.TIM_Period=GENERAL_TIM_Period;
	  // 时钟预分频数
    TIM_TimeBaseStructure.TIM_Prescaler= GENERAL_TIM_Prescaler;	
		// 时钟分频因子 ，没用到不用管
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		
		// 计数器计数模式，设置为向上计数
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 		
		// 重复计数器的值，没用到不用管
		TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	  // 初始化定时器
    TIM_TimeBaseInit(GENERAL_TIM, &TIM_TimeBaseStructure);
	
		// 清除计数器中断标志位
    TIM_ClearFlag(GENERAL_TIM, TIM_FLAG_Update);
											//TIM_TimeBaseInit函数末尾，手动产生了更新事件
											//若不清除此标志位，则开启中断后，会立刻进入一次中断
											//如果不介意此问题，则不清除此标志位也可
	
	  
		// 开启计数器中断
    TIM_ITConfig(GENERAL_TIM,TIM_IT_Update,ENABLE);
		
		// 使能计数器
    TIM_Cmd(GENERAL_TIM, ENABLE);
}

void GENERAL_TIM_Init_TIMING(void)
{
	GENERAL_TIM_NVIC_Config();
	GENERAL_TIM_Mode_Config_TIMING();		
}
#endif
/*********************************************高级定时器**********************/

/*********************************************输出4路PWM**********************/
#if General_TIM_PWM
static void GENERAL_TIM_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

  // 输出比较通道1 GPIO 初始化
	RCC_APB2PeriphClockCmd(GENERAL_TIM_CH1_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GENERAL_TIM_CH1_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GENERAL_TIM_CH1_PORT, &GPIO_InitStructure);
	
	// 输出比较通道2 GPIO 初始化
	RCC_APB2PeriphClockCmd(GENERAL_TIM_CH2_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GENERAL_TIM_CH2_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GENERAL_TIM_CH2_PORT, &GPIO_InitStructure);
	
	// 输出比较通道3 GPIO 初始化
	RCC_APB2PeriphClockCmd(GENERAL_TIM_CH3_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GENERAL_TIM_CH3_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GENERAL_TIM_CH3_PORT, &GPIO_InitStructure);
	
	// 输出比较通道4 GPIO 初始化
	RCC_APB2PeriphClockCmd(GENERAL_TIM_CH4_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GENERAL_TIM_CH4_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GENERAL_TIM_CH4_PORT, &GPIO_InitStructure);
}


///*
// * 注意：TIM_TimeBaseInitTypeDef结构体里面有5个成员，TIM6和TIM7的寄存器里面只有
// * TIM_Prescaler和TIM_Period，所以使用TIM6和TIM7的时候只需初始化这两个成员即可，
// * 另外三个成员是通用定时器和高级定时器才有.
// *-----------------------------------------------------------------------------
// *typedef struct
// *{ TIM_Prescaler            都有
// *	TIM_CounterMode			     TIMx,x[6,7]没有，其他都有
// *  TIM_Period               都有
// *  TIM_ClockDivision        TIMx,x[6,7]没有，其他都有
// *  TIM_RepetitionCounter    TIMx,x[1,8,15,16,17]才有
// *}TIM_TimeBaseInitTypeDef; 
// *-----------------------------------------------------------------------------
// */

/* ----------------   PWM信号 周期和占空比的计算--------------- */
// ARR ：自动重装载寄存器的值
// CLK_cnt：计数器的时钟，等于 Fck_int / (psc+1) = 72M/(psc+1)
// PWM 信号的周期 T = ARR * (1/CLK_cnt) = ARR*(PSC+1) / 72M
// 占空比P=CCR/(ARR+1)

static void GENERAL_TIM_Mode_Config_PWM(void)
{
//		// 占空比配置
//	uint16_t CCR1_Val = 5;
//	uint16_t CCR2_Val = 5;
//	uint16_t CCR3_Val = 5;
//	uint16_t CCR4_Val = 5;
	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  // 开启定时器时钟,即内部时钟CK_INT=72M
	GENERAL_TIM_APBxClock_FUN_PWM(GENERAL_TIM_CLK_PWM,ENABLE);

/*--------------------时基结构体初始化-------------------------*/
	// 配置周期，这里配置为100K
	
	// 自动重装载寄存器的值，累计TIM_Period_PWM+1个频率后产生一个更新或者中断
	TIM_TimeBaseStructure.TIM_Period=GENERAL_TIM_Period_PWM;	
	// 驱动CNT计数器的时钟 = Fck_int/(psc+1)
	TIM_TimeBaseStructure.TIM_Prescaler= GENERAL_TIM_Prescaler_PWM;	
	// 时钟分频因子 ，配置死区时间时需要用到
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		
	// 计数器计数模式，设置为向上计数
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		
	// 重复计数器的值，没用到不用管
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	// 初始化定时器
	TIM_TimeBaseInit(GENERAL_TIM_PWM, &TIM_TimeBaseStructure);

}

static void GENERAL_TIM3_NVIC(void)
{
//配置定时器中断部分(TIM3)
	TIM_ClearFlag(TIM3,TIM_FLAG_Update);//避免刚初始化完就进入中断
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //使能指定的TIM3中断,允许更新中断

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置为PriorityGroup_2：先占优先级：2个  从占优先级：2个
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = GENERAL_TIM_IRQ_PWM;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;//先占优先级2级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //从优先级2级
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
	
	TIM_Cmd(TIM3, ENABLE);  //使能TIM3外设		
}


void GENERAL_TIM_Init_PWM(void)
{
	GENERAL_TIM_GPIO_Config();
	GENERAL_TIM_Mode_Config_PWM();	
	GENERAL_TIM3_NVIC();
}

void TIM2_PWM_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef   TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef         TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
	TIM_DeInit(TIM2);
	TIM_TimeBaseInitStructure.TIM_Prescaler=0;
	TIM_TimeBaseInitStructure.TIM_Period=1440-1;  //25KHz
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_CenterAligned1;
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1; 
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse=0;
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
	TIM_OC2Init(TIM2,&TIM_OCInitStructure);
	TIM_OC3Init(TIM2,&TIM_OCInitStructure);
	TIM_OC4Init(TIM2,&TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM2,TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM2,TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM2,TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(TIM2,ENABLE);
	TIM_Cmd(TIM2,ENABLE);
}

void TIM3_PWM_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef   TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef         TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	TIM_DeInit(TIM3);
	TIM_TimeBaseInitStructure.TIM_Prescaler=0;
	TIM_TimeBaseInitStructure.TIM_Period=1440-1;  //25KHz
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_CenterAligned1;
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1; 
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse=0;
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
	TIM_OC2Init(TIM3,&TIM_OCInitStructure);
	TIM_OC3Init(TIM3,&TIM_OCInitStructure);
	TIM_OC4Init(TIM3,&TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM3,TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM3,TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(TIM3,ENABLE);
	TIM_Cmd(TIM3,ENABLE);
}
#endif
/*********************************************输出4路PWM**********************/


/*********************************************输入捕获**********************/
#if General_TIM_INPUT

// 定时器输入捕获用户自定义变量结构体定义
TIM_ICUserValueTypeDef TIM_ICUserValueStructure = {0,0,0,0};

// 中断优先级配置
static void GENERAL_TIM_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    // 设置中断组为0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);		
		// 设置中断来源
    NVIC_InitStructure.NVIC_IRQChannel = GENERAL_TIM_IRQ ;	
		// 设置主优先级为 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
	  // 设置抢占优先级为3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

static void GENERAL_TIM_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

  // 输入捕获通道 GPIO 初始化
	RCC_APB2PeriphClockCmd(GENERAL_TIM_CH1_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GENERAL_TIM_CH1_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GENERAL_TIM_CH1_PORT, &GPIO_InitStructure);	
}


///*
// * 注意：TIM_TimeBaseInitTypeDef结构体里面有5个成员，TIM6和TIM7的寄存器里面只有
// * TIM_Prescaler和TIM_Period，所以使用TIM6和TIM7的时候只需初始化这两个成员即可，
// * 另外三个成员是通用定时器和高级定时器才有.
// *-----------------------------------------------------------------------------
// *typedef struct
// *{ TIM_Prescaler            都有
// *	TIM_CounterMode			     TIMx,x[6,7]没有，其他都有
// *  TIM_Period               都有
// *  TIM_ClockDivision        TIMx,x[6,7]没有，其他都有
// *  TIM_RepetitionCounter    TIMx,x[1,8,15,16,17]才有
// *}TIM_TimeBaseInitTypeDef; 
// *-----------------------------------------------------------------------------
// */

/* ----------------   PWM信号 周期和占空比的计算--------------- */
// ARR ：自动重装载寄存器的值
// CLK_cnt：计数器的时钟，等于 Fck_int / (psc+1) = 72M/(psc+1)
// PWM 信号的周期 T = ARR * (1/CLK_cnt) = ARR*(PSC+1) / 72M
// 占空比P=CCR/(ARR+1)

static void GENERAL_TIM_Mode_Config(void)
{
	/*--------------------时基结构体初始化-------------------------*/	
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	/*--------------------输入捕获结构体初始化-------------------*/	
	TIM_ICInitTypeDef TIM_ICInitStructure;
  // 开启定时器时钟,即内部时钟CK_INT=72M
	GENERAL_TIM_APBxClock_FUN(GENERAL_TIM_CLK,ENABLE);

	// 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
	TIM_TimeBaseStructure.TIM_Period=GENERAL_TIM_PERIOD;	
	// 驱动CNT计数器的时钟 = Fck_int/(psc+1)
	TIM_TimeBaseStructure.TIM_Prescaler= GENERAL_TIM_PSC;	
	// 时钟分频因子 ，配置死区时间时需要用到
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		
	// 计数器计数模式，设置为向上计数
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		
	// 重复计数器的值，没用到不用管
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	// 初始化定时器
	TIM_TimeBaseInit(GENERAL_TIM, &TIM_TimeBaseStructure);

	// 配置输入捕获的通道，需要根据具体的GPIO来配置
	TIM_ICInitStructure.TIM_Channel = GENERAL_TIM_CHANNEL_x;
	// 输入捕获信号的极性配置
	TIM_ICInitStructure.TIM_ICPolarity = GENERAL_TIM_STRAT_ICPolarity;
	// 输入通道和捕获通道的映射关系，有直连和非直连两种
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	// 输入的需要被捕获的信号的分频系数
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	// 输入的需要被捕获的信号的滤波系数
	TIM_ICInitStructure.TIM_ICFilter = 0;
	// 定时器输入捕获初始化
	TIM_ICInit(GENERAL_TIM, &TIM_ICInitStructure);
	
	// 清除更新和捕获中断标志位
  TIM_ClearFlag(GENERAL_TIM, TIM_FLAG_Update|GENERAL_TIM_IT_CCx);	
  // 开启更新和捕获中断  
	TIM_ITConfig (GENERAL_TIM, TIM_IT_Update | GENERAL_TIM_IT_CCx, ENABLE );
	
	// 使能计数器
	TIM_Cmd(GENERAL_TIM, ENABLE);
}

void GENERAL_TIM_Init_INPUT(void)
{
	GENERAL_TIM_GPIO_Config();
	GENERAL_TIM_NVIC_Config();
	GENERAL_TIM_Mode_Config();		
}
#endif
/*********************************************输入捕获**********************/

/*********************************************定时器之间的同步**********************/
#if General_TIM_Synchronization

/**
  * @brief  配置TIM复用输出PWM时用到的I/O
  * @param  无
  * @retval 无
  */
static void TIMx_GPIO_Config(void) 
{
	GPIO_InitTypeDef GPIO_InitStructure;

  /* 使能GPIO时钟 */
  RCC_APB2PeriphClockCmd(GENERAL_MASTER_GPIO_CLK |GENERAL_SLAVE1_GPIO_CLK | GENERAL_SLAVE11_GPIO_CLK, ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//使能AFIO时钟
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE);//完全重映像TIM2_CH3->PB10
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE);//完全重映像TIM3_CH1->PC6

  /* GPIO 配置: PC6(TIM3 CH1) 作为推挽复用输出功能 ------*/
  GPIO_InitStructure.GPIO_Pin = GENERAL_SLAVE1_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;;
  GPIO_Init(GENERAL_SLAVE1_GPIO_PORT, &GPIO_InitStructure);

  /* GPIO 配置: PB6(TIM4 CH1) 作为推挽复用输出功能 ------*/
  GPIO_InitStructure.GPIO_Pin = GENERAL_SLAVE11_PIN;
  GPIO_Init(GENERAL_SLAVE11_GPIO_PORT, &GPIO_InitStructure);

  /* GPIO 配置: PB10(TIM2 CH3) 作为推挽复用输出功能 ------*/
  GPIO_InitStructure.GPIO_Pin = GENERAL_MASTER_PIN;
  GPIO_Init(GENERAL_MASTER_GPIO_PORT, &GPIO_InitStructure);

}

/*
 * 注意：TIM_TimeBaseInitTypeDef结构体里面有5个成员，TIM6和TIM7的寄存器里面只有
 * TIM_Prescaler和TIM_Period，所以使用TIM6和TIM7的时候只需初始化这两个成员即可，
 * 另外三个成员是通用定时器和高级定时器才有.
 *-----------------------------------------------------------------------------
 * TIM_Prescaler         都有
 * TIM_CounterMode			 TIMx,x[6,7]没有，其他都有（基本定时器）
 * TIM_Period            都有
 * TIM_ClockDivision     TIMx,x[6,7]没有，其他都有(基本定时器)
 * TIM_RepetitionCounter TIMx,x[1,8]才有(高级定时器)
 *-----------------------------------------------------------------------------
 */
static void TIM_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	  /* 使能TIM时钟  */
  RCC_APB1PeriphClockCmd(GENERAL_TIM_MASTER_CLK | GENERAL_TIM_SLAVE1_CLK |
                         GENERAL_TIM_SLAVE11_CLK, ENABLE);
  /* Timers synchronisation in cascade mode ------------------------------------
     1/TIM2 is configured as Master Timer:
         - PWM Mode is used
         - The TIM2 Update event is used as Trigger Output  

     2/TIM3 is slave for TIM2 and Master for TIM4,
         - PWM Mode is used
         - The ITR1(TIM2) is used as input trigger 
         - Gated mode is used, so start and stop of slave counter
           are controlled by the Master trigger output signal(TIM2 update event).
         - The TIM3 Update event is used as Trigger Output. 

     3/TIM4 is slave for TIM3,
         - PWM Mode is used
         - The ITR2(TIM3) is used as input trigger
         - Gated mode is used, so start and stop of slave counter
           are controlled by the Master trigger output signal(TIM3 update event).

     In this example TIM2 input clock (TIM2CLK) is set to 2 * APB1 clock (PCLK1), 
     since APB1 prescaler is different from 1.   
     TIM2CLK = 2 * PCLK1  
     PCLK1 = HCLK / 4 
     => TIM2CLK = HCLK / 2 = SystemCoreClock /2

     The Master Timer TIM2 is running at TIM2 counter clock:
     TIM2 frequency = (TIM2 counter clock)/ (TIM2 period + 1) = 500 KHz 
     and the duty cycle = TIM2_CCR1/(TIM2_ARR + 1) = 25%.

     The TIM3 is running:
     - At (TIM2 frequency)/ (TIM3 period + 1) = 100 KHz and a duty cycle
       equal to TIM3_CCR1/(TIM3_ARR + 1) = 20%

     The TIM4 is running:
     - At (TIM3 frequency)/ (TIM4 period + 1) = 20 KHz and a duty cycle
       equal to TIM4_CCR1/(TIM4_ARR + 1) = 20%
  
     Note: 
     SystemCoreClock variable holds HCLK frequency and is defined in system_stm32f10x.c file.
     Each time the core clock (HCLK) changes, user had to call SystemCoreClockUpdate()
     function to update SystemCoreClock variable value. Otherwise, any configuration
     based on this variable will be incorrect. 
  --------------------------------------------------------------------------- */

  /* 时基配置 */
  TIM_TimeBaseStructure.TIM_Period = 72;
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(GENERAL_TIM_MASTER, &TIM_TimeBaseStructure);

  TIM_TimeBaseStructure.TIM_Period = 4;
  TIM_TimeBaseInit(GENERAL_TIM_SLAVE1, &TIM_TimeBaseStructure);

  TIM_TimeBaseStructure.TIM_Period = 4;
  TIM_TimeBaseInit(GENERAL_TIM_SLAVE11, &TIM_TimeBaseStructure);

  /* 主 设置成PWM1模式 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 36;//占空比
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC3Init(GENERAL_TIM_MASTER, &TIM_OCInitStructure);//初始化 TIM2 输出通道3
  /* 使能“主从模式” */
  TIM_SelectMasterSlaveMode(GENERAL_TIM_MASTER, TIM_MasterSlaveMode_Enable);
	/* 输出触发源选项 */
  TIM_SelectOutputTrigger(GENERAL_TIM_MASTER, TIM_TRGOSource_Update);

	/* 从 设置成PWM1模式 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 1;
  TIM_OC1Init(GENERAL_TIM_SLAVE1, &TIM_OCInitStructure);//初始化 TIM3 输出通道1
	TIM_OC1Init(GENERAL_TIM_SLAVE11, &TIM_OCInitStructure);//初始化 TIM4 输出通道1
  /* 使能“主从模式” */
  TIM_SelectMasterSlaveMode(GENERAL_TIM_SLAVE1, TIM_MasterSlaveMode_Enable);
  /* 输出触发源选项 */
  TIM_SelectOutputTrigger(GENERAL_TIM_SLAVE1, TIM_TRGOSource_Update);
	
  TIM_SelectSlaveMode(GENERAL_TIM_SLAVE1, TIM_SlaveMode_Gated);//从模式配置 : TIM3
  TIM_SelectInputTrigger(GENERAL_TIM_SLAVE1, TIM_TS_ITR1);//设置输入触发源
	
  TIM_SelectSlaveMode(TIM4, TIM_SlaveMode_Gated);//从模式配置 : TIM4
  TIM_SelectInputTrigger(GENERAL_TIM_SLAVE11, TIM_TS_ITR2);//设置输入触发源
  
  /* TIM 计数器溢出中断启用 */
  TIM_Cmd(GENERAL_TIM_SLAVE1,ENABLE);
  TIM_Cmd(GENERAL_TIM_MASTER,ENABLE);
  TIM_Cmd(GENERAL_TIM_SLAVE11,ENABLE);
}

/**
  * @brief  初始化通用定时器级联同步PWM输出
  * @param  无
  * @retval 无
  */
void GENERAL_TIMx_Configuration_SYN(void)
{  
	TIMx_GPIO_Config();
	
  TIM_Mode_Config();
}

/*********************************************定时器之间的同步**********************/

#endif

#if Adance_TIM

// 中断优先级配置
static void ADVANCE_TIM_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    // 设置中断组为0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);		
		// 设置中断来源
    NVIC_InitStructure.NVIC_IRQChannel = ADVANCE_TIM_IRQ ;	
		// 设置主优先级为 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
	  // 设置抢占优先级为3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

///*
// * 注意：TIM_TimeBaseInitTypeDef结构体里面有5个成员，TIM6和TIM7的寄存器里面只有
// * TIM_Prescaler和TIM_Period，所以使用TIM6和TIM7的时候只需初始化这两个成员即可，
// * 另外三个成员是通用定时器和高级定时器才有.
// *-----------------------------------------------------------------------------
// *typedef struct
// *{ TIM_Prescaler            都有
// *	TIM_CounterMode			     TIMx,x[6,7]没有，其他都有
// *  TIM_Period               都有
// *  TIM_ClockDivision        TIMx,x[6,7]没有，其他都有
// *  TIM_RepetitionCounter    TIMx,x[1,8,15,16,17]才有
// *}TIM_TimeBaseInitTypeDef; 
// *-----------------------------------------------------------------------------
// */
static void ADVANCE_TIM_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;		
		// 开启定时器时钟,即内部时钟CK_INT=72M
    ADVANCE_TIM_APBxClock_FUN(ADVANCE_TIM_CLK, ENABLE);	
		// 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
    TIM_TimeBaseStructure.TIM_Period=ADVANCE_TIM_Period;
	  // 时钟预分频数
    TIM_TimeBaseStructure.TIM_Prescaler= ADVANCE_TIM_Prescaler;	
		// 时钟分频因子 ，没用到不用管
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		
		// 计数器计数模式，设置为向上计数
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 		
		// 重复计数器的值，没用到不用管
		TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	  // 初始化定时器
    TIM_TimeBaseInit(ADVANCE_TIM, &TIM_TimeBaseStructure);
	
		// 清除计数器中断标志位
    TIM_ClearFlag(ADVANCE_TIM, TIM_FLAG_Update);
	  
		// 开启计数器中断
    TIM_ITConfig(ADVANCE_TIM,TIM_IT_Update,ENABLE);
		
		// 使能计数器
    TIM_Cmd(ADVANCE_TIM, ENABLE);
}

void ADVANCE_TIM_Init(void)
{
	ADVANCE_TIM_NVIC_Config();
	ADVANCE_TIM_Mode_Config();		
}
/*********************************************高级定时器**********************/
#endif

#if Advance_TIM_PWM

void Advance_TIM_PWM_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

  // 输出比较通道 GPIO 初始化
	RCC_APB2PeriphClockCmd(ADVANCE_TIM_CH1_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  ADVANCE_TIM_CH1_PIN | ADVANCE_TIM_CH2_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(ADVANCE_TIM_CH1_PORT, &GPIO_InitStructure);

//  // 输出比较通道互补通道 GPIO 初始化
//	RCC_APB2PeriphClockCmd(ADVANCE_TIM_CH1N_GPIO_CLK, ENABLE);
//  GPIO_InitStructure.GPIO_Pin =  ADVANCE_TIM_CH1N_PIN;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_Init(ADVANCE_TIM_CH1N_PORT, &GPIO_InitStructure);
	
	
	// 开启定时器时钟,即内部时钟CK_INT=72M
	ADVANCE_TIM_APBxClock_FUN(ADVANCE_TIM_CLK,ENABLE);

/*--------------------时基结构体初始化-------------------------*/
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	// 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
	TIM_TimeBaseStructure.TIM_Period=ADVANCE_TIM_PERIOD;	
	// 驱动CNT计数器的时钟 = Fck_int/(psc+1)
	TIM_TimeBaseStructure.TIM_Prescaler= ADVANCE_TIM_PSC;	
	// 时钟分频因子 ，配置死区时间时需要用到
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		
	// 计数器计数模式，设置为向上计数
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		
	// 重复计数器的值，没用到不用管
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	// 初始化定时器
	TIM_TimeBaseInit(ADVANCE_TIM, &TIM_TimeBaseStructure);

	/*--------------------输出比较结构体初始化-------------------*/		
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	// 配置为PWM模式1
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	// 输出使能
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	// 互补输出使能
//	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable; 
	// 设置占空比大小
//	TIM_OCInitStructure.TIM_Pulse = ADVANCE_TIM_PULSE;
	// 输出通道电平极性配置
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	// 互补输出通道电平极性配置
//	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	// 输出通道空闲电平极性配置
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	// 互补输出通道空闲电平极性配置
//	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	TIM_OC1Init(ADVANCE_TIM, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(ADVANCE_TIM, TIM_OCPreload_Enable);
		// 使能计数器
	TIM_Cmd(ADVANCE_TIM, ENABLE);	
	// 主输出使能，当使用的是通用定时器时，这句不需要
	TIM_CtrlPWMOutputs(ADVANCE_TIM, ENABLE);
}

#endif







/*********************************************高级定时器PWM互补死区**********************/
#if Advance_TIM_Complementary_DeadZone

static void ADVANCE_TIM_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

  // 输出比较通道 GPIO 初始化
	RCC_APB2PeriphClockCmd(ADVANCE_TIM_CH1_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  ADVANCE_TIM_CH1_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(ADVANCE_TIM_CH1_PORT, &GPIO_InitStructure);

  // 输出比较通道互补通道 GPIO 初始化
	RCC_APB2PeriphClockCmd(ADVANCE_TIM_CH1N_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  ADVANCE_TIM_CH1N_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(ADVANCE_TIM_CH1N_PORT, &GPIO_InitStructure);

  // 输出比较通道刹车通道 GPIO 初始化
	RCC_APB2PeriphClockCmd(ADVANCE_TIM_BKIN_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  ADVANCE_TIM_BKIN_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(ADVANCE_TIM_BKIN_PORT, &GPIO_InitStructure);
	// BKIN引脚默认先输出低电平
	GPIO_ResetBits(ADVANCE_TIM_BKIN_PORT,ADVANCE_TIM_BKIN_PIN);	
}


///*
// * 注意：TIM_TimeBaseInitTypeDef结构体里面有5个成员，TIM6和TIM7的寄存器里面只有
// * TIM_Prescaler和TIM_Period，所以使用TIM6和TIM7的时候只需初始化这两个成员即可，
// * 另外三个成员是通用定时器和高级定时器才有.
// *-----------------------------------------------------------------------------
// *typedef struct
// *{ TIM_Prescaler            都有
// *	TIM_CounterMode			     TIMx,x[6,7]没有，其他都有
// *  TIM_Period               都有
// *  TIM_ClockDivision        TIMx,x[6,7]没有，其他都有
// *  TIM_RepetitionCounter    TIMx,x[1,8,15,16,17]才有
// *}TIM_TimeBaseInitTypeDef; 
// *-----------------------------------------------------------------------------
// */

/* ----------------   PWM信号 周期和占空比的计算--------------- */
// ARR ：自动重装载寄存器的值
// CLK_cnt：计数器的时钟，等于 Fck_int / (psc+1) = 72M/(psc+1)
// PWM 信号的周期 T = (ARR+1) * (1/CLK_cnt) = (ARR+1)*(PSC+1) / 72M
// 占空比P=CCR/(ARR+1)

static void ADVANCE_TIM_Mode_Config(void)
{
	/*--------------------输出比较结构体初始化-------------------*/		
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	// 有关刹车和死区结构体的成员具体可参考BDTR寄存器的描述
	TIM_BDTRInitTypeDef TIM_BDTRInitStructure;
	/*--------------------时基结构体初始化-------------------------*/
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  // 开启定时器时钟,即内部时钟CK_INT=72M
	ADVANCE_TIM_APBxClock_FUN(ADVANCE_TIM_CLK,ENABLE);


	// 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
	TIM_TimeBaseStructure.TIM_Period=ADVANCE_TIM_PERIOD;	
	// 驱动CNT计数器的时钟 = Fck_int/(psc+1)
	TIM_TimeBaseStructure.TIM_Prescaler= ADVANCE_TIM_PSC;	
	// 时钟分频因子 ，配置死区时间时需要用到
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		
	// 计数器计数模式，设置为向上计数
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		
	// 重复计数器的值，没用到不用管
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	// 初始化定时器
	TIM_TimeBaseInit(ADVANCE_TIM, &TIM_TimeBaseStructure);


	// 配置为PWM模式1
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	// 输出使能
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	// 互补输出使能
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable; 
	// 设置占空比大小
	TIM_OCInitStructure.TIM_Pulse = ADVANCE_TIM_PULSE;
	// 输出通道电平极性配置
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	// 互补输出通道电平极性配置
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	// 输出通道空闲电平极性配置
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	// 互补输出通道空闲电平极性配置
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	TIM_OC1Init(ADVANCE_TIM, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(ADVANCE_TIM, TIM_OCPreload_Enable);

	/*-------------------刹车和死区结构体初始化-------------------*/

  TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;
  TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;
  TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_1;
	// 输出比较信号死区时间配置，具体如何计算可参考 BDTR:UTG[7:0]的描述
	// 这里配置的死区时间为152ns
  TIM_BDTRInitStructure.TIM_DeadTime = 11;
  TIM_BDTRInitStructure.TIM_Break = TIM_Break_Enable;
	// 当BKIN引脚检测到高电平的时候，输出比较信号被禁止，就好像是刹车一样
  TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;
  TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;
  TIM_BDTRConfig(ADVANCE_TIM, &TIM_BDTRInitStructure);
	
	// 使能计数器
	TIM_Cmd(ADVANCE_TIM, ENABLE);	
	// 主输出使能，当使用的是通用定时器时，这句不需要
	TIM_CtrlPWMOutputs(ADVANCE_TIM, ENABLE);
}

void ADVANCE_TIM_Init_CDZ(void)
{
	ADVANCE_TIM_GPIO_Config();
	ADVANCE_TIM_Mode_Config();		
}

/*********************************************高级定时器PWM互补死区**********************/
#endif

/*********************************************高级定时器PWM输入捕获**********************/
#if Advance_TIM_PWM_INPUT

 /**
  * @brief  高级控制定时器 TIMx,x[1,8]中断优先级配置
  * @param  无
  * @retval 无
  */
static void ADVANCE_TIM_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    // 设置中断组为0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);		
		// 设置中断来源
    NVIC_InitStructure.NVIC_IRQChannel = ADVANCE_TIM_IRQ; 	
		// 设置抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
	  // 设置子优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
/**
  * @brief  高级定时器PWM输入用到的GPIO初始化
  * @param  无
  * @retval 无
  */
static void ADVANCE_TIM_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(ADVANCE_TIM_CH1_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  ADVANCE_TIM_CH1_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
  GPIO_Init(ADVANCE_TIM_CH1_PORT, &GPIO_InitStructure);	
}


///*
// * 注意：TIM_TimeBaseInitTypeDef结构体里面有5个成员，TIM6和TIM7的寄存器里面只有
// * TIM_Prescaler和TIM_Period，所以使用TIM6和TIM7的时候只需初始化这两个成员即可，
// * 另外三个成员是通用定时器和高级定时器才有.
// *-----------------------------------------------------------------------------
// *typedef struct
// *{ TIM_Prescaler            都有
// *	TIM_CounterMode			     TIMx,x[6,7]没有，其他都有
// *  TIM_Period               都有
// *  TIM_ClockDivision        TIMx,x[6,7]没有，其他都有
// *  TIM_RepetitionCounter    TIMx,x[1,8,15,16,17]才有
// *}TIM_TimeBaseInitTypeDef; 
// *-----------------------------------------------------------------------------
// */

/* ----------------   PWM信号 周期和占空比的计算--------------- */
// ARR ：自动重装载寄存器的值
// CLK_cnt：计数器的时钟，等于 Fck_int / (psc+1) = 72M/(psc+1)
// PWM 信号的周期 T = ARR * (1/CLK_cnt) = ARR*(PSC+1) / 72M
// 占空比P=CCR/(ARR+1)

/**
  * @brief  高级定时器PWM输入初始化和用到的GPIO初始化
  * @param  无
  * @retval 无
  */
static void ADVANCE_TIM_Mode_Config(void)
{
 
/*--------------------时基结构体初始化-------------------------*/
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;
	// 开启定时器时钟,即内部时钟CK_INT=72M
	ADVANCE_TIM_APBxClock_FUN(ADVANCE_TIM_CLK,ENABLE);
	// 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
	TIM_TimeBaseStructure.TIM_Period=ADVANCE_TIM_PERIOD;	
	// 驱动CNT计数器的时钟 = Fck_int/(psc+1)
	TIM_TimeBaseStructure.TIM_Prescaler= ADVANCE_TIM_PSC;	
	// 时钟分频因子 ，配置死区时间时需要用到
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		
	// 计数器计数模式，设置为向上计数
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		
	// 重复计数器的值，没用到不用管
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	// 初始化定时器
	TIM_TimeBaseInit(ADVANCE_TIM, &TIM_TimeBaseStructure);

	/*--------------------输入捕获结构体初始化-------------------*/	
  // 使用PWM输入模式时，需要占用两个捕获寄存器，一个测周期，另外一个测占空比
	

	// 捕获通道IC1配置
	// 选择捕获通道
  TIM_ICInitStructure.TIM_Channel = ADVANCE_TIM_IC1PWM_CHANNEL;
	// 设置捕获的边沿
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	// 设置捕获通道的信号来自于哪个输入通道，有直连和非直连两种
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	// 1分频，即捕获信号的每个有效边沿都捕获
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	// 不滤波
  TIM_ICInitStructure.TIM_ICFilter = 0x0;
	// 初始化PWM输入模式
  TIM_PWMIConfig(ADVANCE_TIM, &TIM_ICInitStructure);
	
	// 当工作做PWM输入模式时,只需要设置触发信号的那一路即可（用于测量周期）
	// 另外一路（用于测量占空比）会由硬件自带设置，不需要再配置
	
	// 捕获通道IC2配置	
//	TIM_ICInitStructure.TIM_Channel = ADVANCE_TIM_IC1PWM_CHANNEL;
//  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;
//  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_IndirectTI;
//  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
//  TIM_ICInitStructure.TIM_ICFilter = 0x0;
//  TIM_PWMIConfig(ADVANCE_TIM, &TIM_ICInitStructure);
	
	// 选择输入捕获的触发信号
  TIM_SelectInputTrigger(ADVANCE_TIM, TIM_TS_TI1FP1);		

	// 选择从模式: 复位模式
	// PWM输入模式时,从模式必须工作在复位模式，当捕获开始时,计数器CNT会被复位
  TIM_SelectSlaveMode(ADVANCE_TIM, TIM_SlaveMode_Reset);
  TIM_SelectMasterSlaveMode(ADVANCE_TIM,TIM_MasterSlaveMode_Enable); 

  // 使能捕获中断,这个中断针对的是主捕获通道（测量周期那个）
  TIM_ITConfig(ADVANCE_TIM, TIM_IT_CC1, ENABLE);	
	// 清除中断标志位
	TIM_ClearITPendingBit(ADVANCE_TIM, TIM_IT_CC1);
	
	 // 使能高级控制定时器，计数器开始计数
  TIM_Cmd(ADVANCE_TIM, ENABLE);
}
/**
  * @brief  高级定时器PWM输入初始化和用到的GPIO初始化
  * @param  无
  * @retval 无
  */
void ADVANCE_TIM_Init_PWM_INPUT(void)
{
	ADVANCE_TIM_GPIO_Config();
	ADVANCE_TIM_NVIC_Config();
	ADVANCE_TIM_Mode_Config();		
}

/*********************************************高级定时器PWM输入捕获**********************/
#endif

/*********************************************高级定时器DMA**********************/
#if Advance_TIM_DMA

uint16_t aSRC_Buffer[TIM_DMA_BUFSIZE] ={700,700,100};
/**
  * @brief  配置TIM复用输出PWM时用到的I/O
  * @param  无
  * @retval 无
  */
static void TIMx_GPIO_Config(void) 
{
	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;
  DMA_InitTypeDef DMA_InitStructure;
	
	/*开启定时器相关的GPIO外设时钟*/
	RCC_APB2PeriphClockCmd(ADVANCE_TIM_CH1_GPIO_CLK, ENABLE);
	
	/* 定时器功能引脚初始化 */															   
	GPIO_InitStructure.GPIO_Pin = ADVANCE_TIM_CH1_PIN;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(ADVANCE_TIM_CH1_PORT, &GPIO_InitStructure);
	
	/* DMA clock enable */
  RCC_AHBPeriphClockCmd(TIM_DMA_CLK, ENABLE);	
	
	DMA_DeInit(TIM_DMA_STREAM);

  // 设置DMA源地址：TIM的CCR寄存器地址*/
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(TIM_DMA_ADDRESS) ;
	// 内存地址(要传输的变量的指针)
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)aSRC_Buffer;
	// 方向：从内存到外设	
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	// 传输数目	
  DMA_InitStructure.DMA_BufferSize = TIM_DMA_BUFSIZE;
	// 外设地址不增	
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	// 内存地址自增
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	// 外设数据单位	
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	// 内存数据单位
  DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_HalfWord;
	// DMA模式，一次或者循环模式
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	// 优先级：中
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	// 禁止内存到内存的传输
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable ;
	// 配置DMA通道	
  DMA_Init(TIM_DMA_STREAM, &DMA_InitStructure);
}

/*
 * 注意：TIM_TimeBaseInitTypeDef结构体里面有5个成员，TIM6和TIM7的寄存器里面只有
 * TIM_Prescaler和TIM_Period，所以使用TIM6和TIM7的时候只需初始化这两个成员即可，
 * 另外三个成员是通用定时器和高级定时器才有.
 *-----------------------------------------------------------------------------
 * TIM_Prescaler         都有
 * TIM_CounterMode			 TIMx,x[6,7]没有，其他都有（基本定时器）
 * TIM_Period            都有
 * TIM_ClockDivision     TIMx,x[6,7]没有，其他都有(基本定时器)
 * TIM_RepetitionCounter TIMx,x[1,8]才有(高级定时器)
 *-----------------------------------------------------------------------------
 */
static void TIM_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	// 开启TIMx_CLK,x[1,8] 
  RCC_APB2PeriphClockCmd(ADVANCE_TIM_CLK, ENABLE); 

  /* 累计 TIM_Period个后产生一个更新或者中断*/		
  //当定时器从0计数到1023，即为1024次，为一个定时周期
  TIM_TimeBaseStructure.TIM_Period = 1024-1;
	// 高级控制定时器时钟源TIMxCLK = HCLK=72MHz 
	// 设定定时器频率为=TIMxCLK/(TIM_Prescaler+1)=100000Hz
  TIM_TimeBaseStructure.TIM_Prescaler = 7200-1;		
  // 采样时钟分频
  TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
  // 计数方式
  TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
  // 重复计数器
  TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	// 初始化定时器TIMx, x[1,8]
	TIM_TimeBaseInit(ADVANCE_TIM, &TIM_TimeBaseStructure);
	
  /*PWM模式配置*/
	//配置为PWM模式1
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	// 输出使能
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
  TIM_OCInitStructure.TIM_Pulse = 0;
	// 输出通道电平极性配置	
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
  //使能通道1
  TIM_OC1Init(ADVANCE_TIM, &TIM_OCInitStructure);
		
	/* Enable preload feature */
  TIM_OC1PreloadConfig(ADVANCE_TIM, TIM_OCPreload_Enable);
	
	// 使能定时器
	TIM_Cmd(ADVANCE_TIM, ENABLE);	
	
	/* DMA enable*/
  DMA_Cmd(TIM_DMA_STREAM, ENABLE);
	
	/* TIM Update DMA Request enable */
  TIM_DMACmd(ADVANCE_TIM, TIM_DMA_CC1, ENABLE);	

	/* 主动输出使能 */
  TIM_CtrlPWMOutputs(ADVANCE_TIM, ENABLE);
}

/**
  * @brief  初始化高级控制定时器
  * @param  无
  * @retval 无
  */
void ADVANCE_TIMx_Configuration_DMA(void)
{  
	TIMx_GPIO_Config();
	
  TIM_Mode_Config();
}

/*********************************************高级定时器DMA**********************/
#endif
