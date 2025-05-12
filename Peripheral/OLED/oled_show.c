#include "oled_show.h"
#include "oled_Data.h"
/* oledд���� */
static void Oled_Write_Data(SoftI2C_HandleTypeDef *IICWHERE,uint8_t data)
{
#if I2C_Select
    IIC_Write_Byte(OLED_ID,OLED_WR_DATA,data);
#else
    i2c_Start(IICWHERE);
    i2c_SendByte(IICWHERE,OLED_ID);
    /* �ȴ�Ӧ�� */
    while (i2c_WaitAck(IICWHERE));
    i2c_SendByte(IICWHERE,OLED_WR_DATA);
    /* �ȴ�Ӧ�� */
    while (i2c_WaitAck(IICWHERE));
    i2c_SendByte(IICWHERE,data);
    /* �ȴ�Ӧ�� */
    while (i2c_WaitAck(IICWHERE));
    i2c_Stop(IICWHERE);
#endif
}

/* oledд���� */
static void Oled_Write_Cmd(SoftI2C_HandleTypeDef *IICWHERE,uint8_t cmd)
{
#if I2C_Select
    IIC_Write_Byte(OLED_ID,OLED_WR_CMD,cmd);
#else
    i2c_Start(IICWHERE);
    i2c_SendByte(IICWHERE,OLED_ID);
    /* �ȴ�Ӧ�� */
    while (i2c_WaitAck(IICWHERE));
    i2c_SendByte(IICWHERE,OLED_WR_CMD);
    /* �ȴ�Ӧ�� */
    while (i2c_WaitAck(IICWHERE));
    i2c_SendByte(IICWHERE,cmd);
    /* �ȴ�Ӧ�� */
    while (i2c_WaitAck(IICWHERE));
    i2c_Stop(IICWHERE);
#endif
}

#if IsLaunchC8T6_IIC

/*��������*/
#define OLED_W_SCL_C8T6(x)		GPIO_WriteBit(GPIOB, GPIO_Pin_8, (BitAction)(x))
#define OLED_W_SDA_C8T6(x)		GPIO_WriteBit(GPIOB, GPIO_Pin_9, (BitAction)(x))

/*���ų�ʼ��*/
void OLED_I2C_Init_C8T6(void)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	OLED_W_SCL_C8T6(1);
	OLED_W_SDA_C8T6(1);
}

/**
  * @brief  I2C��ʼ
  * @param  ��
  * @retval ��
  */
void OLED_I2C_Start_C8T6(void)
{
	OLED_W_SDA_C8T6(1);
	OLED_W_SCL_C8T6(1);
	OLED_W_SDA_C8T6(0);
	OLED_W_SCL_C8T6(0);
}

/**
  * @brief  I2Cֹͣ
  * @param  ��
  * @retval ��
  */
void OLED_I2C_Stop_C8T6(void)
{
	OLED_W_SDA_C8T6(0);
	OLED_W_SCL_C8T6(1);
	OLED_W_SDA_C8T6(1);
}

/**
  * @brief  I2C����һ���ֽ�
  * @param  Byte Ҫ���͵�һ���ֽ�
  * @retval ��
  */
void OLED_I2C_SendByte_C8T6(uint8_t Byte)
{
	uint8_t i;
	for (i = 0; i < 8; i++)
	{
		OLED_W_SDA_C8T6(!!(Byte & (0x80 >> i)));
		OLED_W_SCL_C8T6(1);
		OLED_W_SCL_C8T6(0);
	}
	OLED_W_SCL_C8T6(1);	//�����һ��ʱ�ӣ�������Ӧ���ź�
	OLED_W_SCL_C8T6(0);
}

/**
  * @brief  OLEDд����
  * @param  Command Ҫд�������
  * @retval ��
  */
void OLED_WriteCommand_C8T6(uint8_t Command)
{
	OLED_I2C_Start_C8T6();
	OLED_I2C_SendByte_C8T6(0x78);		//�ӻ���ַ
	OLED_I2C_SendByte_C8T6(0x00);		//д����
	OLED_I2C_SendByte_C8T6(Command); 
	OLED_I2C_Stop_C8T6();
}

/**
  * @brief  OLEDд����
  * @param  Data Ҫд�������
  * @retval ��
  */
void OLED_WriteData_C8T6(uint8_t Data)
{
	OLED_I2C_Start_C8T6();
	OLED_I2C_SendByte_C8T6(0x78);		//�ӻ���ַ
	OLED_I2C_SendByte_C8T6(0x40);		//д����
	OLED_I2C_SendByte_C8T6(Data);
	OLED_I2C_Stop_C8T6();
}

/**
  * @brief  OLED���ù��λ��
  * @param  Y �����Ͻ�Ϊԭ�㣬���·�������꣬��Χ��0~7
  * @param  X �����Ͻ�Ϊԭ�㣬���ҷ�������꣬��Χ��0~127
  * @retval ��
  */
void OLED_SetCursor_C8T6(uint8_t Y, uint8_t X)
{
	OLED_WriteCommand_C8T6(0xB0 | Y);					//����Yλ��
	OLED_WriteCommand_C8T6(0x10 | ((X & 0xF0) >> 4));	//����Xλ�ø�4λ
	OLED_WriteCommand_C8T6(0x00 | (X & 0x0F));			//����Xλ�õ�4λ
}

/**
  * @brief  OLED����
  * @param  ��
  * @retval ��
  */
void OLED_Clear_C8T6(void)
{  
	uint8_t i, j;
	for (j = 0; j < 8; j++)
	{
		OLED_SetCursor_C8T6(j, 0);
		for(i = 0; i < 128; i++)
		{
			OLED_WriteData_C8T6(0x00);
		}
	}
}

/**
  * @brief  OLED��ʾһ���ַ�
  * @param  Line ��λ�ã���Χ��1~4
  * @param  Column ��λ�ã���Χ��1~16
  * @param  Char Ҫ��ʾ��һ���ַ�����Χ��ASCII�ɼ��ַ�
  * @retval ��
  */
void OLED_ShowChar_C8T6(uint8_t Line, uint8_t Column, char Char)
{      	
	uint8_t i;
	OLED_SetCursor_C8T6((Line - 1) * 2, (Column - 1) * 8);		//���ù��λ�����ϰ벿��
	for (i = 0; i < 8; i++)
	{
		OLED_WriteData_C8T6(OLED_F8x16[Char - ' '][i]);			//��ʾ�ϰ벿������
	}
	OLED_SetCursor_C8T6((Line - 1) * 2 + 1, (Column - 1) * 8);	//���ù��λ�����°벿��
	for (i = 0; i < 8; i++)
	{
		OLED_WriteData_C8T6(OLED_F8x16[Char - ' '][i + 8]);		//��ʾ�°벿������
	}
}

/**
  * @brief  OLED��ʾ�ַ���
  * @param  Line ��ʼ��λ�ã���Χ��1~4
  * @param  Column ��ʼ��λ�ã���Χ��1~16
  * @param  String Ҫ��ʾ���ַ�������Χ��ASCII�ɼ��ַ�
  * @retval ��
  */
void OLED_ShowString_C8T6(uint8_t Line, uint8_t Column, char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i++)
	{
		OLED_ShowChar_C8T6(Line, Column + i, String[i]);
	}
}

/**
  * @brief  OLED�η�����
  * @retval ����ֵ����X��Y�η�
  */
uint32_t OLED_Pow_C8T6(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while (Y--)
	{
		Result *= X;
	}
	return Result;
}

/**
  * @brief  OLED��ʾ���֣�ʮ���ƣ�������
  * @param  Line ��ʼ��λ�ã���Χ��1~4
  * @param  Column ��ʼ��λ�ã���Χ��1~16
  * @param  Number Ҫ��ʾ�����֣���Χ��0~4294967295
  * @param  Length Ҫ��ʾ���ֵĳ��ȣ���Χ��1~10
  * @retval ��
  */
void OLED_ShowNum_C8T6(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar_C8T6(Line, Column + i, Number / OLED_Pow_C8T6(10, Length - i - 1) % 10 + '0');
	}
}


/**
 * @brief OLED ��ʾ���������Զ�����������С��λ����
 * @param Line    �кţ�0-3��
 * @param Column  �кţ�0-127��
 * @param Number  Ҫ��ʾ�ĸ�����
 * @param MaxDec  ���С��λ������ֹ�����
 */
void OLED_ShowFloat_C8T6(uint8_t Line, uint8_t Column, float Number, uint8_t MaxDec) 
	{
    char strBuffer[16];  // ��ʱ�洢ת������ַ���
    uint8_t strPos = 0;  // �ַ�����ǰλ��
    
    // ������
    if (Number < 0) {
        strBuffer[strPos++] = '-';
        Number = -Number;
    }
    
    // ��ȡ��������
    uint32_t intPart = (uint32_t)Number;
    float decPart = Number - intPart;
    
    // ������������λ��
    uint8_t intLength = 0;
    uint32_t temp = (intPart == 0) ? 1 : intPart; // ���� 0 ���
    while (temp > 0) {
        temp /= 10;
        intLength++;
    }
    
    // ת��Ϊ�ַ���
    sprintf(&strBuffer[strPos], "%lu.", intPart); // ���� + С����
    strPos += intLength;
    strPos++; // С����ռ1λ
    
    // ��ȡС�����֣���� MaxDec λ��
    for (uint8_t i = 0; i < MaxDec; i++) {
        decPart *= 10;
        uint8_t digit = (uint8_t)decPart;
        strBuffer[strPos + i] = digit + '0';
        decPart -= digit;
    }
    
    // ȥ��ĩβ�� 0���� 3.140 �� 3.14��
    uint8_t finalLength = strPos + MaxDec;
    while (finalLength > strPos && strBuffer[finalLength - 1] == '0') {
        finalLength--;
    }
    
    // ���û��С�����֣�ȥ��С���㣨�� 100. �� 100��
    if (finalLength == strPos) {
        finalLength--;  // ȥ��С����
    }
    
    // ��ʾ�ַ���
    OLED_ShowString_C8T6(Line, Column, strBuffer);
}


/**
  * @brief  OLED��ʾ���֣�ʮ���ƣ�����������
  * @param  Line ��ʼ��λ�ã���Χ��1~4
  * @param  Column ��ʼ��λ�ã���Χ��1~16
  * @param  Number Ҫ��ʾ�����֣���Χ��-2147483648~2147483647
  * @param  Length Ҫ��ʾ���ֵĳ��ȣ���Χ��1~10
  * @retval ��
  */
void OLED_ShowSignedNum_C8T6(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length)
{
	uint8_t i;
	uint32_t Number1;
	if (Number >= 0)
	{
		OLED_ShowChar_C8T6(Line, Column, '+');
		Number1 = Number;
	}
	else
	{
		OLED_ShowChar_C8T6(Line, Column, '-');
		Number1 = -Number;
	}
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar_C8T6(Line, Column + i + 1, Number1 / OLED_Pow_C8T6(10, Length - i - 1) % 10 + '0');
	}
}

/**
  * @brief  OLED��ʾ���֣�ʮ�����ƣ�������
  * @param  Line ��ʼ��λ�ã���Χ��1~4
  * @param  Column ��ʼ��λ�ã���Χ��1~16
  * @param  Number Ҫ��ʾ�����֣���Χ��0~0xFFFFFFFF
  * @param  Length Ҫ��ʾ���ֵĳ��ȣ���Χ��1~8
  * @retval ��
  */
void OLED_ShowHexNum_C8T6(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i, SingleNumber;
	for (i = 0; i < Length; i++)							
	{
		SingleNumber = Number / OLED_Pow_C8T6(16, Length - i - 1) % 16;
		if (SingleNumber < 10)
		{
			OLED_ShowChar_C8T6(Line, Column + i, SingleNumber + '0');
		}
		else
		{
			OLED_ShowChar_C8T6(Line, Column + i, SingleNumber - 10 + 'A');
		}
	}
}

/**
  * @brief  OLED��ʾ���֣������ƣ�������
  * @param  Line ��ʼ��λ�ã���Χ��1~4
  * @param  Column ��ʼ��λ�ã���Χ��1~16
  * @param  Number Ҫ��ʾ�����֣���Χ��0~1111 1111 1111 1111
  * @param  Length Ҫ��ʾ���ֵĳ��ȣ���Χ��1~16
  * @retval ��
  */
void OLED_ShowBinNum_C8T6(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar_C8T6(Line, Column + i, Number / OLED_Pow_C8T6(2, Length - i - 1) % 2 + '0');
	}
}

/**
  * @brief  OLED��ʼ��
  * @param  ��
  * @retval ��
  */
void OLED_Init_C8T6(void)
{
	uint32_t i, j;
	
	for (i = 0; i < 1000; i++)			//�ϵ���ʱ
	{
		for (j = 0; j < 1000; j++);
	}
	
	OLED_I2C_Init_C8T6();			//�˿ڳ�ʼ��
	
	OLED_WriteCommand_C8T6(0xAE);	//�ر���ʾ
	
	OLED_WriteCommand_C8T6(0xD5);	//������ʾʱ�ӷ�Ƶ��/����Ƶ��
	OLED_WriteCommand_C8T6(0x80);
	
	OLED_WriteCommand_C8T6(0xA8);	//���ö�·������
	OLED_WriteCommand_C8T6(0x3F);
	
	OLED_WriteCommand_C8T6(0xD3);	//������ʾƫ��
	OLED_WriteCommand_C8T6(0x00);
	
	OLED_WriteCommand_C8T6(0x40);	//������ʾ��ʼ��
	
	OLED_WriteCommand_C8T6(0xA1);	//�������ҷ���0xA1���� 0xA0���ҷ���
	
	OLED_WriteCommand_C8T6(0xC8);	//�������·���0xC8���� 0xC0���·���

	OLED_WriteCommand_C8T6(0xDA);	//����COM����Ӳ������
	OLED_WriteCommand_C8T6(0x12);
	
	OLED_WriteCommand_C8T6(0x81);	//���öԱȶȿ���
	OLED_WriteCommand_C8T6(0xCF);

	OLED_WriteCommand_C8T6(0xD9);	//����Ԥ�������
	OLED_WriteCommand_C8T6(0xF1);

	OLED_WriteCommand_C8T6(0xDB);	//����VCOMHȡ��ѡ�񼶱�
	OLED_WriteCommand_C8T6(0x30);

	OLED_WriteCommand_C8T6(0xA4);	//����������ʾ��/�ر�

	OLED_WriteCommand_C8T6(0xA6);	//��������/��ת��ʾ

	OLED_WriteCommand_C8T6(0x8D);	//���ó���
	OLED_WriteCommand_C8T6(0x14);

	OLED_WriteCommand_C8T6(0xAF);	//������ʾ
		
	OLED_Clear_C8T6();				//OLED����
}

#endif


void OLED_Init(SoftI2C_HandleTypeDef *IICWHERE)
{
		i2c_CfgGpio(IICWHERE);
    /* ������ʾ��/�ر�
     * AE--->��ʾ��
     * AF--->��ʾ�ر�(����ģʽ)
     */
    Oled_Write_Cmd(IICWHERE,0xAE);

    /* ================== ��������� ===================*/
    /* ���öԱȶ�
     * 0~255����ֵԽ�� ����Խ��
     */
    Oled_Write_Cmd(IICWHERE,0x81);
    Oled_Write_Cmd(IICWHERE,0xFF);

    /* ʹ��ȫ����ʾ
     * A4--->�ָ���RAM������ʾ
     * A5--->����RAM������ʾ
     */
    Oled_Write_Cmd(IICWHERE,0xA4);

    /* ������ʾģʽ
     * A6--->������ʾ��0��1��
     * A7--->����ʾ��1��0��
     */
    Oled_Write_Cmd(IICWHERE,0xA6);

    /* ================== ��������� ===================*/
    /* ����ʹ��/ʧ��
     * 2E--->ʧ��
     * 2F--->ʹ��
     */
    Oled_Write_Cmd(IICWHERE,0x2E);

    /* ���ֽ���� ����ˮƽ�������� */

    /* ��/��ˮƽ��������
     * 26--->��ˮƽ����
     * 27--->��ˮƽ����
     */
    Oled_Write_Cmd(IICWHERE,0x26);
    /* �����ֽ� */
    Oled_Write_Cmd(IICWHERE,0x00);
    /* ���ù�����ʼҳ��ַ */
    Oled_Write_Cmd(IICWHERE,0x00);
    /* ���ù������ */
    Oled_Write_Cmd(IICWHERE,0x03);
    /* ���ù���������ַ */
    Oled_Write_Cmd(IICWHERE,0x07);
    /* �����ֽ� */
    Oled_Write_Cmd(IICWHERE,0x00);
    Oled_Write_Cmd(IICWHERE,0xFF);

    /* =============== Ѱַ��������� ==================*/

    /* ˫�ֽ�����:�Ĵ���Ѱַģʽ */
    Oled_Write_Cmd(IICWHERE,0x20);

    /* 10:ҳѰַģʽ
     * 01:��ֱѰַģʽ
     * 00:ˮƽѰַģʽ
     */
    Oled_Write_Cmd(IICWHERE,0x10);
    /* ���ֽ�����:����ҳѰַ����ʼҳ��ַ */
    Oled_Write_Cmd(IICWHERE,0xB0);
    /* ���ֽ�����:����ҳѰַ����ʼ�е�ַ��λ */
    Oled_Write_Cmd(IICWHERE,0x00);
    /* ���ֽ�����:����ҳѰַ����ʼ�е�ַ��λ */
    Oled_Write_Cmd(IICWHERE,0x10);

    /*=============== Ӳ����������� ==================*/

    /* ������ʾ��ʼ��
     * 0x40~0x7F��Ӧ0~63
     */
    Oled_Write_Cmd(IICWHERE,0x40);

    /* ��������ӳ��
     * A0:addressX--->segX
     * A1:addressX--->seg(127-X)
     */
    Oled_Write_Cmd(IICWHERE,0xA1);

    /* ���ö�·���ñ� */
    Oled_Write_Cmd(IICWHERE,0xA8);
    Oled_Write_Cmd(IICWHERE,0x3F);

    /* ����COM���ɨ�跽��
     * C0��COM0--->COM63(��������ɨ��)
     * C8��COM63--->COM0(��������ɨ��)
     */
    Oled_Write_Cmd(IICWHERE,0xC8);

    /* ˫�ֽ��������COM��ʾƫ���� */
    Oled_Write_Cmd(IICWHERE,0xD3);
    Oled_Write_Cmd(IICWHERE,0x00); /* COM��ƫ�� */

    /* ˫�ֽ��������COM��ӳ�� */
    Oled_Write_Cmd(IICWHERE,0xDA);
    Oled_Write_Cmd(IICWHERE,0x12);

    /* ˫�ֽ��������Ԥ���� */
    Oled_Write_Cmd(IICWHERE,0xD9);
    Oled_Write_Cmd(IICWHERE,0x22); /* �׶�һ2����ЧDCLKʱ��/�׶ζ�2����ЧDCLKʱ�� */

    /* ����VCOMHȡ��ѡ���ƽ
     * 00:0.65xVcc
     * 20:0.77xVcc
     * 30:0.83xVcc
     */
    Oled_Write_Cmd(IICWHERE,0xDB);
    Oled_Write_Cmd(IICWHERE,0x20);

    /* ˫�ֽ�������õ�ɱ� */
    Oled_Write_Cmd(IICWHERE,0x8d);
    Oled_Write_Cmd(IICWHERE,0x14);

    Oled_Write_Cmd(IICWHERE,0xAF);
}

/**
 * @brief  ���ù��
 * @param  x,���xλ��
 *		   y,���yλ��
 * @retval ��
 */
void OLED_SetPos(SoftI2C_HandleTypeDef *IICWHERE,unsigned char x, unsigned char y) // ������ʼ������
{
    Oled_Write_Cmd(IICWHERE,0xb0 + y);
    Oled_Write_Cmd(IICWHERE,((x & 0xf0) >> 4) | 0x10);
    Oled_Write_Cmd(IICWHERE,(x & 0x0f) | 0x01);
}

/**
 * @brief  ���������Ļ
 * @param  fill_data:Ҫ��������
 * @retval ��
 */
void OLED_Fill(SoftI2C_HandleTypeDef *IICWHERE,unsigned char fill_data) // ȫ�����
{
    unsigned char m, n;
    for (m = 0; m < 8; m++)
    {
        Oled_Write_Cmd(IICWHERE,0xb0 + m); // page0-page1
        Oled_Write_Cmd(IICWHERE,0x00);     // low column start address
        Oled_Write_Cmd(IICWHERE,0x10);     // high column start address
        for (n = 0; n < 128; n++)
        {
            Oled_Write_Data(IICWHERE,fill_data);
        }
    }
}

/**
 * @brief  ����
 * @param  ��
 * @retval ��
 */
void OLED_CLS(SoftI2C_HandleTypeDef *IICWHERE) // ����
{
    OLED_Fill(IICWHERE,0x00);
}

/**
 * @brief  ��OLED�������л���
 * @param  ��
 * @retval ��
 */
void OLED_ON(SoftI2C_HandleTypeDef *IICWHERE)
{
    Oled_Write_Cmd(IICWHERE,0X8D); // ���õ�ɱ�
    Oled_Write_Cmd(IICWHERE,0X14); // ������ɱ�
    Oled_Write_Cmd(IICWHERE,0XAF); // OLED����
}

/**
 * @brief  ��OLED���� -- ����ģʽ��,OLED���Ĳ���10uA
 * @param  ��
 * @retval ��
 */
void OLED_OFF(SoftI2C_HandleTypeDef *IICWHERE)
{
    Oled_Write_Cmd(IICWHERE,0X8D); // ���õ�ɱ�
    Oled_Write_Cmd(IICWHERE,0X10); // �رյ�ɱ�
    Oled_Write_Cmd(IICWHERE,0XAE); // OLED����
}

/**
 * @brief ��ʾcodetab.h�е�ASCII�ַ�,��6*8��8*16��ѡ��
 * @param  x,y : ��ʼ������(x:0~127, y:0~7);
 *					ch[] :- Ҫ��ʾ���ַ���;
 *					textsize : �ַ���С(1:6*8 ; 2:8*16)
 * @retval ��
 */
void OLED_ShowStr(SoftI2C_HandleTypeDef *IICWHERE,unsigned char x, unsigned char y, unsigned char ch[], unsigned char textsize)
{
    unsigned char c = 0, i = 0, j = 0;
    switch (textsize)
    {
    case 1:
    {
        while (ch[j] != '\0')
        {
            c = ch[j] - 32;
            if (x > 126)
            {
                x = 0;
                y++;
            }
            OLED_SetPos(IICWHERE,x, y);
            for (i = 0; i < 6; i++)
                Oled_Write_Data(IICWHERE,F6x8[c][i]);
            x += 6;
            j++;
        }
    }
    break;
    case 2:
    {
        while (ch[j] != '\0')
        {
            c = ch[j] - 32;
            if (x > 120)
            {
                x = 0;
                y++;
            }
            OLED_SetPos(IICWHERE,x, y);
            for (i = 0; i < 8; i++)
                Oled_Write_Data(IICWHERE,F8X16[c * 16 + i]);
            OLED_SetPos(IICWHERE,x, y + 1);
            for (i = 0; i < 8; i++)
                Oled_Write_Data(IICWHERE,F8X16[c * 16 + i + 8]);
            x += 8;
            j++;
        }
    }
    break;
    }
}
//���ֶ�̬��ʾ��ֵ�ı仯��ʽ������һЩ���⣬���ã�������
void OLED_ShowDynamic_FNum(SoftI2C_HandleTypeDef *IICWHERE,u8 x,u8 y,float Fnum,unsigned char textsize)
{
	uint8_t Data[]= " ";                             //����Ŀ�����飬�������ת������ַ����� 
  sprintf(Data,"%.1f",Fnum);                       //����С�����3λС������ӡ��Data������
	OLED_ShowStr(IICWHERE,x,y,Data,textsize);                 //����OLED�ַ�����ʾ��������OLED������ʾ
}
   
void OLED_ShowDynamic_FNumInt(SoftI2C_HandleTypeDef *IICWHERE,u8 x,u8 y,uint8_t Fnum,unsigned char textsize)
{
	uint8_t Data[]= " ";                             //����Ŀ�����飬�������ת������ַ����� 
  sprintf(Data,"%xd",Fnum);                       //����С�����3λС������ӡ��Data������
	OLED_ShowStr(IICWHERE,x,y,Data,textsize);                 //����OLED�ַ�����ʾ��������OLED������ʾ
}

/**
 * @brief  OLED_ShowCN����ʾcodetab.h�еĺ���,16*16����
 * @param  x,y: ��ʼ������(x:0~127, y:0~7); N:������codetab.h�е�����
 *
 * @retval ��
 */
void OLED_ShowCN(SoftI2C_HandleTypeDef *IICWHERE,unsigned char x, unsigned char y, unsigned char n)
{
    unsigned char wm = 0;
    unsigned int adder = 32 * n;
    OLED_SetPos(IICWHERE,x, y);
    for (wm = 0; wm < 16; wm++)
    {
        Oled_Write_Data(IICWHERE,F16x16[adder]);
        adder += 1;
    }
    OLED_SetPos(IICWHERE,x, y + 1);
    for (wm = 0; wm < 16; wm++)
    {
        Oled_Write_Data(IICWHERE,F16x16[adder]);
        adder += 1;
    }
}

/**
 * @brief  OLED_DrawBMP����ʾBMPλͼ
 * @param  x0,y0 :��ʼ������(x0:0~127, y0:0~7);x1,y1 : ���Խ���(������)������(x1:1~128,y1:1~8)
 * @retval ��
 */
void OLED_DrawBMP(SoftI2C_HandleTypeDef *IICWHERE,unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char BMP[])
{
    unsigned int j = 0;
    unsigned char x, y;

    if (y1 % 8 == 0)
    {
        y = y1 / 8;
    }
    else
    {
        y = y1 / 8 + 1;
    }
    for (y = y0; y < y1; y++)
    {
        OLED_SetPos(IICWHERE,x0, y);
        for (x = x0; x < x1; x++)
        {
            Oled_Write_Data(IICWHERE,BMP[j++]);
        }
    }
}

void OLED_Refresh(SoftI2C_HandleTypeDef *IICWHERE,uint8_t second)
{
	Delay_ms(second * 1000); // 2s
  OLED_Fill(IICWHERE,0x00); // ȫ����
   
}



/**
  * @brief  OLED���ù��λ��
  * @param  Y �����Ͻ�Ϊԭ�㣬���·�������꣬��Χ��0~7
  * @param  X �����Ͻ�Ϊԭ�㣬���ҷ�������꣬��Χ��0~127
  * @retval ��
  */
void OLED_SetCursor(SoftI2C_HandleTypeDef *IICWHERE,uint8_t Y, uint8_t X)
{
	Oled_Write_Cmd(IICWHERE,0xB0 | Y);					//����Yλ��
	Oled_Write_Cmd(IICWHERE,0x10 | ((X & 0xF0) >> 4));	//����Xλ�ø�4λ
	Oled_Write_Cmd(IICWHERE,0x00 | (X & 0x0F));			//����Xλ�õ�4λ
}

/**
  * @brief  OLED����
  * @param  ��
  * @retval ��
  */
void OLED_Clear(SoftI2C_HandleTypeDef *IICWHERE)
{  
	uint8_t i, j;
	for (j = 0; j < 8; j++)
	{
		OLED_SetCursor(IICWHERE,j, 0);
		for(i = 0; i < 128; i++)
		{
			Oled_Write_Cmd(IICWHERE,0x00);
		}
	}
}

/**
  * @brief  OLED��ʾһ���ַ�
  * @param  Line ��λ�ã���Χ��1~4
  * @param  Column ��λ�ã���Χ��1~16
  * @param  Char Ҫ��ʾ��һ���ַ�����Χ��ASCII�ɼ��ַ�
  * @retval ��
  */
void OLED_ShowChar(SoftI2C_HandleTypeDef *IICWHERE,uint8_t Line, uint8_t Column, char Char)
{      	
	uint8_t i;
	OLED_SetCursor(IICWHERE,(Line - 1) * 2, (Column - 1) * 8);		//���ù��λ�����ϰ벿��
	for (i = 0; i < 8; i++)
	{
		Oled_Write_Cmd(IICWHERE,OLED_F8x16[Char - ' '][i]);			//��ʾ�ϰ벿������
	}
	OLED_SetCursor(IICWHERE,(Line - 1) * 2 + 1, (Column - 1) * 8);	//���ù��λ�����°벿��
	for (i = 0; i < 8; i++)
	{
		Oled_Write_Cmd(IICWHERE,OLED_F8x16[Char - ' '][i + 8]);		//��ʾ�°벿������
	}
}

/**
  * @brief  OLED��ʾ�ַ���
  * @param  Line ��ʼ��λ�ã���Χ��1~4
  * @param  Column ��ʼ��λ�ã���Χ��1~16
  * @param  String Ҫ��ʾ���ַ�������Χ��ASCII�ɼ��ַ�
  * @retval ��
  */
void OLED_ShowString(SoftI2C_HandleTypeDef *IICWHERE,uint8_t Line, uint8_t Column, char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i++)
	{
		OLED_ShowChar(IICWHERE,Line, Column + i, String[i]);
	}
}

/**
  * @brief  OLED�η�����
  * @retval ����ֵ����X��Y�η�
  */
uint32_t OLED_Pow(SoftI2C_HandleTypeDef *IICWHERE,uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while (Y--)
	{
		Result *= X;
	}
	return Result;
}

/**
  * @brief  OLED��ʾ���֣�ʮ���ƣ�������
  * @param  Line ��ʼ��λ�ã���Χ��1~4
  * @param  Column ��ʼ��λ�ã���Χ��1~16
  * @param  Number Ҫ��ʾ�����֣���Χ��0~4294967295
  * @param  Length Ҫ��ʾ���ֵĳ��ȣ���Χ��1~10
  * @retval ��
  */
void OLED_ShowNum(SoftI2C_HandleTypeDef *IICWHERE,uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(IICWHERE,Line, Column + i, Number / OLED_Pow(IICWHERE,10, Length - i - 1) % 10 + '0');
	}
}

/**
  * @brief  OLED��ʾ���֣�ʮ���ƣ�����������
  * @param  Line ��ʼ��λ�ã���Χ��1~4
  * @param  Column ��ʼ��λ�ã���Χ��1~16
  * @param  Number Ҫ��ʾ�����֣���Χ��-2147483648~2147483647
  * @param  Length Ҫ��ʾ���ֵĳ��ȣ���Χ��1~10
  * @retval ��
  */
void OLED_ShowSignedNum(SoftI2C_HandleTypeDef *IICWHERE,uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length)
{
	uint8_t i;
	uint32_t Number1;
	if (Number >= 0)
	{
		OLED_ShowChar(IICWHERE,Line, Column, '+');
		Number1 = Number;
	}
	else
	{
		OLED_ShowChar(IICWHERE,Line, Column, '-');
		Number1 = -Number;
	}
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(IICWHERE,Line, Column + i + 1, Number1 / OLED_Pow(IICWHERE,10, Length - i - 1) % 10 + '0');
	}
}

/**
  * @brief  OLED��ʾ���֣�ʮ�����ƣ�������
  * @param  Line ��ʼ��λ�ã���Χ��1~4
  * @param  Column ��ʼ��λ�ã���Χ��1~16
  * @param  Number Ҫ��ʾ�����֣���Χ��0~0xFFFFFFFF
  * @param  Length Ҫ��ʾ���ֵĳ��ȣ���Χ��1~8
  * @retval ��
  */
void OLED_ShowHexNum(SoftI2C_HandleTypeDef *IICWHERE,uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i, SingleNumber;
	for (i = 0; i < Length; i++)							
	{
		SingleNumber = Number / OLED_Pow(IICWHERE,16, Length - i - 1) % 16;
		if (SingleNumber < 10)
		{
			OLED_ShowChar(IICWHERE,Line, Column + i, SingleNumber + '0');
		}
		else
		{
			OLED_ShowChar(IICWHERE,Line, Column + i, SingleNumber - 10 + 'A');
		}
	}
}

/**
  * @brief  OLED��ʾ���֣������ƣ�������
  * @param  Line ��ʼ��λ�ã���Χ��1~4
  * @param  Column ��ʼ��λ�ã���Χ��1~16
  * @param  Number Ҫ��ʾ�����֣���Χ��0~1111 1111 1111 1111
  * @param  Length Ҫ��ʾ���ֵĳ��ȣ���Χ��1~16
  * @retval ��
  */
void OLED_ShowBinNum(SoftI2C_HandleTypeDef *IICWHERE,uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(IICWHERE,Line, Column + i, Number / OLED_Pow(IICWHERE,2, Length - i - 1) % 2 + '0');
	}
}


