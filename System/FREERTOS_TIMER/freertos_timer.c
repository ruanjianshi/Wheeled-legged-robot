#include "freertos_timer.h"

uint32_t FreeRTOSRunTimeTicks;        /* FreeRTOS��������ʱ��ͳ�Ʊ��� */

/**********************************************************
* @funcName ��ConfigureTimeForRunTimeStats
* @brief    ��FreeRTOS��������ʱ��ͳ�Ƶ�ʱ����ʱ����ʼ��
* @param    ��void
* @retval   ��void
* @details  ��ʹ�õ�ʱ����ʱ���Ƕ�ʱ��6
* @fn       ��
************************************************************/
void ConfigureTimeForRunTimeStats(void)
{
    TIM4_Init(9, 72);             /* 100����ϵͳʱ�ӽ��� */
    FreeRTOSRunTimeTicks = 0;
}


/**********************************************************
* @funcName ��TIM6_Init
* @brief    ���Զ�ʱ��6���г�ʼ������
* @param    ��uint16_t arr (����ֵ)
* @param    ��uint16_t psc (Ԥ��Ƶֵ)
* @retval   ��void
* @details  ��
* @fn       ����ʱ��6��ռ���ȼ�4�������ȼ�0
************************************************************/
void TIM4_Init(uint16_t arr, uint16_t psc)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;  /* TIMx���ýṹ�� */
    NVIC_InitTypeDef NVIC_InitStruct;                /* �ж����ýṹ�� */

    /* TIMxʱ��ʹ�� */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

    /*TIMx��ʼ������*/
    TIM_TimeBaseInitStruct.TIM_Period        = arr;                 /* ����ֵ(TIMx_ARR�Ĵ���) */
    TIM_TimeBaseInitStruct.TIM_Prescaler     = psc;                 /* ��Ƶ(TIMx_PSC�Ĵ���) */
//    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;      /* TDTS = Tck_tim(TIMx_CR1�Ĵ����ĵ�9-8λ)(������ʱ��û��) */
//    TIM_TimeBaseInitStruct.TIM_CounterMode   = TIM_CounterMode_Up;/* ���ϼ���(TIMx_CR1�Ĵ����ĵ�4λ)(������ʱ��ֻ����) */
    TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStruct);                 /* ��ʼ��TIM6 */

    /* NVIC���� */
    /* ��ʱ��6�ж����� */
    NVIC_InitStruct.NVIC_IRQChannel = TIM4_IRQn;            /* ѡ��TIM6�ж� */
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 6;  /* ��ռ���ȼ�6 */
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;         /* �����ȼ�0 */
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;            /* IRQͨ��ʹ�� */
    NVIC_Init(&NVIC_InitStruct);                            /* TIM6�жϳ�ʼ�� */

    /* ����TIMx�����ж� */
    TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
    /* ʹ��TIMx */
    TIM_Cmd(TIM4, ENABLE);
}



/**********************************************************
* @funcName ��TIM6_IRQHandler
* @brief    ����ʱ��6���жϷ�����
* @param    ��void
* @retval   ��void
* @details  ��
* @fn       ��
************************************************************/
void TIM4_IRQHandler(void)
{
    /* �ж��Ƿ�ʱ��6�ж� */
    if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
    {
        /* ���жϱ�־λ */
        TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
        /* �����¼� */
        FreeRTOSRunTimeTicks++;
    }
}




