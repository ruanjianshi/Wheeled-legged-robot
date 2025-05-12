#include "usart.h"
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Ƕ�������жϿ�������ѡ�� */
	/* ��ʾ NVIC_PriorityGroupConfig() ����������ֻ��Ҫ����һ�����������ȼ�����*/
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* ����USARTΪ�ж�Դ */
  NVIC_InitStructure.NVIC_IRQChannel = TONGXIN_USART_IRQ;
  /* �������ȼ�*/
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* �����ȼ� */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* ʹ���ж� */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* ��ʼ������NVIC */
  NVIC_Init(&NVIC_InitStructure);
}

void USART_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	// �򿪴���GPIO��ʱ��
	TONGXIN_USART_GPIO_APBxClkCmd(TONGXIN_USART_GPIO_CLK, ENABLE);
	
	// �򿪴��������ʱ��
	TONGXIN_USART_APBxClkCmd(TONGXIN_USART_CLK, ENABLE);

	// ��USART Tx��GPIO����Ϊ���츴��ģʽ
	GPIO_InitStructure.GPIO_Pin = TONGXIN_USART_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(TONGXIN_USART_TX_GPIO_PORT, &GPIO_InitStructure);

  // ��USART Rx��GPIO����Ϊ��������ģʽ
	GPIO_InitStructure.GPIO_Pin = TONGXIN_USART_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(TONGXIN_USART_RX_GPIO_PORT, &GPIO_InitStructure);
	
	// ���ô��ڵĹ�������
	// ���ò�����
	USART_InitStructure.USART_BaudRate = TONGXIN_USART_BAUDRATE;
	// ���� �������ֳ�
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// ����ֹͣλ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// ����У��λ
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	// ����Ӳ��������
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	// ���ù���ģʽ���շ�һ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// ��ɴ��ڵĳ�ʼ������
	USART_Init(TONGXIN_USARTx, &USART_InitStructure);
	
	// �����ж����ȼ�����
	NVIC_Configuration();
	
	// ʹ�ܴ��ڽ����ж�
	USART_ITConfig(TONGXIN_USARTx, USART_IT_RXNE, ENABLE);	
	
	// ʹ�ܴ���
	USART_Cmd(TONGXIN_USARTx, ENABLE);	    
}


//���Ͱ�λ
void Usart_Send8Bit(USART_TypeDef *pUSARTx, uint8_t data)
{
	USART_SendData(pUSARTx, data);
	
	while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);

}
//����16λ
void Usart_Send16Bit(USART_TypeDef *pUSARTx, uint16_t data)
{
	
	uint8_t temp_h,temp_l;
	
	temp_h = (data&0xff00) >> 8;
	temp_l = (data&0xff);
	
	USART_SendData(pUSARTx,temp_h);
	while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
	
	USART_SendData(pUSARTx,temp_l);
	while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);

}

//��������
void Usart_SendArray(USART_TypeDef *pUSARTx, uint8_t *array,uint8_t num)
{
	uint8_t i;
	while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET);
	for(i = 0; i<num; i++)
	{
		USART_SendData(pUSARTx,array[i]);
		while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET);
	}

}

//�����ַ���
void Usart_SendStr(USART_TypeDef *pUSARTx, uint8_t *str)
{
	uint8_t i = 0;
	do
	{
		USART_SendData(pUSARTx,*(str+i));
		i++;
	
	}while(*(str+i) != '\0');
	
	while(USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET);
}

///�ض���c�⺯��printf�����ڣ��ض�����ʹ��printf����
int fputc(int ch, FILE *f)
{
		/* ����һ���ֽ����ݵ����� */
		USART_SendData(TONGXIN_USARTx, (uint8_t) ch);
		
		/* �ȴ�������� */
		while (USART_GetFlagStatus(TONGXIN_USARTx, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

///�ض���c�⺯��scanf�����ڣ���д����ʹ��scanf��getchar�Ⱥ���
int fgetc(FILE *f)
{
		/* �ȴ������������� */
		while (USART_GetFlagStatus(TONGXIN_USARTx, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(TONGXIN_USARTx);
}

