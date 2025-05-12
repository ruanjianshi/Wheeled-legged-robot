#include "bluetooth.h"
uint8_t Usart3_Receive = 0;
float C_Kp_site = 1;
float C_Ki_site = 0.0;
float C_Kd_site = 2;

void bluetooth_init()
	{
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB1PeriphClockCmd(BLUETOOTH_USART_CLK,ENABLE);	//使能USART3，GPIOB时钟
	RCC_APB2PeriphClockCmd(BLUETOOTH_USART_GPIO_CLK, ENABLE);	 //使能PA端口时钟
  
	//USART3_TX   GPIOB10
  GPIO_InitStructure.GPIO_Pin = BLUETOOTH_USART_TX_GPIO_PIN; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(BLUETOOTH_USART_TX_GPIO_PORT, &GPIO_InitStructure);//
   
  //USART3_RX	  GPIOB11初始化
  GPIO_InitStructure.GPIO_Pin = BLUETOOTH_USART_RX_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(BLUETOOTH_USART_TX_GPIO_PORT, &GPIO_InitStructure);  

  //Usart3 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = BLUETOOTH_USART_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = BLUETOOTH_USART_BAUDRATE;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

  USART_Init(BLUETOOTH_USARTx, &USART_InitStructure); //初始化串口3
  USART_ITConfig(BLUETOOTH_USARTx, USART_IT_RXNE, ENABLE);//开启串口接受中断
  USART_Cmd(BLUETOOTH_USARTx, ENABLE);                    //使能串口3 

}


/**************************************************************************
函数功能：串口3接收中断
入口参数：无
返回  值：无
**************************************************************************/

void BLUETOOTH_USART_IRQHandler(void)
{	
	if(USART_GetITStatus(BLUETOOTH_USARTx, USART_IT_RXNE) != RESET)
	{	  
		USART_ClearITPendingBit(BLUETOOTH_USARTx, USART_IT_RXNE);
		Usart3_Receive = USART_ReceiveData(BLUETOOTH_USARTx); //接收数据 
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

