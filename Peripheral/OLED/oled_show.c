#include "oled_show.h"
#include "oled_Data.h"
/* oled写数据 */
static void Oled_Write_Data(SoftI2C_HandleTypeDef *IICWHERE,uint8_t data)
{
#if I2C_Select
    IIC_Write_Byte(OLED_ID,OLED_WR_DATA,data);
#else
    i2c_Start(IICWHERE);
    i2c_SendByte(IICWHERE,OLED_ID);
    /* 等待应答 */
    while (i2c_WaitAck(IICWHERE));
    i2c_SendByte(IICWHERE,OLED_WR_DATA);
    /* 等待应答 */
    while (i2c_WaitAck(IICWHERE));
    i2c_SendByte(IICWHERE,data);
    /* 等待应答 */
    while (i2c_WaitAck(IICWHERE));
    i2c_Stop(IICWHERE);
#endif
}

/* oled写命令 */
static void Oled_Write_Cmd(SoftI2C_HandleTypeDef *IICWHERE,uint8_t cmd)
{
#if I2C_Select
    IIC_Write_Byte(OLED_ID,OLED_WR_CMD,cmd);
#else
    i2c_Start(IICWHERE);
    i2c_SendByte(IICWHERE,OLED_ID);
    /* 等待应答 */
    while (i2c_WaitAck(IICWHERE));
    i2c_SendByte(IICWHERE,OLED_WR_CMD);
    /* 等待应答 */
    while (i2c_WaitAck(IICWHERE));
    i2c_SendByte(IICWHERE,cmd);
    /* 等待应答 */
    while (i2c_WaitAck(IICWHERE));
    i2c_Stop(IICWHERE);
#endif
}

#if IsLaunchC8T6_IIC

/*引脚配置*/
#define OLED_W_SCL_C8T6(x)		GPIO_WriteBit(GPIOB, GPIO_Pin_8, (BitAction)(x))
#define OLED_W_SDA_C8T6(x)		GPIO_WriteBit(GPIOB, GPIO_Pin_9, (BitAction)(x))

/*引脚初始化*/
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
  * @brief  I2C开始
  * @param  无
  * @retval 无
  */
void OLED_I2C_Start_C8T6(void)
{
	OLED_W_SDA_C8T6(1);
	OLED_W_SCL_C8T6(1);
	OLED_W_SDA_C8T6(0);
	OLED_W_SCL_C8T6(0);
}

/**
  * @brief  I2C停止
  * @param  无
  * @retval 无
  */
void OLED_I2C_Stop_C8T6(void)
{
	OLED_W_SDA_C8T6(0);
	OLED_W_SCL_C8T6(1);
	OLED_W_SDA_C8T6(1);
}

/**
  * @brief  I2C发送一个字节
  * @param  Byte 要发送的一个字节
  * @retval 无
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
	OLED_W_SCL_C8T6(1);	//额外的一个时钟，不处理应答信号
	OLED_W_SCL_C8T6(0);
}

/**
  * @brief  OLED写命令
  * @param  Command 要写入的命令
  * @retval 无
  */
void OLED_WriteCommand_C8T6(uint8_t Command)
{
	OLED_I2C_Start_C8T6();
	OLED_I2C_SendByte_C8T6(0x78);		//从机地址
	OLED_I2C_SendByte_C8T6(0x00);		//写命令
	OLED_I2C_SendByte_C8T6(Command); 
	OLED_I2C_Stop_C8T6();
}

/**
  * @brief  OLED写数据
  * @param  Data 要写入的数据
  * @retval 无
  */
void OLED_WriteData_C8T6(uint8_t Data)
{
	OLED_I2C_Start_C8T6();
	OLED_I2C_SendByte_C8T6(0x78);		//从机地址
	OLED_I2C_SendByte_C8T6(0x40);		//写数据
	OLED_I2C_SendByte_C8T6(Data);
	OLED_I2C_Stop_C8T6();
}

/**
  * @brief  OLED设置光标位置
  * @param  Y 以左上角为原点，向下方向的坐标，范围：0~7
  * @param  X 以左上角为原点，向右方向的坐标，范围：0~127
  * @retval 无
  */
void OLED_SetCursor_C8T6(uint8_t Y, uint8_t X)
{
	OLED_WriteCommand_C8T6(0xB0 | Y);					//设置Y位置
	OLED_WriteCommand_C8T6(0x10 | ((X & 0xF0) >> 4));	//设置X位置高4位
	OLED_WriteCommand_C8T6(0x00 | (X & 0x0F));			//设置X位置低4位
}

/**
  * @brief  OLED清屏
  * @param  无
  * @retval 无
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
  * @brief  OLED显示一个字符
  * @param  Line 行位置，范围：1~4
  * @param  Column 列位置，范围：1~16
  * @param  Char 要显示的一个字符，范围：ASCII可见字符
  * @retval 无
  */
void OLED_ShowChar_C8T6(uint8_t Line, uint8_t Column, char Char)
{      	
	uint8_t i;
	OLED_SetCursor_C8T6((Line - 1) * 2, (Column - 1) * 8);		//设置光标位置在上半部分
	for (i = 0; i < 8; i++)
	{
		OLED_WriteData_C8T6(OLED_F8x16[Char - ' '][i]);			//显示上半部分内容
	}
	OLED_SetCursor_C8T6((Line - 1) * 2 + 1, (Column - 1) * 8);	//设置光标位置在下半部分
	for (i = 0; i < 8; i++)
	{
		OLED_WriteData_C8T6(OLED_F8x16[Char - ' '][i + 8]);		//显示下半部分内容
	}
}

/**
  * @brief  OLED显示字符串
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  String 要显示的字符串，范围：ASCII可见字符
  * @retval 无
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
  * @brief  OLED次方函数
  * @retval 返回值等于X的Y次方
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
  * @brief  OLED显示数字（十进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~4294967295
  * @param  Length 要显示数字的长度，范围：1~10
  * @retval 无
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
 * @brief OLED 显示浮点数（自动计算整数和小数位数）
 * @param Line    行号（0-3）
 * @param Column  列号（0-127）
 * @param Number  要显示的浮点数
 * @param MaxDec  最大小数位数（防止溢出）
 */
void OLED_ShowFloat_C8T6(uint8_t Line, uint8_t Column, float Number, uint8_t MaxDec) 
	{
    char strBuffer[16];  // 临时存储转换后的字符串
    uint8_t strPos = 0;  // 字符串当前位置
    
    // 处理负数
    if (Number < 0) {
        strBuffer[strPos++] = '-';
        Number = -Number;
    }
    
    // 提取整数部分
    uint32_t intPart = (uint32_t)Number;
    float decPart = Number - intPart;
    
    // 计算整数部分位数
    uint8_t intLength = 0;
    uint32_t temp = (intPart == 0) ? 1 : intPart; // 处理 0 情况
    while (temp > 0) {
        temp /= 10;
        intLength++;
    }
    
    // 转换为字符串
    sprintf(&strBuffer[strPos], "%lu.", intPart); // 整数 + 小数点
    strPos += intLength;
    strPos++; // 小数点占1位
    
    // 提取小数部分（最多 MaxDec 位）
    for (uint8_t i = 0; i < MaxDec; i++) {
        decPart *= 10;
        uint8_t digit = (uint8_t)decPart;
        strBuffer[strPos + i] = digit + '0';
        decPart -= digit;
    }
    
    // 去掉末尾的 0（如 3.140 → 3.14）
    uint8_t finalLength = strPos + MaxDec;
    while (finalLength > strPos && strBuffer[finalLength - 1] == '0') {
        finalLength--;
    }
    
    // 如果没有小数部分，去掉小数点（如 100. → 100）
    if (finalLength == strPos) {
        finalLength--;  // 去掉小数点
    }
    
    // 显示字符串
    OLED_ShowString_C8T6(Line, Column, strBuffer);
}


/**
  * @brief  OLED显示数字（十进制，带符号数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：-2147483648~2147483647
  * @param  Length 要显示数字的长度，范围：1~10
  * @retval 无
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
  * @brief  OLED显示数字（十六进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~0xFFFFFFFF
  * @param  Length 要显示数字的长度，范围：1~8
  * @retval 无
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
  * @brief  OLED显示数字（二进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~1111 1111 1111 1111
  * @param  Length 要显示数字的长度，范围：1~16
  * @retval 无
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
  * @brief  OLED初始化
  * @param  无
  * @retval 无
  */
void OLED_Init_C8T6(void)
{
	uint32_t i, j;
	
	for (i = 0; i < 1000; i++)			//上电延时
	{
		for (j = 0; j < 1000; j++);
	}
	
	OLED_I2C_Init_C8T6();			//端口初始化
	
	OLED_WriteCommand_C8T6(0xAE);	//关闭显示
	
	OLED_WriteCommand_C8T6(0xD5);	//设置显示时钟分频比/振荡器频率
	OLED_WriteCommand_C8T6(0x80);
	
	OLED_WriteCommand_C8T6(0xA8);	//设置多路复用率
	OLED_WriteCommand_C8T6(0x3F);
	
	OLED_WriteCommand_C8T6(0xD3);	//设置显示偏移
	OLED_WriteCommand_C8T6(0x00);
	
	OLED_WriteCommand_C8T6(0x40);	//设置显示开始行
	
	OLED_WriteCommand_C8T6(0xA1);	//设置左右方向，0xA1正常 0xA0左右反置
	
	OLED_WriteCommand_C8T6(0xC8);	//设置上下方向，0xC8正常 0xC0上下反置

	OLED_WriteCommand_C8T6(0xDA);	//设置COM引脚硬件配置
	OLED_WriteCommand_C8T6(0x12);
	
	OLED_WriteCommand_C8T6(0x81);	//设置对比度控制
	OLED_WriteCommand_C8T6(0xCF);

	OLED_WriteCommand_C8T6(0xD9);	//设置预充电周期
	OLED_WriteCommand_C8T6(0xF1);

	OLED_WriteCommand_C8T6(0xDB);	//设置VCOMH取消选择级别
	OLED_WriteCommand_C8T6(0x30);

	OLED_WriteCommand_C8T6(0xA4);	//设置整个显示打开/关闭

	OLED_WriteCommand_C8T6(0xA6);	//设置正常/倒转显示

	OLED_WriteCommand_C8T6(0x8D);	//设置充电泵
	OLED_WriteCommand_C8T6(0x14);

	OLED_WriteCommand_C8T6(0xAF);	//开启显示
		
	OLED_Clear_C8T6();				//OLED清屏
}

#endif


void OLED_Init(SoftI2C_HandleTypeDef *IICWHERE)
{
		i2c_CfgGpio(IICWHERE);
    /* 设置显示打开/关闭
     * AE--->显示打开
     * AF--->显示关闭(休眠模式)
     */
    Oled_Write_Cmd(IICWHERE,0xAE);

    /* ================== 基本命令表 ===================*/
    /* 设置对比度
     * 0~255：数值越大 亮度越亮
     */
    Oled_Write_Cmd(IICWHERE,0x81);
    Oled_Write_Cmd(IICWHERE,0xFF);

    /* 使能全屏显示
     * A4--->恢复到RAM内容显示
     * A5--->忽略RAM内容显示
     */
    Oled_Write_Cmd(IICWHERE,0xA4);

    /* 设置显示模式
     * A6--->正常显示：0灭1亮
     * A7--->逆显示：1灭0亮
     */
    Oled_Write_Cmd(IICWHERE,0xA6);

    /* ================== 滚动命令表 ===================*/
    /* 滚动使能/失能
     * 2E--->失能
     * 2F--->使能
     */
    Oled_Write_Cmd(IICWHERE,0x2E);

    /* 七字节命令： 连续水平滚动设置 */

    /* 左/右水平滚动设置
     * 26--->右水平滚动
     * 27--->左水平滚动
     */
    Oled_Write_Cmd(IICWHERE,0x26);
    /* 虚拟字节 */
    Oled_Write_Cmd(IICWHERE,0x00);
    /* 设置滚动起始页地址 */
    Oled_Write_Cmd(IICWHERE,0x00);
    /* 设置滚动间隔 */
    Oled_Write_Cmd(IICWHERE,0x03);
    /* 设置滚动结束地址 */
    Oled_Write_Cmd(IICWHERE,0x07);
    /* 虚拟字节 */
    Oled_Write_Cmd(IICWHERE,0x00);
    Oled_Write_Cmd(IICWHERE,0xFF);

    /* =============== 寻址设置命令表 ==================*/

    /* 双字节命令:寄存器寻址模式 */
    Oled_Write_Cmd(IICWHERE,0x20);

    /* 10:页寻址模式
     * 01:垂直寻址模式
     * 00:水平寻址模式
     */
    Oled_Write_Cmd(IICWHERE,0x10);
    /* 单字节命令:设置页寻址的起始页地址 */
    Oled_Write_Cmd(IICWHERE,0xB0);
    /* 单字节命令:设置页寻址的起始列地址低位 */
    Oled_Write_Cmd(IICWHERE,0x00);
    /* 单字节命令:设置页寻址的起始列地址高位 */
    Oled_Write_Cmd(IICWHERE,0x10);

    /*=============== 硬件配置命令表 ==================*/

    /* 设置显示开始线
     * 0x40~0x7F对应0~63
     */
    Oled_Write_Cmd(IICWHERE,0x40);

    /* 设置列重映射
     * A0:addressX--->segX
     * A1:addressX--->seg(127-X)
     */
    Oled_Write_Cmd(IICWHERE,0xA1);

    /* 设置多路复用比 */
    Oled_Write_Cmd(IICWHERE,0xA8);
    Oled_Write_Cmd(IICWHERE,0x3F);

    /* 设置COM输出扫描方向
     * C0：COM0--->COM63(从上往下扫描)
     * C8：COM63--->COM0(从下往上扫描)
     */
    Oled_Write_Cmd(IICWHERE,0xC8);

    /* 双字节命令：设置COM显示偏移量 */
    Oled_Write_Cmd(IICWHERE,0xD3);
    Oled_Write_Cmd(IICWHERE,0x00); /* COM不偏移 */

    /* 双字节命令：配置COM重映射 */
    Oled_Write_Cmd(IICWHERE,0xDA);
    Oled_Write_Cmd(IICWHERE,0x12);

    /* 双字节命令：设置预充期 */
    Oled_Write_Cmd(IICWHERE,0xD9);
    Oled_Write_Cmd(IICWHERE,0x22); /* 阶段一2个无效DCLK时钟/阶段二2个无效DCLK时钟 */

    /* 设置VCOMH取消选择电平
     * 00:0.65xVcc
     * 20:0.77xVcc
     * 30:0.83xVcc
     */
    Oled_Write_Cmd(IICWHERE,0xDB);
    Oled_Write_Cmd(IICWHERE,0x20);

    /* 双字节命令：设置电荷泵 */
    Oled_Write_Cmd(IICWHERE,0x8d);
    Oled_Write_Cmd(IICWHERE,0x14);

    Oled_Write_Cmd(IICWHERE,0xAF);
}

/**
 * @brief  设置光标
 * @param  x,光标x位置
 *		   y,光标y位置
 * @retval 无
 */
void OLED_SetPos(SoftI2C_HandleTypeDef *IICWHERE,unsigned char x, unsigned char y) // 设置起始点坐标
{
    Oled_Write_Cmd(IICWHERE,0xb0 + y);
    Oled_Write_Cmd(IICWHERE,((x & 0xf0) >> 4) | 0x10);
    Oled_Write_Cmd(IICWHERE,(x & 0x0f) | 0x01);
}

/**
 * @brief  填充整个屏幕
 * @param  fill_data:要填充的数据
 * @retval 无
 */
void OLED_Fill(SoftI2C_HandleTypeDef *IICWHERE,unsigned char fill_data) // 全屏填充
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
 * @brief  清屏
 * @param  无
 * @retval 无
 */
void OLED_CLS(SoftI2C_HandleTypeDef *IICWHERE) // 清屏
{
    OLED_Fill(IICWHERE,0x00);
}

/**
 * @brief  将OLED从休眠中唤醒
 * @param  无
 * @retval 无
 */
void OLED_ON(SoftI2C_HandleTypeDef *IICWHERE)
{
    Oled_Write_Cmd(IICWHERE,0X8D); // 设置电荷泵
    Oled_Write_Cmd(IICWHERE,0X14); // 开启电荷泵
    Oled_Write_Cmd(IICWHERE,0XAF); // OLED唤醒
}

/**
 * @brief  让OLED休眠 -- 休眠模式下,OLED功耗不到10uA
 * @param  无
 * @retval 无
 */
void OLED_OFF(SoftI2C_HandleTypeDef *IICWHERE)
{
    Oled_Write_Cmd(IICWHERE,0X8D); // 设置电荷泵
    Oled_Write_Cmd(IICWHERE,0X10); // 关闭电荷泵
    Oled_Write_Cmd(IICWHERE,0XAE); // OLED休眠
}

/**
 * @brief 显示codetab.h中的ASCII字符,有6*8和8*16可选择
 * @param  x,y : 起始点坐标(x:0~127, y:0~7);
 *					ch[] :- 要显示的字符串;
 *					textsize : 字符大小(1:6*8 ; 2:8*16)
 * @retval 无
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
//这种动态显示数值的变化方式，存在一些问题，慎用！！！！
void OLED_ShowDynamic_FNum(SoftI2C_HandleTypeDef *IICWHERE,u8 x,u8 y,float Fnum,unsigned char textsize)
{
	uint8_t Data[]= " ";                             //创建目标数组，用来存放转换后的字符数据 
  sprintf(Data,"%.1f",Fnum);                       //保留小数点后3位小数，打印到Data数组中
	OLED_ShowStr(IICWHERE,x,y,Data,textsize);                 //调用OLED字符串显示函数，在OLED屏上显示
}
   
void OLED_ShowDynamic_FNumInt(SoftI2C_HandleTypeDef *IICWHERE,u8 x,u8 y,uint8_t Fnum,unsigned char textsize)
{
	uint8_t Data[]= " ";                             //创建目标数组，用来存放转换后的字符数据 
  sprintf(Data,"%xd",Fnum);                       //保留小数点后3位小数，打印到Data数组中
	OLED_ShowStr(IICWHERE,x,y,Data,textsize);                 //调用OLED字符串显示函数，在OLED屏上显示
}

/**
 * @brief  OLED_ShowCN，显示codetab.h中的汉字,16*16点阵
 * @param  x,y: 起始点坐标(x:0~127, y:0~7); N:汉字在codetab.h中的索引
 *
 * @retval 无
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
 * @brief  OLED_DrawBMP，显示BMP位图
 * @param  x0,y0 :起始点坐标(x0:0~127, y0:0~7);x1,y1 : 起点对角线(结束点)的坐标(x1:1~128,y1:1~8)
 * @retval 无
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
  OLED_Fill(IICWHERE,0x00); // 全屏灭
   
}



/**
  * @brief  OLED设置光标位置
  * @param  Y 以左上角为原点，向下方向的坐标，范围：0~7
  * @param  X 以左上角为原点，向右方向的坐标，范围：0~127
  * @retval 无
  */
void OLED_SetCursor(SoftI2C_HandleTypeDef *IICWHERE,uint8_t Y, uint8_t X)
{
	Oled_Write_Cmd(IICWHERE,0xB0 | Y);					//设置Y位置
	Oled_Write_Cmd(IICWHERE,0x10 | ((X & 0xF0) >> 4));	//设置X位置高4位
	Oled_Write_Cmd(IICWHERE,0x00 | (X & 0x0F));			//设置X位置低4位
}

/**
  * @brief  OLED清屏
  * @param  无
  * @retval 无
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
  * @brief  OLED显示一个字符
  * @param  Line 行位置，范围：1~4
  * @param  Column 列位置，范围：1~16
  * @param  Char 要显示的一个字符，范围：ASCII可见字符
  * @retval 无
  */
void OLED_ShowChar(SoftI2C_HandleTypeDef *IICWHERE,uint8_t Line, uint8_t Column, char Char)
{      	
	uint8_t i;
	OLED_SetCursor(IICWHERE,(Line - 1) * 2, (Column - 1) * 8);		//设置光标位置在上半部分
	for (i = 0; i < 8; i++)
	{
		Oled_Write_Cmd(IICWHERE,OLED_F8x16[Char - ' '][i]);			//显示上半部分内容
	}
	OLED_SetCursor(IICWHERE,(Line - 1) * 2 + 1, (Column - 1) * 8);	//设置光标位置在下半部分
	for (i = 0; i < 8; i++)
	{
		Oled_Write_Cmd(IICWHERE,OLED_F8x16[Char - ' '][i + 8]);		//显示下半部分内容
	}
}

/**
  * @brief  OLED显示字符串
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  String 要显示的字符串，范围：ASCII可见字符
  * @retval 无
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
  * @brief  OLED次方函数
  * @retval 返回值等于X的Y次方
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
  * @brief  OLED显示数字（十进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~4294967295
  * @param  Length 要显示数字的长度，范围：1~10
  * @retval 无
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
  * @brief  OLED显示数字（十进制，带符号数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：-2147483648~2147483647
  * @param  Length 要显示数字的长度，范围：1~10
  * @retval 无
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
  * @brief  OLED显示数字（十六进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~0xFFFFFFFF
  * @param  Length 要显示数字的长度，范围：1~8
  * @retval 无
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
  * @brief  OLED显示数字（二进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~1111 1111 1111 1111
  * @param  Length 要显示数字的长度，范围：1~16
  * @retval 无
  */
void OLED_ShowBinNum(SoftI2C_HandleTypeDef *IICWHERE,uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for (i = 0; i < Length; i++)							
	{
		OLED_ShowChar(IICWHERE,Line, Column + i, Number / OLED_Pow(IICWHERE,2, Length - i - 1) % 2 + '0');
	}
}


