#ifndef VOFA_JUSTFLOAT_H
#define VOFA_JUSTFLOAT_H
 
#include "stm32f10x.h"
 
#define CH_COUNT 10  // 数据通道数量
 
// VOFA+ JustFloat 协议的帧结构体
typedef struct {
    float fdata[CH_COUNT];       // 存储通道数据的数组
    unsigned char tail[4];       // JustFloat 协议的尾部标识
} Frame;

typedef union
{
    float fdata;
    unsigned long ldata;
} FloatLongType;
 
 
// 初始化帧结构体的函数
void Float_to_Byte(float f,unsigned char byte[]);
//主要数据量的同时，需要开始相应的接受通道。
void JustFloatTONGXIN_Array(u16 *data,uint8_t count);
void JustFloatTONGXIN_MPointer(u8 **ptr,uint8_t count);
void JustFloatTONGXIN_SPointer(u8 *ptr,uint8_t positon);
void JustFloat_Test(void);    //justfloat 数据协议测试
void JustFloatTONGXIN_Array_uint16_t(int16_t *data,uint8_t count);
#endif // VOFA_JUSTFLOAT_H

