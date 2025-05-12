#include "vofa_JUSTfloat.h"
#include "usart.h"
//������ת��Ϊ4���ֽڷ���
Frame fl;
 
// ��ʼ��֡�ṹ��
static void Frame_Init(Frame* frame) {
    // ���� JustFloat Э���β����ʶ
    frame->tail[0] = 0x00;
    frame->tail[1] = 0x00;
    frame->tail[2] = 0x80;
    frame->tail[3] = 0x7f;
}
/*
��������fת��Ϊ4���ֽ����ݴ����byte[4]��
*/
void Float_to_Byte(float f,unsigned char byte[])
{
    FloatLongType fl;
    fl.fdata=f;
    byte[0]=(unsigned char)fl.ldata;
    byte[1]=(unsigned char)(fl.ldata>>8);
    byte[2]=(unsigned char)(fl.ldata>>16);
    byte[3]=(unsigned char)(fl.ldata>>24);
}

//���ݷ��ͷ�װ��������������ʽ���Ա�����������������
void JustFloatTONGXIN_Array(u16 *data,uint8_t count)
{
	uint8_t i;
	u8 byte[4] = {0};
	Frame_Init(&fl);
	
	for(i = 0; i < count; i++)
	{
		fl.fdata[i] = data[i];
		Float_to_Byte(fl.fdata[i],byte);
		Usart_SendArray(TONGXIN_USARTx,byte,4);
	}
  Usart_SendArray(TONGXIN_USARTx,fl.tail,4);
}

void JustFloatTONGXIN_Array_uint16_t(int16_t *data,uint8_t count)
{
	uint8_t i;
	uint8_t byte[4] = {0};
	Frame_Init(&fl);
	
	for(i = 0; i < count; i++)
	{
		fl.fdata[i] = data[i];
		Float_to_Byte(fl.fdata[i],byte);
		Usart_SendArray(TONGXIN_USARTx,byte,4);
	}
  Usart_SendArray(TONGXIN_USARTx,fl.tail,4);
}

//���ݷ��ͷ�װ���������������ʽ���Ա�����������������
void JustFloatTONGXIN_MPointer(u8 **ptr,uint8_t count)
{
	  u8 byte[4] = {0};
		static uint8_t i = 0;
		float* float_ptr = (float*)(ptr[i]);
    Frame_Init(&fl);  // ȷ�� fl.fdata Ϊ float ����

    // ת��Ϊ float* ����ָ�룬��ƫ�Ʒ��ʸ�����
    fl.fdata[i] = *float_ptr;

    // ת��Ϊ�ֽ�����ͨ�����ڷ���

    Float_to_Byte(fl.fdata[i], byte);
    Usart_SendArray(TONGXIN_USARTx, byte, 4);
		Usart_SendArray(TONGXIN_USARTx,fl.tail,4);

    // ����������ѭ����i��Χ: 0 ~ count-1��
    i = (i < count - 1) ? (i + 1) : 0;
}
//��ͨ��λ�õ�ѡ�񣬲�������Ԥ��Ч��һ�������������ʾ��Ч���ǣ�0-10������ʾ��
void JustFloatTONGXIN_SPointer(u8 *ptr,uint8_t positon)
{
	u8 byte[4] = {0};
	Frame_Init(&fl);
	fl.fdata[positon] = *ptr; 
  Float_to_Byte(fl.fdata[positon], byte);
  Usart_SendArray(TONGXIN_USARTx, byte, 4);
	Usart_SendArray(TONGXIN_USARTx,fl.tail,4);
}

void JustFloat_Test(void)    //justfloat ����Э�����
{
    u8 byte[4]={0};     //floatת��Ϊ4���ֽ�����
		Frame_Init(&fl);
    //����λ�������и���
    fl.fdata[0] = 2; 
    fl.fdata[1] = 4;    //���͵����� ����ͨ��
 
    //����λ����������ͨ������
    Float_to_Byte(fl.fdata[0],byte);
    //u1_printf("%f\r\n",a);
    Usart_SendArray(TONGXIN_USARTx,byte,4);
 
    //u1_SendArray(byte,4); //1ת��Ϊ4�ֽ����� ����  0x00 0x00 0x80 0x3F
 
    Float_to_Byte(fl.fdata[1],byte);
    //u1_SendArray(byte,4); //2ת��Ϊ4�ֽ����� ����  0x00 0x00 0x00 0x40 
    Usart_SendArray(TONGXIN_USARTx,byte,4);
 
    //����֡β
    //u1_SendArray(tail,4); //֡βΪ 0x00 0x00 0x80 0x7f
    Usart_SendArray(TONGXIN_USARTx,fl.tail,4);
		
 
}



