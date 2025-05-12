#include "bluetooth.h"
uint8_t Usart3_Receive = 0;
float C_Kp_site = 1;
float C_Ki_site = 0.0;
float C_Kd_site = 2;

void bluetooth_init()
	{
  //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB1PeriphClockCmd(BLUETOOTH_USART_CLK,ENABLE);	//ʹ��USART3��GPIOBʱ��
	RCC_APB2PeriphClockCmd(BLUETOOTH_USART_GPIO_CLK, ENABLE);	 //ʹ��PA�˿�ʱ��
  
	//USART3_TX   GPIOB10
  GPIO_InitStructure.GPIO_Pin = BLUETOOTH_USART_TX_GPIO_PIN; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(BLUETOOTH_USART_TX_GPIO_PORT, &GPIO_InitStructure);//
   
  //USART3_RX	  GPIOB11��ʼ��
  GPIO_InitStructure.GPIO_Pin = BLUETOOTH_USART_RX_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(BLUETOOTH_USART_TX_GPIO_PORT, &GPIO_InitStructure);  

  //Usart3 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = BLUETOOTH_USART_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = BLUETOOTH_USART_BAUDRATE;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

  USART_Init(BLUETOOTH_USARTx, &USART_InitStructure); //��ʼ������3
  USART_ITConfig(BLUETOOTH_USARTx, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
  USART_Cmd(BLUETOOTH_USARTx, ENABLE);                    //ʹ�ܴ���3 

}


/**************************************************************************
�������ܣ�����3�����ж�
��ڲ�������
����  ֵ����
**************************************************************************/

void BLUETOOTH_USART_IRQHandler(void)
{	
	if(USART_GetITStatus(BLUETOOTH_USARTx, USART_IT_RXNE) != RESET)
	{	  
		USART_ClearITPendingBit(BLUETOOTH_USARTx, USART_IT_RXNE);
		Usart3_Receive = USART_ReceiveData(BLUETOOTH_USARTx); //�������� 
		//Usart_Send8Bit(BLUETOOTH_USARTx,Usart3_Receive);
		//Usart_SendStr(BLUETOOTH_USARTx,"Please select debug mode");

	if(flagMode == 1)
		{
			Usart_SendStr(BLUETOOTH_USARTx,"P");
			switch(Usart3_Receive)//SBUF
			 {
					 case '2':  C_Kp_site = C_Kp_site + Kp_site_add;        
																	 
										break;
					 case '8': 	C_Ki_site = C_Ki_site + Ki_site_add;         
																	 
										break; 
								 
					 case '4': 	C_Kd_site = C_Kd_site + Kd_site_add;         
																	 
										break; 
								 
					 case '6':          
																	 
										break; 
								 
					 case '5':          
										break;	

					 default: Usart_SendStr(BLUETOOTH_USARTx,"Please input correct instructions");   
										break;
					}
		}
	else if(flagMode == 3)
		{
			Usart_SendStr(BLUETOOTH_USARTx,"C");
			switch(Usart3_Receive)
			{
					case '2':
										
									 break;
					
					
				  default: Usart_SendStr(BLUETOOTH_USARTx,"Please input correct instructions");   
									 break;
			
			}
		}

	} 
}

