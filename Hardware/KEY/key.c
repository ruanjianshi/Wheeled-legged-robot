#include "key.h"

// 配置外部中断0的NVIC（嵌套向量中断控制器）参数
static void EXTI_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitConfig; // 定义NVIC初始化结构体

    // 设置中断优先级分组为组1（预占优先级1位，子优先级3位）
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); 

    // 配置EXTI0中断通道（对应GPIO_Pin_0）
    NVIC_InitConfig.NVIC_IRQChannel = EXTI0_IRQn;   
    // 设置预占优先级为1
    NVIC_InitConfig.NVIC_IRQChannelPreemptionPriority = 1; 
    // 设置子优先级为1
    NVIC_InitConfig.NVIC_IRQChannelSubPriority = 1; 
    // 使能该中断通道
    NVIC_InitConfig.NVIC_IRQChannelCmd = ENABLE;     
    // 应用NVIC配置
    NVIC_Init(&NVIC_InitConfig);      


		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); 
	  NVIC_InitConfig.NVIC_IRQChannel = EXTI15_10_IRQn; 
	  NVIC_InitConfig.NVIC_IRQChannelPreemptionPriority = 1;
	  NVIC_InitConfig.NVIC_IRQChannelSubPriority = 2;
	  NVIC_InitConfig.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitConfig);
}

// 配置按键（KEY）的外部中断功能
void EXTI_Key_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStruct; // GPIO初始化结构体
    EXTI_InitTypeDef EXTI_InitStruct; // EXTI初始化结构体

    // 配置外部中断的优先级（调用上述NVIC配置函数）
    EXTI_NVIC_Config();

    // 使能按键对应GPIO端口的时钟（例如：RCC_APB2Periph_GPIOA）
    RCC_APB2PeriphClockCmd(KEY1_GPIO_CLK, ENABLE);
    
    // 配置GPIO引脚（例如：GPIO_Pin_0）
    GPIO_InitStruct.GPIO_Pin = KEY1_GPIO_PIN; 
    // 设置为浮空输入模式（检测外部信号）
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
    // 应用GPIO配置（例如：GPIOA）
    GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStruct);   

    /*---- 配置外部中断线与GPIO的映射 ----*/
    // 使能AFIO时钟（必须的，用于重映射和EXTI配置）
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); 
    // 将PA0引脚连接到EXTI0中断线
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0); 

    // 选择EXTI0中断线
    EXTI_InitStruct.EXTI_Line = EXTI_Line0; 
    // 设置为中断模式（非事件模式）
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt; 
    // 设置为上升沿触发（当电压从低到高变化时触发）
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising; 
    // 使能该EXTI线
    EXTI_InitStruct.EXTI_LineCmd = ENABLE; 
    // 应用EXTI配置
    EXTI_Init(&EXTI_InitStruct); 
		
		
		RCC_APB2PeriphClockCmd(KEY2_GPIO_CLK, ENABLE);
		GPIO_InitStruct.GPIO_Pin = KEY2_GPIO_PIN;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
    GPIO_Init(KEY2_GPIO_PORT, &GPIO_InitStruct);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource13);
		EXTI_InitStruct.EXTI_Line = EXTI_Line13;
		EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt; 
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising; 
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStruct); 
}


/**
  * @brief  配置按键用到的I/O口
  * @param  无
  * @retval 无
  */
void Key_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/*开启按键端口的时钟*/
	RCC_APB2PeriphClockCmd(KEY1_GPIO_CLK|KEY2_GPIO_CLK,ENABLE);
	
	//选择按键的引脚
	GPIO_InitStructure.GPIO_Pin = KEY1_GPIO_PIN; 
	// 设置按键的引脚为浮空输入
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	//使用结构体初始化按键
	GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStructure);
	
	//选择按键的引脚
	GPIO_InitStructure.GPIO_Pin = KEY2_GPIO_PIN; 
	//设置按键的引脚为浮空输入
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	//使用结构体初始化按键
	GPIO_Init(KEY2_GPIO_PORT, &GPIO_InitStructure);	
}

//该检测方式缺少实时性，且受干扰影响大，建议加入实时操作系统，取消时间片轮回，采用任务调度或者采用中断映射。
 /*
 * 函数名：Key_Scan
 * 描述  ：检测是否有按键按下
 * 输入  ：GPIOx：x 可以是 A，B，C，D或者 E
 *		     GPIO_Pin：待读取的端口位 	
 * 输出  ：KEY_OFF(没按下按键)、KEY_ON（按下按键）
 */
uint8_t Key_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
{			
	/*检测是否有按键按下 */
	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON )  
	{	 
		/*等待按键释放 */
		while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON);   
		return 	KEY_ON;	 
	}
	else
		return KEY_OFF;
}
/*********************************************END OF FILE**********************/

