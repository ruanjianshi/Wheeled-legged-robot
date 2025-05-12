#include "filter.h"
//int A[2][2] = {{0,1},{2,4}};
//int B[2][2] = {{0,1},{2,4}};
//int C= 1;
//int D = 2;
//int Q[2][2] = {{0,1},{2,4}};
//int P[2][2] = {{0,1},{2,4}};
/***********************************************
低通滤波器:
***********************************************/
float LowPass_Filter(float x)
{
    float y_prev;
	float y = 0.9*y_prev + 0.1*x;
	
	y_prev=y;
	
	return y;
}


/***********************************************
磁编码器速度低通滤波计算:(0-∞)
***********************************************/
float Get_LowPass_Filter_Speed(SoftI2C_HandleTypeDef *IICWHERE)
{
	//速度原始数据采集
	float vel_M0_ori=Calculate_Velocity(IICWHERE);
	//原始数据低通滤波
	float vel_M0_flit=LowPass_Filter(Correction_direction*vel_M0_ori);
	return vel_M0_flit;
}


float filtered_angle = 0;

float Get_Filtered_Angle(SoftI2C_HandleTypeDef *IICWHERE) 
{
    float raw = Get_Angle_Radian_Accumulation(IICWHERE);
    filtered_angle = filtered_angle * (1 - FILTER_COEFF) + raw * FILTER_COEFF;
    return filtered_angle;
}

//void kalman_filter(float z, float u) {
//    // 预测状态
//    float x_pred[2] = {
//        A[0][0] * x[0] + A[0][1] * x[1] + B[0] * u,
//        A[1][0] * x[0] + A[1][1] * x[1] + B[1] * u
//    };

//    // 预测误差协方差矩阵
//    float P_pred[2][2] = {
//        {A[0][0]*P[0][0] + A[0][1]*P[1][0], A[0][0]*P[0][1] + A[0][1]*P[1][1]},
//        {A[1][0]*P[0][0] + A[1][1]*P[1][0], A[1][0]*P[0][1] + A[1][1]*P[1][1]}
//    };
//    P_pred[0][0] += Q[0][0];
//    P_pred[0][1] += Q[0][1];
//    P_pred[1][0] += Q[1][0];
//    P_pred[1][1] += Q[1][1];

//    // 计算卡尔曼增益
//    float S = C[0] * (P_pred[0][0] * C[0] + P_pred[1][0] * C[1]) + C[1] * (P_pred[0][1] * C[0] + P_pred[1][1] * C[1]) + R;
//    float K[2] = {
//        (P_pred[0][0] * C[0] + P_pred[1][0] * C[1]) / S,
//        (P_pred[0][1] * C[0] + P_pred[1][1] * C[1]) / S
//    };

//    // 更新状态
//    float y = z - (C[0] * x_pred[0] + C[1] * x_pred[1] + D * u);
//    x[0] = x_pred[0] + K[0] * y;
//    x[1] = x_pred[1] + K[1] * y;

//    // 更新误差协方差矩阵
//    P[0][0] = (1 - K[0] * C[0]) * P_pred[0][0] - K[0] * C[1] * P_pred[1][0];
//    P[0][1] = (1 - K[0] * C[0]) * P_pred[0][1] - K[0] * C[1] * P_pred[1][1];
//    P[1][0] = -K[1] * C[0] * P_pred[0][0] + (1 - K[1] * C[1]) * P_pred[1][0];
//    P[1][1] = -K[1] * C[0] * P_pred[0][1] + (1 - K[1] * C[1]) * P_pred[1][1];
//}

