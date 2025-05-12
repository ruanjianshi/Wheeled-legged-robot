#include "vofa_JUSTfloat.h"
#include "usart.h"
//浮点型转化为4个字节发送
Frame fl;
 
// 初始化帧结构体
static void Frame_Init(Frame* frame) {
    // 设置 JustFloat 协议的尾部标识
    frame->tail[0] = 0x00;
    frame->tail[1] = 0x00;
    frame->tail[2] = 0x80;
    frame->tail[3] = 0x7f;
}
/*
将浮点数f转化为4个字节数据存放在byte[4]中
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

//数据发送封装函数，数组表达形式，自变量：数据量，数据
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

//数据发送封装函数，变量表达形式，自变量：变量，个数。
void JustFloatTONGXIN_MPointer(u8 **ptr,uint8_t count)
{
	  u8 byte[4] = {0};
		static uint8_t i = 0;
		float* float_ptr = (float*)(ptr[i]);
    Frame_Init(&fl);  // 确保 fl.fdata 为 float 数组

    // 转换为 float* 类型指针，按偏移访问浮点数
    fl.fdata[i] = *float_ptr;

    // 转换为字节流并通过串口发送

    Float_to_Byte(fl.fdata[i], byte);
    Usart_SendArray(TONGXIN_USARTx, byte, 4);
		Usart_SendArray(TONGXIN_USARTx,fl.tail,4);

    // 更新索引并循环（i范围: 0 ~ count-1）
    i = (i < count - 1) ? (i + 1) : 0;
}
//该通道位置的选择，并不会如预料效果一样，在软件上显示的效果是，0-10依次显示。
void JustFloatTONGXIN_SPointer(u8 *ptr,uint8_t positon)
{
	u8 byte[4] = {0};
	Frame_Init(&fl);
	fl.fdata[positon] = *ptr; 
  Float_to_Byte(fl.fdata[positon], byte);
  Usart_SendArray(TONGXIN_USARTx, byte, 4);
	Usart_SendArray(TONGXIN_USARTx,fl.tail,4);
}

void JustFloat_Test(void)    //justfloat 数据协议测试
{
    u8 byte[4]={0};     //float转化为4个字节数据
		Frame_Init(&fl);
    //数据位：可自行更改
    fl.fdata[0] = 2; 
    fl.fdata[1] = 4;    //发送的数据 两个通道
 
    //向上位机发送两个通道数据
    Float_to_Byte(fl.fdata[0],byte);
    //u1_printf("%f\r\n",a);
    Usart_SendArray(TONGXIN_USARTx,byte,4);
 
    //u1_SendArray(byte,4); //1转化为4字节数据 就是  0x00 0x00 0x80 0x3F
 
    Float_to_Byte(fl.fdata[1],byte);
    //u1_SendArray(byte,4); //2转换为4字节数据 就是  0x00 0x00 0x00 0x40 
    Usart_SendArray(TONGXIN_USARTx,byte,4);
 
    //发送帧尾
    //u1_SendArray(tail,4); //帧尾为 0x00 0x00 0x80 0x7f
    Usart_SendArray(TONGXIN_USARTx,fl.tail,4);
		
 
}



