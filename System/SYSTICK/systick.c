#include "systick.h"

#if 0
static __INLINE uint32_t SysTick_Config(uint32_t ticks)
{ 
	//判断tick的值是否大于2^24，如果大于，则不符合规则
  if (ticks > SysTick_LOAD_RELOAD_Msk)  return (1);            /* Reload value impossible */
  //初始化reload寄存器的值                                                            
  SysTick->LOAD  = (ticks & SysTick_LOAD_RELOAD_Msk) - 1;      /* set reload register */
	//配置中断优先级，配置为15，默认为最低的优先级
  NVIC_SetPriority (SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1);  /* set Priority for Cortex-M0 System Interrupts */
	//初始化counter的值为0
  SysTick->VAL   = 0;                                          /* Load the SysTick Counter Value */
	
	//配置systick的时钟为72M
  //使能中断
  //使能systick
  SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk | 
                   SysTick_CTRL_TICKINT_Msk   | 
                   SysTick_CTRL_ENABLE_Msk;                    /* Enable SysTick IRQ and SysTick Timer */
  return (0);                                                  /* Function successful */
}


void SysTick_Delay_us(uint32_t us)
{
	uint32_t i;
	SysTick_Config(72);
	
	for(i = 0;i<us;i++)
	{
		while(!((SysTick->CTRL) & (1<<16)));
	}
	
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
	
}


void SysTick_Delay_ms(uint32_t ms)
{
	uint32_t i;
	SysTick_Config(72000);
	
	for(i = 0;i<ms;i++)
	{
		while(!((SysTick->CTRL) & (1<<16)));
	}
	
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
	
}
#endif

/*******************************************
SysTick初始化(模拟micros函数初始化)：

备注：     
	1.从0xFFFFFF到0循环计数
	2.开启后无法再使用delay延时函数
*******************************************/
void SysTick_Computation_timeBegin(void)
{
	SysTick->LOAD = 0xFFFFFF-1;
	SysTick->VAL  = 0;
	SysTick->CTRL = SysTick_CTRL_ENABLE_Msk;
}

uint32_t SysTick_Computation_time(void)
{
	return SysTick->VAL;
}

void SysTick_Computation_timeEnd(void)
{
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}


/**
  * @brief  微秒级延时
  * @param  xus 延时时长，范围：0~233015
  * @retval 无
  */
void Delay_us(uint32_t xus)
{
	SysTick->LOAD = 72 * xus;				//设置定时器重装值
	SysTick->VAL = 0x00;					//清空当前计数值
	SysTick->CTRL = 0x00000005;				//设置时钟源为HCLK，启动定时器
	while(!(SysTick->CTRL & 0x00010000));	//等待计数到0
	SysTick->CTRL = 0x00000004;				//关闭定时器
}

/**
  * @brief  毫秒级延时
  * @param  xms 延时时长，范围：0~4294967295
  * @retval 无
  */
void Delay_ms(uint32_t xms)
{
	while(xms--)
	{
		Delay_us(1000);
	}
}
 
/**
  * @brief  秒级延时
  * @param  xs 延时时长，范围：0~4294967295
  * @retval 无
  */
void Delay_s(uint32_t xs)
{
	while(xs--)
	{
		Delay_ms(1000);
	}
} 




void Delay(uint32_t count)
{
	for(;count!=0;count--);
}

