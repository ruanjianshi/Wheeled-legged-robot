#include "as5600.h"

void AS5600_INIT(SoftI2C_HandleTypeDef *IICWHERE)
{
	i2c_CfgGpio(IICWHERE);
}
/***************************************************************************/
u8 AS5600_ReadOneByte(SoftI2C_HandleTypeDef *IICWHERE,u8 addr)
{
	u8 temp;		  	    																 
	
	i2c_Start(IICWHERE);
	i2c_SendByte(IICWHERE,AS5600_Address<<1);
	while(i2c_WaitAck(IICWHERE));
	i2c_SendByte(IICWHERE,addr);
	while(i2c_WaitAck(IICWHERE)); 
  i2c_Stop(IICWHERE);	
	
	i2c_Start(IICWHERE);  	 	   
	i2c_SendByte(IICWHERE,(AS5600_Address<<1)+1);
	while(i2c_WaitAck(IICWHERE));	 
	temp=i2c_ReadByte(IICWHERE,0);		   
	i2c_Stop(IICWHERE);
	
	return temp;
}
/***************************************************************************/
float AS5600_ReadRawAngleTwo(SoftI2C_HandleTypeDef *IICWHERE)
{
	u8 dh,dl;		  	    																 
	
	i2c_Start(IICWHERE);
	i2c_SendByte(IICWHERE,AS5600_Address<<1);
	while(i2c_WaitAck(IICWHERE));
	i2c_SendByte(IICWHERE,RAW_Angle_Hi);
	while(i2c_WaitAck(IICWHERE));
	i2c_Stop(IICWHERE);	
	
	i2c_Start(IICWHERE);
	i2c_SendByte(IICWHERE,(AS5600_Address<<1)+1);
	while(i2c_WaitAck(IICWHERE));
	dh=i2c_ReadByte(IICWHERE,1);   //1-ack for next byte
	dl=i2c_ReadByte(IICWHERE,0);   //0-end trans
	i2c_Stop(IICWHERE);
	
	return (((dh<<8)+dl)*0.08789);
}
/***************************************************************************/
unsigned short noAS5600_ReadRawAngleTwo(SoftI2C_HandleTypeDef *IICWHERE)
{
	u8 dh,dl;		  	    																 
	
	i2c_Start(IICWHERE);
	i2c_SendByte(IICWHERE,AS5600_Address<<1);
	while(i2c_WaitAck(IICWHERE));
	i2c_SendByte(IICWHERE,RAW_Angle_Hi);
	while(i2c_WaitAck(IICWHERE));
	i2c_Stop(IICWHERE);	
	
	i2c_Start(IICWHERE);
	i2c_SendByte(IICWHERE,(AS5600_Address<<1)+1);
	while(i2c_WaitAck(IICWHERE));
	dh=i2c_ReadByte(IICWHERE,1);   //1-ack for next byte
	dl=i2c_ReadByte(IICWHERE,0);   //0-end trans
	i2c_Stop(IICWHERE);
	
	return ((dh<<8)+dl);
}

/***********************************************
读取磁编码器360度值:(0°-360°)
***********************************************/
float Get_Angle_Degree(SoftI2C_HandleTypeDef *IICWHERE)
{
	return AS5600_ReadRawAngleTwo(IICWHERE);
}

/***********************************************
读取磁编码器归一化弧度值:(0-6.28)
***********************************************/
float Get_Angle_Radian(SoftI2C_HandleTypeDef *IICWHERE)
{
	return AS5600_ReadRawAngleTwo(IICWHERE) / 57.32484;
}
/***********************************************
磁编码器弧度制角度累计计算:(0-∞)
***********************************************/
float Get_Angle_Radian_Accumulation(SoftI2C_HandleTypeDef *IICWHERE)
{
	static float angle_previou = 0,full_rotations = 0;
	float angle_current = Get_Angle_Radian(IICWHERE);
	float deviation_angle = angle_current - angle_previou;
	//计算旋转的圈数
	//通过判断角度变化是否大于80%的一圈(0.8f*6.28318530718f)来判断是否发生了溢出
	//如果发生了溢出,则将full_rotations增加1(如果d_angle小于0)或减少1(如果d_angle大于0)
	if( fabs(deviation_angle) > (0.8f*6.28318530718f) )
		full_rotations += (deviation_angle > 0) ? -1 : 1;
	angle_previou = angle_current;
	return (float)full_rotations * 6.28318530718f + angle_previou;
}
/***********************************************/
