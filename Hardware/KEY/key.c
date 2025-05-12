#include "key.h"

// �����ⲿ�ж�0��NVIC��Ƕ�������жϿ�����������
static void EXTI_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitConfig; // ����NVIC��ʼ���ṹ��

    // �����ж����ȼ�����Ϊ��1��Ԥռ���ȼ�1λ�������ȼ�3λ��
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); 

    // ����EXTI0�ж�ͨ������ӦGPIO_Pin_0��
    NVIC_InitConfig.NVIC_IRQChannel = EXTI0_IRQn;   
    // ����Ԥռ���ȼ�Ϊ1
    NVIC_InitConfig.NVIC_IRQChannelPreemptionPriority = 1; 
    // ���������ȼ�Ϊ1
    NVIC_InitConfig.NVIC_IRQChannelSubPriority = 1; 
    // ʹ�ܸ��ж�ͨ��
    NVIC_InitConfig.NVIC_IRQChannelCmd = ENABLE;     
    // Ӧ��NVIC����
    NVIC_Init(&NVIC_InitConfig);      


		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); 
	  NVIC_InitConfig.NVIC_IRQChannel = EXTI15_10_IRQn; 
	  NVIC_InitConfig.NVIC_IRQChannelPreemptionPriority = 1;
	  NVIC_InitConfig.NVIC_IRQChannelSubPriority = 2;
	  NVIC_InitConfig.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitConfig);
}

// ���ð�����KEY�����ⲿ�жϹ���
void EXTI_Key_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStruct; // GPIO��ʼ���ṹ��
    EXTI_InitTypeDef EXTI_InitStruct; // EXTI��ʼ���ṹ��

    // �����ⲿ�жϵ����ȼ�����������NVIC���ú�����
    EXTI_NVIC_Config();

    // ʹ�ܰ�����ӦGPIO�˿ڵ�ʱ�ӣ����磺RCC_APB2Periph_GPIOA��
    RCC_APB2PeriphClockCmd(KEY1_GPIO_CLK, ENABLE);
    
    // ����GPIO���ţ����磺GPIO_Pin_0��
    GPIO_InitStruct.GPIO_Pin = KEY1_GPIO_PIN; 
    // ����Ϊ��������ģʽ������ⲿ�źţ�
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
    // Ӧ��GPIO���ã����磺GPIOA��
    GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStruct);   

    /*---- �����ⲿ�ж�����GPIO��ӳ�� ----*/
    // ʹ��AFIOʱ�ӣ�����ģ�������ӳ���EXTI���ã�
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); 
    // ��PA0�������ӵ�EXTI0�ж���
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0); 

    // ѡ��EXTI0�ж���
    EXTI_InitStruct.EXTI_Line = EXTI_Line0; 
    // ����Ϊ�ж�ģʽ�����¼�ģʽ��
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt; 
    // ����Ϊ�����ش���������ѹ�ӵ͵��߱仯ʱ������
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising; 
    // ʹ�ܸ�EXTI��
    EXTI_InitStruct.EXTI_LineCmd = ENABLE; 
    // Ӧ��EXTI����
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
  * @brief  ���ð����õ���I/O��
  * @param  ��
  * @retval ��
  */
void Key_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/*���������˿ڵ�ʱ��*/
	RCC_APB2PeriphClockCmd(KEY1_GPIO_CLK|KEY2_GPIO_CLK,ENABLE);
	
	//ѡ�񰴼�������
	GPIO_InitStructure.GPIO_Pin = KEY1_GPIO_PIN; 
	// ���ð���������Ϊ��������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	//ʹ�ýṹ���ʼ������
	GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStructure);
	
	//ѡ�񰴼�������
	GPIO_InitStructure.GPIO_Pin = KEY2_GPIO_PIN; 
	//���ð���������Ϊ��������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	//ʹ�ýṹ���ʼ������
	GPIO_Init(KEY2_GPIO_PORT, &GPIO_InitStructure);	
}

//�ü�ⷽʽȱ��ʵʱ�ԣ����ܸ���Ӱ��󣬽������ʵʱ����ϵͳ��ȡ��ʱ��Ƭ�ֻأ�����������Ȼ��߲����ж�ӳ�䡣
 /*
 * ��������Key_Scan
 * ����  ������Ƿ��а�������
 * ����  ��GPIOx��x ������ A��B��C��D���� E
 *		     GPIO_Pin������ȡ�Ķ˿�λ 	
 * ���  ��KEY_OFF(û���°���)��KEY_ON�����°�����
 */
uint8_t Key_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
{			
	/*����Ƿ��а������� */
	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON )  
	{	 
		/*�ȴ������ͷ� */
		while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON);   
		return 	KEY_ON;	 
	}
	else
		return KEY_OFF;
}
/*********************************************END OF FILE**********************/

