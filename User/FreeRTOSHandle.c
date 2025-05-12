#include "FreeRTOSHandle.h"
extern SoftI2C_HandleTypeDef *OLED_iic;
extern SoftI2C_HandleTypeDef *MPU6050_iic;
extern SoftI2C_HandleTypeDef *AS5600_iicLeft;
extern SoftI2C_HandleTypeDef *AS5600_iicRight;
extern uint8_t IDd; // �������ڴ��ID�ŵı���
extern int16_t AX, AY, AZ, GX, GY, GZ; // �������ڴ�Ÿ������ݵı���
extern uint8_t Usart3_Receive;
extern float C_Kp_site;
extern float C_Ki_site;
extern float C_Kd_site;
/*		���ú����Ĳ�ѯ��
    vTaskSuspend(Task1_Handle);  //����Ĺ���
    vTaskResume(Task1_Handle);	 //�ָ�����
    �ж����ȼ���ֵԽСԽ���ȣ��������ȼ���ֵԽ��Խ���ȡ�
    vTaskSuspendALL();					//�������������
    vTaskResumeALL();						//�ָ����������

    vListInitialise(&TestList);         / ��ʼ���б�
    vListInitialiseItem(&ListItem1);    // ��ʼ���б���1
    vListInsert((List_t*    )&TestList,         // �б�
                (ListItem_t*)&ListItem1);     // �б���
    uxListRemove((ListItem_t*   )&ListItem2);   // �Ƴ��б���
    vListInsertEnd((List_t*     )&TestList,     // �б�
                   (ListItem_t* )&ListItem2); // �б���

    vTaskDelay();								 //�����ʱ
    xTaskDelayUntil();           //������ʱ

    xTimerStart(timer1_handle,portMAX_DELAY);   //���������ʱ��
    xTimerStop(timer1_handle,portMAX_DELAY);    //�ر������ʱ��

    pvPortMalloc(30);                 // �����ڴ�
    vPortFree(buf);                   // �ͷ��ڴ�
    xPortGetFreeHeapSize()				    //ʣ��Ŀ����ڴ��С

            ��Ҫ�����뺯����
            1����Ϣ���У��������������жϡ��жϵ��������ݽ�����һ�ֻ���(��Ϣ����)��
            2���ź��������ȼ���ת���󣩣�һ�ֽ��ͬ������Ļ��ƣ�����ʵ�ֶԹ�����Դ��������ʡ�
            3�����м���һ�ֽ��һ������ֻ����ͬһ���������͵ķ��������ݲ�ͬ�������͡�
            4���¼���־�飺��һ��λ������ʾ�¼��Ƿ�����һ���¼���־λ�ĺϼ�
            5������֪ͨ(����Ҫ�����������Դ������Ե�����ģ����Ϣ���У���ֵ�ź����������ź�����ʱ���־��)
                    ������֪ͨ����ģ�������ƿ��еĽṹ���Ա����ulNotifiedValue�������ֵ֪ͨ��
        //��������֪ͨ
        xTaskNotify();
        xTaskNotifyAndQuery();
        xTaskNotifyGive();
        xTaskNotifyFromISR();   //���ж��з�������֪ͨ--ISR
        //��������֪ͨ
        ulTaskNotifyTake();
        xTaskNotifyWait();

        //����֪ͨ��ʽ
        eNoAction = 0,              // �޲���
        eSetBits                    // ����ָ��bit
        eIncrement                  // ֵ֪ͨ��һ
        eSetValueWithOverwrite      // ��д�ķ�ʽ����ֵ֪ͨ
        eSetValueWithoutOverwrite   // ����дֵ֪ͨ

*/

///* FreeRTOS�����ʱ������ */

///* �����ʱ��1 */
//void timer1_callback(TimerHandle_t pxTimer);
//TimerHandle_t timer1_handle = 0; /* ���ζ�ʱ������ */

///* �����ʱ��2 */
//void timer2_callback(TimerHandle_t pxTimer);
//TimerHandle_t timer2_handle = 0; /* ���ڶ�ʱ������ */

///* FreeRTOS��Ϣ�������� */

//QueueHandle_t key_queue;      /* С���ݾ�� */
//QueueHandle_t big_date_queue; /* �����ݾ�� */
//QueueHandle_t queue_handle;   /* queue_handle���о�� */

///* FreeRTOS��ֵ�������������ź������� */

//QueueHandle_t semphore_handle;       /* semphore_handle��ֵ�ź������ */
//QueueHandle_t count_semphore_handle; /* count_semphore_handle�������ź������ */
QueueHandle_t mutex_semphore_handle; /* mutex_semphore_handle�����ź������ */

///* FreeRTOS���м����� */

//QueueSetHandle_t queueset_handle; /* queueset_handle���м���� */

///* FreeRTOS�¼���־������ */

//EventGroupHandle_t eventgroup_handle; /* eventgroup_handle�¼���־���� */
//#define EVENTBIT_0 (1 << 0)           /* �¼�1��־ */
//#define EVENTBIT_1 (1 << 1)           /* �¼�2��־ */

///***
// * ��������Ͷ�ʱ��������������
// * ����: �����ջ ������ƿ�
// */
//static StackType_t IdleTaskStack[configMINIMAL_STACK_SIZE];      /* �������������ջ */
//static StaticTask_t IdleTaskTCB;                                 /* ����������ƿ� */
//static StackType_t TimerTaskStack[configTIMER_TASK_STACK_DEPTH]; /* ��ʱ�����������ջ */
//static StaticTask_t TimerTaskTCB;                                /* ��ʱ������������ƿ� */

/***
 * ���񴴽���������
 * ����: �������ȼ� ��ջ��С �����ջ ������ƿ� ������ ������
 */
#define START_PRIO 1                          /* �������ȼ� */
#define START_STK_SIZE 256                     /* �����ջ��С����Ϊ��λ��1�ֵ���4�ֽ� */
StackType_t StartTaskStack[START_STK_SIZE];   /* �����ջ */
StaticTask_t StartTaskTCB;                    /* ������ƿ� */
static TaskHandle_t StartTask_Handler = NULL; /* ��������������� */
static void StartTaskCreate(void *parameter); /* ����������������� */

/***
 * TASK1��������
 * ����: �������ȼ� ��ջ��С �����ջ ������ƿ� ������ ������
 */
#define TASK1_PRIO 1                        /* �������ȼ� */
#define TASK1_STK_SIZE 128                   /* �����ջ��С����Ϊ��λ��1�ֵ���4�ֽ� */
StackType_t Task1TaskStack[TASK1_STK_SIZE]; /* �����ջ */
StaticTask_t Task1TaskTCB;                  /* ������ƿ� */
static TaskHandle_t Task1_Handle = NULL;    /* ����1�������� */
static void Task1(void *parameter);         /* ����1���������� */

/***
 * TASK2��������
 * ����: �������ȼ� ��ջ��С �����ջ ������ƿ� ������ ������
 */
#define TASK2_PRIO 3                        /* �������ȼ� */
#define TASK2_STK_SIZE 128                   /* �����ջ��С����Ϊ��λ��1�ֵ���4�ֽ� */
StackType_t Task2TaskStack[TASK2_STK_SIZE]; /* �����ջ */
StaticTask_t Task2TaskTCB;                  /* ������ƿ� */
static TaskHandle_t Task2_Handle = NULL;    /* ����2�������� */
static void Task2(void *parameter);         /* ����2���������� */

/***
 * TASK3��������
 * ����: �������ȼ� ��ջ��С �����ջ ������ƿ� ������ ������
 */
#define TASK3_PRIO 4                        /* �������ȼ� */
#define TASK3_STK_SIZE 128                   /* �����ջ��С����Ϊ��λ��1�ֵ���4�ֽ� */
StackType_t Task3TaskStack[TASK3_STK_SIZE]; /* �����ջ */
StaticTask_t Task3TaskTCB;                  /* ������ƿ� */
static TaskHandle_t Task3_Handle = NULL;    /* ����3�������� */
static void Task3(void *parameter);         /* ����3���������� */

/***
 * TASK4��������
 * ����: �������ȼ� ��ջ��С �����ջ ������ƿ� ������ ������
 */
#define TASK4_PRIO 7                        /* �������ȼ� */
#define TASK4_STK_SIZE 128                   /* �����ջ��С����Ϊ��λ��1�ֵ���4�ֽ� */
StackType_t Task4TaskStack[TASK4_STK_SIZE]; /* �����ջ */
StaticTask_t Task4TaskTCB;                  /* ������ƿ� */
static TaskHandle_t Task4_Handle = NULL;    /* ����4�������� */
static void Task4(void *parameter);         /* ����4���������� */

/***
 * TASK5�������� -- ��ȡ������Ϣ
 * ����: �������ȼ� ��ջ��С �����ջ ������ƿ� ������ ������
 */
#define TASK5_PRIO 6                        /* �������ȼ� */
#define TASK5_STK_SIZE 128                   /* �����ջ��С����Ϊ��λ��1�ֵ���4�ֽ� */
StackType_t Task5TaskStack[TASK5_STK_SIZE]; /* �����ջ */
StaticTask_t Task5TaskTCB;                  /* ������ƿ� */
static TaskHandle_t Task5_Handle = NULL;    /* ����5�������� */
static void Task5(void *parameter);         /* ����5���������� */

/***
 * TASK5��������-- ������Ϣ����
 * ����: �������ȼ� ��ջ��С �����ջ ������ƿ� ������ ������
 */
#define TASK6_PRIO 5                        /* �������ȼ� */
#define TASK6_STK_SIZE 64                   /* �����ջ��С����Ϊ��λ��1�ֵ���4�ֽ� */
StackType_t Task6TaskStack[TASK6_STK_SIZE]; /* �����ջ */
StaticTask_t Task6TaskTCB;                  /* ������ƿ� */
static TaskHandle_t Task6_Handle = NULL;    /* ����5�������� */
static void Task6(void *parameter);         /* ����5���������� */
/**************************************************************************/

//#if Static_Create_task

///**********************************************************
// * @funcName ��vApplicationGetIdleTaskMemory
// * @brief    ����ȡ��������������ջ��������ƿ��ڴ�
// * @param    ��ppxIdleTaskTCBBuffer  (������ƿ��ڴ�)
// * @param    ��ppxIdleTaskStackBuffer(�����ջ�ڴ�)
// * @param    ��pulIdleTaskStackSize  (�����ջ��С)
// * @retval   ��void
// * @details  ��
// * @fn       ��
// *               ��ȡ��������������ջ��������ƿ��ڴ棬��Ϊ������ʹ�õ�
// *           ��̬�ڴ棬��˿�������������ջ��������ƿ���ڴ��Ӧ������
// *           �����ṩ��FreeRTOS�ṩ�˽ӿں���vApplicationGetIdleTaskMemory()
// *           ʵ�ִ˺������ɡ�
// ************************************************************/
//void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
//                                   StackType_t **ppxIdleTaskStackBuffer,
//                                   uint32_t *pulIdleTaskStackSize)
//{
//    *ppxIdleTaskTCBBuffer = &IdleTaskTCB;
//    *ppxIdleTaskStackBuffer = IdleTaskStack;
//    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
//}

///**********************************************************************
// *@funcName ��vApplicationGetTimerTaskMemory
// *@brief    ����ȡ��ʱ����������������ջ��������ƿ��ڴ�
// *@param    ��
// *            ppxTimerTaskTCBBuffer    ������ƿ��ڴ�
// *            ppxTimerTaskStackBuffer  �����ջ�ڴ�
// *            pulTimerTaskStackSize    �����ջ��С
// *@retval   ��void
// *@fn       ��
// *               ��ȡ��ʱ����������������ջ��������ƿ��ڴ棬��Ϊ������ʹ�õ�
// *           ��̬�ڴ棬��˶�ʱ����������������ջ��������ƿ���ڴ��Ӧ������
// *           �����ṩ��FreeRTOS�ṩ�˽ӿں���vApplicationGetTimerTaskMemory()
// *           ʵ�ִ˺������ɡ�
// ************************************************************************/
//void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
//                                    StackType_t **ppxTimerTaskStackBuffer,
//                                    uint32_t *pulTimerTaskStackSize)
//{
//    *ppxTimerTaskTCBBuffer = &TimerTaskTCB;
//    *ppxTimerTaskStackBuffer = TimerTaskStack;
//    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
//}

//#endif

//#if IsLaunchCreateMessage
//static void CreateMessage(void)
//{

//#if IsLaunchSoft_timer

//    /* �����ʱ��1 --- ���ζ�ʱ�� */
//    timer1_handle = xTimerCreate("timer1",
//                                 500,
//                                 pdFALSE,
//                                 (void *)1,
//                                 timer1_callback);

//    /* �����ʱ��2 --- ���ڶ�ʱ�� */
//    timer2_handle = xTimerCreate("timer2",
//                                 2000,
//                                 pdTRUE,
//                                 (void *)2,
//                                 timer2_callback);
//#endif

//#if IsLaunchQueueCreate
//    /* key_queue���еĴ��� */
//    key_queue = xQueueCreate(2, sizeof(uint8_t)); /* ���г���Ϊ2����Ա��СΪsizeof(uint8_t) */
//    if (key_queue != NULL)
//        printf("key_queue���д����ɹ�����\r\n");
//    else
//        printf("key_queue���д���ʧ�ܣ���\r\n");

//    /* big_date_queue���еĴ��� */
//    big_date_queue = xQueueCreate(1, sizeof(char *)); /* ���г���Ϊ1����Ա��СΪsizeof(char*) */
//    if (big_date_queue != NULL)
//        printf("big_date_queue���д����ɹ�����\r\n");
//    else
//        printf("big_date_queue���д���ʧ�ܣ���\r\n");
//#endif

//#if IsLaunchSemaphoreCreateBinary
//    /* ������ֵ�ź��� */
//    semphore_handle = xSemaphoreCreateBinary();
//    if (semphore_handle != NULL)
//        printf("��ֵ�ź��������ɹ�������\r\n");
//    else
//        printf("��ֵ�ź�������ʧ�ܣ�����\r\n");
//#endif

//#if IsLaunchSemaphoreCreateCounting
//    /* �����������ź��� */
//    count_semphore_handle = xSemaphoreCreateCounting(100, 0); /* �������ֵΪ100����ʼֵΪ0 */
//    if (count_semphore_handle != NULL)
//        printf("�������ź��������ɹ�������\r\n");
//    else
//        printf("�������ź�������ʧ�ܣ�����\r\n");
//#endif

//#if IsLaunchSemaphoreCreateMutex
//    /* ���������ź��� */
//    mutex_semphore_handle = xSemaphoreCreateMutex(); /* ���������ź��������������ͷ�һ���ź��� */
//    if (mutex_semphore_handle != NULL)
//        printf("�����ź��������ɹ�������\r\n");
//    else
//        printf("�����ź�������ʧ�ܣ�����\r\n");
//#endif

//#if IsLaunchQueueCreateSet
//    /* �������м� */
//    queueset_handle = xQueueCreateSet(2); /* �������м������Դ��2������ */
//    if (queueset_handle != NULL)
//        printf("���м������ɹ�����\r\n");
//    else
//        printf("���м�����ʧ�ܣ���\r\n");

//    /* ����ֵ�ź����Ͷ�����ӵ����м� */
//    xQueueAddToSet(queue_handle, queueset_handle);
//    xQueueAddToSet(semphore_handle, queueset_handle);
//#endif

//#if IsLaunchEventGroupCreate
//    /* �����¼���־�� */
//    eventgroup_handle = xEventGroupCreate();
//    if (eventgroup_handle != NULL)
//        printf("�¼���־�鴴���ɹ�����\r\n");
//    else
//        printf("�¼���־�鴴��ʧ�ܣ���\r\n");
//#endif
//}
//#endif

void freertos_head(void)
{
#if Dynamic_Create_task
#if Dynamic_Create_task_define
		printf("������̬���񴴽���ʽ��\n");
    BaseType_t xReturn = pdFALSE; /* ����һ��������Ϣ����ֵ pdPASS���ɹ� */

    /* ��̬����������������� */
    xReturn = xTaskCreate(
        (TaskFunction_t)StartTaskCreate,     /* ������ */
        (const char *)"StartTaskCreate",     /* �������� */
        (uint32_t)START_STK_SIZE,            /* �����ջ��С */
        (void *)NULL,                        /* ���ݸ��������Ĳ��� */
        (UBaseType_t)START_PRIO,             /* �������ȼ� */
        (TaskHandle_t *)&StartTask_Handler); /* ������ */

    if (xReturn == pdPASS)
        printf("StartTaskCreate���񴴽��ɹ�! ʣ����ڴ�: %lu bytes\r\n", xPortGetFreeHeapSize());
    else
        printf("StartTaskCreate���񴴽�ʧ��!\r\n");

#endif
#endif

#if Static_Create_task

    /* ��̬����������������� */
    StartTask_Handler = xTaskCreateStatic(
        (TaskFunction_t)StartTaskCreate, /* ������ */
        (const char *)"StartTaskCreate", /* �������� */
        (uint32_t)START_STK_SIZE,        /* �����ջ��С */
        (void *)NULL,                    /* ���ݸ��������Ĳ��� */
        (UBaseType_t)START_PRIO,         /* �������ȼ� */
        (StackType_t *)StartTaskStack,   /* �����ջ */
        (StaticTask_t *)&StartTaskTCB);  /* ������ƿ� */

    if (StartTask_Handler != NULL)
        printf("StartTaskCreate���񴴽��ɹ�!\r\n");
    else
        printf("StartTaskCreate���񴴽�ʧ��!\r\n");

#endif

    vTaskStartScheduler(); // �������������
		
		// ��ִ�е����˵������������ʧ�ܣ�
    printf("����������ʧ�ܣ�\n");
    while (1);
}


/**********************************************************
 * @funcName ��StartTaskCreate
 * @brief    �����ڴ������������
 * @param    ��void* parameter(�������,δ�õ�)
 * @retval   ��void
 * @details  ��
 * @fn       ��
 *            ������񴴽�������ר�����������������ģ���
 *        �ǻ���������񴴽����������������񴴽���ɺ���
 *        �ǻ����ǻ�Ѹ��������١�
 ************************************************************/
static void StartTaskCreate(void *pvParameters)
{
	  BaseType_t xReturn = pdPASS; /* ����һ��������Ϣ����ֵ pdPASS���ɹ� */
    taskENTER_CRITICAL(); /* �����ٽ�������������������Ǳ��뱣֤���ٽ��� */
	
#if IsLaunchCreateMessage
    CreateMessage();
#endif
	
		mutex_semphore_handle = xSemaphoreCreateMutex(); /* ���������ź��������������ͷ�һ���ź��� */
    if (mutex_semphore_handle != NULL)
        printf("�����ź��������ɹ�������\r\n");
    else
        printf("�����ź�������ʧ�ܣ�����\r\n");

#if Dynamic_Create_task
#if Dynamic_Create_task_define

		printf("���봴����������\n");
    /* ��̬����Task1���� */
    xReturn = xTaskCreate(
        (TaskFunction_t)Task1,          /* ������ */
        (const char *)"Task1",          /* �������� */
        (uint32_t)TASK1_STK_SIZE,       /* �����ջ��С */
        (void *)NULL,                   /* ���ݸ��������Ĳ��� */
        (UBaseType_t)TASK1_PRIO,        /* �������ȼ� */
        (TaskHandle_t *)&Task1_Handle); /* ������ */

    if (xReturn == pdPASS)
        printf("Task1���񴴽��ɹ�! ʣ����ڴ�: %lu bytes\r\n", xPortGetFreeHeapSize());
    else
        printf("Task1���񴴽�ʧ��!\r\n");

    /* ��̬����Task2���� */
    xReturn = xTaskCreate(
        (TaskFunction_t)Task2,          /* ������ */
        (const char *)"Task2",          /* �������� */
        (uint32_t)TASK2_STK_SIZE,       /* �����ջ��С */
        (void *)NULL,                   /* ���ݸ��������Ĳ��� */
        (UBaseType_t)TASK2_PRIO,        /* �������ȼ� */
        (TaskHandle_t *)&Task2_Handle); /* ������ */

    if (xReturn == pdPASS)
        printf("Task2���񴴽��ɹ�! ʣ����ڴ�: %lu bytes\r\n", xPortGetFreeHeapSize());
    else
        printf("Task2���񴴽�ʧ��!\r\n");

//    /* ��̬����Task3���� */
//    xReturn = xTaskCreate(
//        (TaskFunction_t)Task3,          /* ������ */
//        (const char *)"Task3",          /* �������� */
//        (uint32_t)TASK3_STK_SIZE,       /* �����ջ��С */
//        (void *)NULL,                   /* ���ݸ��������Ĳ��� */
//        (UBaseType_t)TASK3_PRIO,        /* �������ȼ� */
//        (TaskHandle_t *)&Task3_Handle); /* ������ */

//    if (xReturn == pdPASS)
//        printf("Task3���񴴽��ɹ�! ʣ����ڴ�: %lu bytes\r\n", xPortGetFreeHeapSize());
//    else
//        printf("Task3���񴴽�ʧ��!\r\n");

//    /* ��̬����Task4���� */
//    xReturn = xTaskCreate(
//        (TaskFunction_t)Task4,          /* ������ */
//        (const char *)"Task4",          /* �������� */
//        (uint32_t)TASK4_STK_SIZE,       /* �����ջ��С */
//        (void *)NULL,                   /* ���ݸ��������Ĳ��� */
//        (UBaseType_t)TASK4_PRIO,        /* �������ȼ� */
//        (TaskHandle_t *)&Task4_Handle); /* ������ */

//    if (xReturn == pdPASS)
//        printf("Task4���񴴽��ɹ�! ʣ����ڴ�: %lu bytes\r\n", xPortGetFreeHeapSize());
//    else
//        printf("Task4���񴴽�ʧ��!\r\n");

//    /* ��̬����Task5���� */
//    xReturn = xTaskCreate(
//        (TaskFunction_t)Task5,          /* ������ */
//        (const char *)"Task5",          /* �������� */
//        (uint32_t)TASK5_STK_SIZE,       /* �����ջ��С */
//        (void *)NULL,                   /* ���ݸ��������Ĳ��� */
//        (UBaseType_t)TASK5_PRIO,        /* �������ȼ� */
//        (TaskHandle_t *)&Task5_Handle); /* ������ */

//    if (xReturn == pdPASS)
//        printf("Task5���񴴽��ɹ�! ʣ����ڴ�: %lu bytes\r\n", xPortGetFreeHeapSize());
//    else
//        printf("Task5���񴴽�ʧ��!\r\n");

//    /* ��̬����Task6���� */
//    xReturn = xTaskCreate(
//        (TaskFunction_t)Task6,          /* ������ */
//        (const char *)"Task6",          /* �������� */
//        (uint32_t)TASK6_STK_SIZE,       /* �����ջ��С */
//        (void *)NULL,                   /* ���ݸ��������Ĳ��� */
//        (UBaseType_t)TASK6_PRIO,        /* �������ȼ� */
//        (TaskHandle_t *)&Task6_Handle); /* ������ */

//    if (xReturn == pdPASS)
//        printf("Task6���񴴽��ɹ�!\r\n");
//    else
//        printf("Task6���񴴽�ʧ��!\r\n");
#endif
#endif

#if Static_Create_task
    /* ��̬����Task1���� */
    Task1_Handle = xTaskCreateStatic(
        (TaskFunction_t)Task1,          /* ������ */
        (const char *)"Task1",          /* �������� */
        (uint32_t)TASK1_STK_SIZE,       /* �����ջ��С */
        (void *)NULL,                   /* ���ݸ��������Ĳ��� */
        (UBaseType_t)TASK1_PRIO,        /* �������ȼ� */
        (StackType_t *)Task1TaskStack,  /* �����ջ */
        (StaticTask_t *)&Task1TaskTCB); /* ������ƿ� */

    if (Task1_Handle != NULL)
        printf("Task1���񴴽��ɹ�!\r\n");
    else
        printf("Task1���񴴽�ʧ��!\r\n");

    /* ��̬����Task2���� */
    Task2_Handle = xTaskCreateStatic(
        (TaskFunction_t)Task2,          /* ������ */
        (const char *)"Task2",          /* �������� */
        (uint32_t)TASK2_STK_SIZE,       /* �����ջ��С */
        (void *)NULL,                   /* ���ݸ��������Ĳ��� */
        (UBaseType_t)TASK2_PRIO,        /* �������ȼ� */
        (StackType_t *)Task2TaskStack,  /* �����ջ */
        (StaticTask_t *)&Task2TaskTCB); /* ������ƿ� */

    if (Task2_Handle != NULL)
        printf("Task2���񴴽��ɹ�!\r\n");
    else
        printf("Task2���񴴽�ʧ��!\r\n");

    /* ��̬����Task3���� */
    Task3_Handle = xTaskCreateStatic(
        (TaskFunction_t)Task3,          /* ������ */
        (const char *)"Task3",          /* �������� */
        (uint32_t)TASK3_STK_SIZE,       /* �����ջ��С */
        (void *)NULL,                   /* ���ݸ��������Ĳ��� */
        (UBaseType_t)TASK3_PRIO,        /* �������ȼ� */
        (StackType_t *)Task3TaskStack,  /* �����ջ */
        (StaticTask_t *)&Task3TaskTCB); /* ������ƿ� */

    if (Task3_Handle != NULL)
        printf("Task3���񴴽��ɹ�!\r\n");
    else
        printf("Task3���񴴽�ʧ��!\r\n");

#endif

		vTaskDelete(NULL);   /* ɾ����ʼ���� */
    taskEXIT_CRITICAL(); /* �˳��ٽ��� */

}

static void Task1(void *parameter)
{

    while (1)
    {
			if(xSemaphoreTake(mutex_semphore_handle, portMAX_DELAY) == pdTRUE)
			{
				//MPU6050_GetData(MPU6050_iic, &AX, &AY, &AZ, &GX, &GY, &GZ);
        //OLED_ShowStr(OLED_iic, 0, 3, (unsigned char *)"MPU6050X:", OLED_8x16); // ����6*8�ַ�
        //OLED_ShowStr(OLED_iic, 0, 5, (unsigned char *)"MPU6050Y:", OLED_8x16);
//				OLED_ShowDynamic_FNum(OLED_iic, 72, 3, IDd, OLED_8x16);
//				OLED_ShowDynamic_FNum(OLED_iic, 72, 1, AX, OLED_8x16);
//				OLED_ShowDynamic_FNum(OLED_iic, 72, 3, AY, OLED_8x16);
//				OLED_ShowDynamic_FNum(OLED_iic, 72, 5, AZ, OLED_8x16);
//				OLED_ShowDynamic_FNum(OLED_iic, 0, 1, GX, OLED_8x16);
//				OLED_ShowDynamic_FNum(OLED_iic, 0, 3, GY, OLED_8x16);
//				OLED_ShowDynamic_FNum(OLED_iic, 0, 5, GZ, OLED_8x16);
			  OLED_ShowString_C8T6(1, 1, "I:");
			  OLED_ShowHexNum_C8T6(1, 3, IDd, 2);
				OLED_ShowString_C8T6(1, 6, "L:");
				OLED_ShowNum_C8T6(1, 8, Get_Angle_Degree(AS5600_iicLeft), 3);
				OLED_ShowString_C8T6(1, 12, "R:");
				OLED_ShowNum_C8T6(1, 14, Get_Angle_Degree(AS5600_iicRight), 3);
//				OLED_ShowSignedNum_C8T6(2, 1, AX, 5);					//OLED��ʾ����
//				OLED_ShowSignedNum_C8T6(3, 1, AY, 5);
//				OLED_ShowSignedNum_C8T6(4, 1, AZ, 5);
//				OLED_ShowSignedNum_C8T6(2, 8, GX, 5);
//				OLED_ShowSignedNum_C8T6(3, 8, GY, 5);
//				OLED_ShowSignedNum_C8T6(4, 8, GZ, 5);
				//OLED_ShowNum_C8T6(2, 8,(uint16_t)(C_Kp_site*10000), 4);
				OLED_ShowString_C8T6(2, 1, "P:");
				OLED_ShowFloat_C8T6(2,4,C_Kp_site,3);
				OLED_ShowString_C8T6(3, 1, "I:");
				OLED_ShowFloat_C8T6(3,4,C_Ki_site,3);
				OLED_ShowString_C8T6(4, 1, "D:");
				OLED_ShowFloat_C8T6(4,4,C_Kd_site,3);
				
				printf("task1����\n");
				// �ͷŻ�����
        xSemaphoreGive(mutex_semphore_handle);
			}
        vTaskDelay(100);
    }
}
static void Task2(void *parameter)
{
		float i = 0.0;
    while (1)
    {
			if(xSemaphoreTake(mutex_semphore_handle, portMAX_DELAY) == pdTRUE)
			{
				//SetSpeed(AS5600_iicLeft,5.0);
				SetAngle(AS5600_iicLeft,60.0);
				printf("%d",Get_Angle_Degree(AS5600_iicRight));
				printf("task2����\n");
				
				// �ͷŻ�����
        xSemaphoreGive(mutex_semphore_handle);
			}
        vTaskDelay(10);
    }
}

///* ʱ��Ƭ�ֻأ�һ��ʱ��Ƭ��С��ȡ��Ϊ�δ�ʱ���ж�Ƶ�ʡ�*/
//static void Task3(void *parameter)
//{
//    uint32_t task3_num = 0;
//    while (1)
//    {
//        taskENTER_CRITICAL(); /* �����ٽ��� */
//        printf("task3���д�����%d\r\n", ++task3_num);
//        taskEXIT_CRITICAL(); /* �˳��ٽ��� */
//        vTaskDelay(100);     /* ��ʱ100��tick */
//    }
//}

//static void Task4(void *parameter)
//{
//    uint32_t task4_num = 0;
//    while (1)
//    {
//        taskENTER_CRITICAL(); /* �����ٽ��� */
//        printf("task4���д�����%d\r\n", ++task4_num);
//        taskEXIT_CRITICAL(); /* �˳��ٽ��� */
//			  delay_ms(1000);
//        vTaskDelay(100);     /* ��ʱ100��tick */
//    }
//}

//char task_buff[500]; /* �嵥��Ϣ������ */
//char task_buff_time[500];
///**********************************************************
// * @funcName ��Task5
// * @brief    ������5
// * @param    ��void* parameter(�������,δ�õ�)
// * @retval   ��void
// * @details  ��
// * @fn       ��ʹ�ò�ѯFreeRTOS����״̬����
// ************************************************************/
//static void Task5(void *parameter)
//{
//    UBaseType_t priority_num = 0;    /* �������ȼ�ֵ */
//    UBaseType_t task_num = 0;        /* �������� */
//    UBaseType_t task_num2 = 0;       /* �������� */
//    TaskStatus_t *status_array = 0;  /* ��������״̬��Ϣ */
//    TaskStatus_t *status_array2 = 0; /* ָ������������״̬��Ϣ */
//    TaskHandle_t task_handle = 0;    /* ĳ������������� */
//    UBaseType_t task_stack_min = 0;  /* ʣ���ջ��С */
//    int state = 0;                   /* ָ�������״̬��Ϣ */

//    uint8_t i = 0;

//    /* �޸�ָ����������ȼ� */
//    vTaskPrioritySet(Task3_Handle, 6);

//    /* ��ȡָ���������ȼ�ֵ��NULL��ʾ��ȡ����� */
//    priority_num = uxTaskPriorityGet(NULL);
//    printf("task1�������ȼ�Ϊ%ld\r\n", priority_num);

//    /* ��ȡ��ǰϵͳ���������� */
//    task_num = uxTaskGetNumberOfTasks();
//    printf("����������%ld\r\n", task_num);

//    /* ��ȡ���������״̬��Ϣ */
//    status_array = mymalloc(0, (sizeof(TaskStatus_t) * task_num)); /* ��SRAM���ڴ����������ռ� */
//    task_num2 = uxTaskGetSystemState(status_array, task_num, NULL);
//    printf("������\t\t�������ȼ�\t������\r\n");
//    for (i = 0; i < task_num2; i++)
//    {
//        printf("%s\t\t%ld\t%ld\r\n",
//               status_array[i].pcTaskName,
//               status_array[i].uxCurrentPriority,
//               status_array[i].xTaskNumber);
//    }

//    /* ��ȡָ�������������Ϣv*/
//    status_array2 = mymalloc(SRAMIN, sizeof(TaskStatus_t));
//    vTaskGetInfo(Task2_Handle, status_array2, pdTRUE, eInvalid);
//    printf("��������%s\r\n", status_array2->pcTaskName);
//    printf("�������ȼ���%ld\r\n", status_array2->uxCurrentPriority);
//    printf("�����ţ�%ld\r\n", status_array2->xTaskNumber);
//    printf("����״̬��%d\r\n", status_array2->eCurrentState);

//    /* ͨ����������ȡ������ */
//    task_handle = xTaskGetHandle("Task1");
//    printf("��������%#x\r\n", (int)task_handle);
//    printf("task1����������%#x\r\n", (int)Task1_Handle);

//    /* ��ȡָ�������״̬ */
//    eTaskGetState(Task2_Handle);
//    state = eTaskGetState(Task2_Handle);
//    printf("��ǰtask2������״̬Ϊ��%d\r\n", state);

//    /* ���嵥��ʽ��ȡ��������״̬��Ϣ */
//    vTaskList(task_buff);
//    printf("%s\r\n", task_buff);
//		


//    while (1)
//    {
//        /* ��ȡʣ���ջ��С */
//        task_stack_min = uxTaskGetStackHighWaterMark(Task2_Handle);
//        printf("task2��ʷʣ����С��ջΪ%ld\r\n", task_stack_min);
//			
//				/* ���嵥��ʽ��ȡ������������ͳ��ʱ��  */
//				vTaskGetRunTimeStats(task_buff_time);
//				printf("%s\r\n", task_buff_time);
//        vTaskDelay(800); /* ��ʱ800��tick */
//    }
//}

///**********************************************************
// * @funcName ��Task6
// * @brief    ����������������Ϣ���ݺ���
// * @param    ��void * parameter(�������,δ�õ�)
// * @retval   ��void
// * @details  ��
// * @fn       ��
// ************************************************************/
//char buff[50] = {"lmlm"};
//static void Task6(void *parameter)
//{
//    uint8_t i = 0;
//    char *buf;
//    BaseType_t err_Q = 0;
//    BaseType_t err_S = 0;
//    BaseType_t err_C = 0;
//    BaseType_t err_M = 0;
//    BaseType_t err_S_Q = 0;
//    BaseType_t err_S_S = 0;
//    buf = &buff[0];
//    QueueSetMemberHandle_t member_handle;
//    EventBits_t event_bit = 0;
//    uint32_t noyify_val = 0;
//    uint32_t reveice = 0;
//    uint32_t notify_val_R = 0, event_bit_R = 0;
//    while (1)
//    {
//        /*��Ϣ������Ϣ�����봴�����ͷ�*/
//        err_Q = xQueueSend(big_date_queue, &buf, portMAX_DELAY);
//        if (err_Q != pdTRUE)
//            printf("key_queue���з���ʧ��\r\n");
//        else
//            printf("key_queue���з��ͳɹ�\r\n");
//        

//        err_Q = xQueueReceive(big_date_queue, &buf, portMAX_DELAY);
//        if (err_Q != pdTRUE)
//            printf("big_date_queue���ж�ȡʧ��\r\n");
//        else
//            printf("���ݣ�%s\r\n", buf);

//        /*��ֵ�ź���Ϣ�����봴�����ͷ�*/
//        err_S = xSemaphoreTake(semphore_handle, portMAX_DELAY); /* ��ȡ�ź��������� */
//        if (err_S == pdTRUE)
//            printf("��ȡ�ź����ɹ�\r\n");
//        else
//            printf("�ѳ�ʱ%d\r\n", ++i);

//        if (semphore_handle != NULL)
//        {
//            err_S = xSemaphoreGive(semphore_handle); /* �ͷŶ�ֵ�ź��� */
//            if (err_S == pdPASS)
//                printf("�ź����ͷųɹ�����\r\n");
//            else
//                printf("�ź����ͷ�ʧ�ܣ���\r\n");
//        }

//        /*�������ź���Ϣ�����봴�����ͷ�*/
//        err_C = xSemaphoreTake(count_semphore_handle, portMAX_DELAY); /* ��ȡ�ź��������� */
//        if (err_C == pdTRUE)
//            printf("�ź����ļ���ֵΪ��%d\r\n", (int)uxSemaphoreGetCount(count_semphore_handle));
//        else
//            printf("�������ź�����ȡʧ��\r\n");

//        if (count_semphore_handle != NULL)
//        {
//            xSemaphoreGive(count_semphore_handle); /* �ͷ��ź��� */
//        }

//        /*�������ź�(���������ȼ���תӰ��)��Ϣ�����봴�����ͷ�*/
//        err_M = xSemaphoreTake(mutex_semphore_handle, portMAX_DELAY); /* ��ȡ�ź��������� */
//        if (err_M == pdTRUE)
//            printf("��ȡ�ź����ɹ�\r\n");
//        else
//            printf("��ȡ�ź���ʧ��\r\n");

//        if (mutex_semphore_handle != NULL)
//        {
//            xSemaphoreGive(mutex_semphore_handle); /* �ͷ��ź��� */
//        }

//        /*���м��ź���Ϣ�����봴�����ͷ�*/
//        /* ��ѯ���м���Ϣ */
//        member_handle = xQueueSelectFromSet(queueset_handle, portMAX_DELAY);
//        /* ʶ����м����Ƕ�ֵ�ź���������Ϣ���������� */
//        if (member_handle == queue_handle)
//        {
//            /* ��ȡ���м�����Ϣ���е����� */
//            xQueueReceive(member_handle, &i, portMAX_DELAY);
//            printf("��ȡ���Ķ�������Ϊ��%d\r\n", i);
//        }
//        else if (member_handle == semphore_handle)
//        {
//            /* ��ȡ���м��ж�ֵ�ź��������� */
//            xSemaphoreTake(member_handle, portMAX_DELAY);
//            printf("��ȡ�ź����ɹ�����\r\n");
//        }
//        /* ����м��е���Ϣ���з������� */
//        err_S_Q = xQueueSend(queue_handle, &i, portMAX_DELAY);
//        if (err_S_Q == pdPASS)
//            printf("������queue_handleд�����ݳɹ�����\r\n");
//        else
//            printf("������queue_handleд������ʧ�ܣ���\r\n");
//        /* ����м��еĶ�ֵ�ź����ͷ��ź��� */
//        err_S_S = xSemaphoreGive(semphore_handle);
//        if (err_S_S == pdPASS)
//            printf("�ͷ��ź����ɹ�����\r\n");
//        else
//            printf("�ͷ��ź���ʧ�ܣ���\r\n");

//        /*�¼���־���ź���Ϣ�����봴�����ͷ�*/
//        event_bit = xEventGroupWaitBits(eventgroup_handle,       /* �¼���־���� */
//                                        EVENTBIT_0 | EVENTBIT_1, /* �ȴ��¼���־���bit0��bit1λ */
//                                        pdTRUE,                  /* �ɹ��ȴ����¼���־λ������¼���־���е�bit0��bit1λ */
//                                        pdTRUE,                  /* �ȴ��¼���־���bit0��bit1λ����1,�ͳ��� */
//                                        portMAX_DELAY);          /* ���� */
//        printf("�ȴ������¼���־λֵΪ��%#x\r\n", event_bit);

//        xEventGroupSetBits(eventgroup_handle, EVENTBIT_0); /* ���¼���־���bit0λ��1 */
//        xEventGroupSetBits(eventgroup_handle, EVENTBIT_1); /* ���¼���־���bit1λ��1 */

//        /*����֪ͨ�ź���Ϣ�����봴�����ͷ�*/
//        // ģ����Ϣ����
//        xTaskNotifyWait(0, 0xFFFFFFFF, &noyify_val, portMAX_DELAY);
//        printf("����֪ͨģ����Ϣ���䷢�ͣ����͵ļ�ֵΪ��%d\r\n", i);
//        xTaskNotify(Task5_Handle, i, eSetValueWithOverwrite);
//        // ģ���ֵ�ź���
//        reveice = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
//        printf("��������֪ͨ�ɹ���ģ���ȡ��ֵ�ź�����\r\n");
//        printf("����֪ͨģ���ֵ�ź����ͷţ�\r\n");
//        xTaskNotifyGive(Task5_Handle);
//        // ģ������ź���
//        reveice = ulTaskNotifyTake(pdFALSE, portMAX_DELAY);
//        printf("����֪ͨģ��������ź����ͷţ�\r\n");
//        xTaskNotifyGive(Task5_Handle);
//        // ģ���¼���־��
//        xTaskNotifyWait(0, 0xFFFFFFFF, &notify_val_R, portMAX_DELAY);
//        /* �¼�1 */
//        if (notify_val_R & EVENTBIT_0)
//            event_bit_R |= EVENTBIT_0;
//        /* �¼�2 */
//        if (notify_val_R & EVENTBIT_1)
//            event_bit_R |= EVENTBIT_1;
//        /* �¼�1���¼�2������ */
//        if (event_bit_R == (EVENTBIT_0 | EVENTBIT_1))
//        {
//            printf("����֪ͨģ���¼���־����ճɹ�����\r\n");
//            event_bit_R = 0;
//        }
//        printf("��bit0λ��1\r\n");
//        xTaskNotify(Task5_Handle, EVENTBIT_0, eSetBits);

//        vTaskDelay(10); /* ��ʱ100��tick */
//    }
//}
///**********************************************************
// * @funcName ��timer1_callback
// * @brief    �������ʱ��1�ĳ�ʱ�ص�����
// * @param    ��TimerHandle_t pxTimer(�������,δ�õ�)
// * @retval   ��void
// * @details  ��
// * @fn       ��
// ************************************************************/
//void timer1_callback(TimerHandle_t pxTimer)
//{
//    static uint32_t timer = 0;
//    printf("timer1�����д�����%d\r\n", ++timer);
//}

///**********************************************************
// * @funcName ��timer2_callback
// * @brief    �������ʱ��2�ĳ�ʱ�ص�����
// * @param    ��TimerHandle_t pxTimer(�������,δ�õ�)
// * @retval   ��void
// * @details  ��
// * @fn       ��
// ************************************************************/
//void timer2_callback(TimerHandle_t pxTimer)
//{
//    static uint32_t timer = 0;
//    printf("timer2�����д�����%d\r\n", ++timer);
//}

///**********************************************************
// * @funcName ��PRE_SLEEP_PROCESSING
// * @brief    ������͹���ǰ����Ҫִ�еĲ����ص�����
// * @param    ��void
// * @retval   ��void
// * @details  ��
// * @fn       ��
// ************************************************************/
//void PRE_SLEEP_PROCESSING(void)
//{
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, DISABLE);
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, DISABLE);
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, DISABLE);
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, DISABLE);
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, DISABLE);
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, DISABLE);
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, DISABLE);
//}

///**********************************************************
// * @funcName ��POST_SLEEP_PROCESSING
// * @brief    ���˳��͹��ĺ�����Ҫִ�еĲ����ص�����
// * @param    ��void
// * @retval   ��void
// * @details  ��
// * @fn       ��
// ************************************************************/
//void POST_SLEEP_PROCESSING(void)
//{
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
//}
