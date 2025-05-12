#ifndef VOFA_JUSTFLOAT_H
#define VOFA_JUSTFLOAT_H
 
#include "stm32f10x.h"
 
#define CH_COUNT 10  // ����ͨ������
 
// VOFA+ JustFloat Э���֡�ṹ��
typedef struct {
    float fdata[CH_COUNT];       // �洢ͨ�����ݵ�����
    unsigned char tail[4];       // JustFloat Э���β����ʶ
} Frame;

typedef union
{
    float fdata;
    unsigned long ldata;
} FloatLongType;
 
 
// ��ʼ��֡�ṹ��ĺ���
void Float_to_Byte(float f,unsigned char byte[]);
//��Ҫ��������ͬʱ����Ҫ��ʼ��Ӧ�Ľ���ͨ����
void JustFloatTONGXIN_Array(u16 *data,uint8_t count);
void JustFloatTONGXIN_MPointer(u8 **ptr,uint8_t count);
void JustFloatTONGXIN_SPointer(u8 *ptr,uint8_t positon);
void JustFloat_Test(void);    //justfloat ����Э�����
void JustFloatTONGXIN_Array_uint16_t(int16_t *data,uint8_t count);
#endif // VOFA_JUSTFLOAT_H

