#ifndef __OLED_SHOW_H
#define __OLED_SHOW_H

#include "stm32f10x.h"
#include "iic.h"
#include "systick.h"
#include "stdio.h"

//X轴（水平方向）：0~127（共128像素）
//Y轴（垂直方向）：0~63（共64像素，按页划分,总分为8页，每页八个像素）
/* 
		X   1 -> 128 
		Y   0 -> 64 (8x8)
		方向：从左至右，从上往下。
*/
#define OLED_8x16 2  //X:8 Y:16
#define OLED_6x8  1

#define OLED_ID          0x78
#define OLED_WR_CMD      0x00
#define OLED_WR_DATA     0x40

void OLED_Init(SoftI2C_HandleTypeDef *IICWHERE);
void OLED_SetPos(SoftI2C_HandleTypeDef *IICWHERE,unsigned char x, unsigned char y);
void OLED_Fill(SoftI2C_HandleTypeDef *IICWHERE,unsigned char fill_data);
void OLED_CLS(SoftI2C_HandleTypeDef *IICWHERE);
void OLED_ON(SoftI2C_HandleTypeDef *IICWHERE);
void OLED_OFF(SoftI2C_HandleTypeDef *IICWHERE);
void OLED_ShowStr(SoftI2C_HandleTypeDef *IICWHERE,unsigned char x, unsigned char y, unsigned char ch[], unsigned char textsize);
void OLED_ShowDynamic_FNum(SoftI2C_HandleTypeDef *IICWHERE,u8 x,u8 y,float Fnum,unsigned char textsize);
void OLED_ShowCN(SoftI2C_HandleTypeDef *IICWHERE,unsigned char x, unsigned char y, unsigned char n);
void OLED_DrawBMP(SoftI2C_HandleTypeDef *IICWHERE,unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);
void OLED_Refresh(SoftI2C_HandleTypeDef *IICWHERE,uint8_t second);
void OLED_ShowDynamic_FNumInt(SoftI2C_HandleTypeDef *IICWHERE,u8 x,u8 y,uint8_t Fnum,unsigned char textsize);

#define IsLaunchC8T6_IIC 1
#if IsLaunchC8T6_IIC
void OLED_Init_C8T6(void);
void OLED_Clear_C8T6(void);
void OLED_ShowChar_C8T6(uint8_t Line, uint8_t Column, char Char);
void OLED_ShowString_C8T6(uint8_t Line, uint8_t Column, char *String);
void OLED_ShowNum_C8T6(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowSignedNum_C8T6(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length);
void OLED_ShowHexNum_C8T6(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowBinNum_C8T6(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowFloat_C8T6(uint8_t Line, uint8_t Column, float Number, uint8_t MaxDec);
#endif

void OLED_Clear(SoftI2C_HandleTypeDef *IICWHERE);
void OLED_ShowChar(SoftI2C_HandleTypeDef *IICWHERE,uint8_t Line, uint8_t Column, char Char);
void OLED_ShowString(SoftI2C_HandleTypeDef *IICWHERE,uint8_t Line, uint8_t Column, char *String);
void OLED_ShowNum(SoftI2C_HandleTypeDef *IICWHERE,uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowSignedNum(SoftI2C_HandleTypeDef *IICWHERE,uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length);
void OLED_ShowHexNum(SoftI2C_HandleTypeDef *IICWHERE,uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowBinNum(SoftI2C_HandleTypeDef *IICWHERE,uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
#endif
