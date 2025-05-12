#include "FreeRTOSHandle.h"
extern SoftI2C_HandleTypeDef *OLED_iic;
extern SoftI2C_HandleTypeDef *MPU6050_iic;
extern SoftI2C_HandleTypeDef *AS5600_iicLeft;
extern SoftI2C_HandleTypeDef *AS5600_iicRight;
extern uint8_t IDd; // 定义用于存放ID号的变量
extern int16_t AX, AY, AZ, GX, GY, GZ; // 定义用于存放各个数据的变量
extern uint8_t Usart3_Receive;
extern float C_Kp_site;
extern float C_Ki_site;
extern float C_Kd_site;
/*		常用函数的查询：
    vTaskSuspend(Task1_Handle);  //任务的挂起
    vTaskResume(Task1_Handle);	 //恢复任务
    中断优先级数值越小越优先，任务优先级数值越大越优先。
    vTaskSuspendALL();					//挂起任务调度器
    vTaskResumeALL();						//恢复任务调度器

    vListInitialise(&TestList);         / 初始化列表
    vListInitialiseItem(&ListItem1);    // 初始化列表项1
    vListInsert((List_t*    )&TestList,         // 列表
                (ListItem_t*)&ListItem1);     // 列表项
    uxListRemove((ListItem_t*   )&ListItem2);   // 移除列表项
    vListInsertEnd((List_t*     )&TestList,     // 列表
                   (ListItem_t* )&ListItem2); // 列表项

    vTaskDelay();								 //相对延时
    xTaskDelayUntil();           //绝对延时

    xTimerStart(timer1_handle,portMAX_DELAY);   //开启软件定时器
    xTimerStop(timer1_handle,portMAX_DELAY);    //关闭软件定时器

    pvPortMalloc(30);                 // 申请内存
    vPortFree(buf);                   // 释放内存
    xPortGetFreeHeapSize()				    //剩余的空闲内存大小

            重要概念与函数：
            1、消息队列：任务到任务、任务到中断、中断到任务数据交流的一种机制(消息传递)。
            2、信号量（优先级翻转现象）：一种解决同步问题的机制，可以实现对共享资源的有序访问。
            3、队列集：一种解决一个队列只允许同一种数据类型的方法，传递不同数据类型。
            4、事件标志组：用一个位，来表示事件是否发生。一组事件标志位的合集
            5、任务通知(不需要创建，任务自带，单对单，可模拟消息队列，二值信号量，计数信号量，时间标志组)
                    ：用来通知任务的，任务控制块中的结构体成员变量ulNotifiedValue就是这个通知值。
        //发送任务通知
        xTaskNotify();
        xTaskNotifyAndQuery();
        xTaskNotifyGive();
        xTaskNotifyFromISR();   //在中断中发送任务通知--ISR
        //接受任务通知
        ulTaskNotifyTake();
        xTaskNotifyWait();

        //任务通知方式
        eNoAction = 0,              // 无操作
        eSetBits                    // 更新指定bit
        eIncrement                  // 通知值加一
        eSetValueWithOverwrite      // 覆写的方式更新通知值
        eSetValueWithoutOverwrite   // 不覆写通知值

*/

///* FreeRTOS软件定时器配置 */

///* 软件定时器1 */
//void timer1_callback(TimerHandle_t pxTimer);
//TimerHandle_t timer1_handle = 0; /* 单次定时器计数 */

///* 软件定时器2 */
//void timer2_callback(TimerHandle_t pxTimer);
//TimerHandle_t timer2_handle = 0; /* 周期定时器计数 */

///* FreeRTOS消息队列配置 */

//QueueHandle_t key_queue;      /* 小数据句柄 */
//QueueHandle_t big_date_queue; /* 大数据句柄 */
//QueueHandle_t queue_handle;   /* queue_handle队列句柄 */

///* FreeRTOS二值、计数、互斥信号量配置 */

//QueueHandle_t semphore_handle;       /* semphore_handle二值信号量句柄 */
//QueueHandle_t count_semphore_handle; /* count_semphore_handle计数型信号量句柄 */
QueueHandle_t mutex_semphore_handle; /* mutex_semphore_handle互斥信号量句柄 */

///* FreeRTOS队列集配置 */

//QueueSetHandle_t queueset_handle; /* queueset_handle队列集句柄 */

///* FreeRTOS事件标志组配置 */

//EventGroupHandle_t eventgroup_handle; /* eventgroup_handle事件标志组句柄 */
//#define EVENTBIT_0 (1 << 0)           /* 事件1标志 */
//#define EVENTBIT_1 (1 << 1)           /* 事件2标志 */

///***
// * 空闲任务和定时器服务任务配置
// * 包括: 任务堆栈 任务控制块
// */
//static StackType_t IdleTaskStack[configMINIMAL_STACK_SIZE];      /* 空闲任务任务堆栈 */
//static StaticTask_t IdleTaskTCB;                                 /* 空闲任务控制块 */
//static StackType_t TimerTaskStack[configTIMER_TASK_STACK_DEPTH]; /* 定时器服务任务堆栈 */
//static StaticTask_t TimerTaskTCB;                                /* 定时器服务任务控制块 */

/***
 * 任务创建任务配置
 * 包括: 任务优先级 堆栈大小 任务堆栈 任务控制块 任务句柄 任务函数
 */
#define START_PRIO 1                          /* 任务优先级 */
#define START_STK_SIZE 256                     /* 任务堆栈大小，字为单位，1字等于4字节 */
StackType_t StartTaskStack[START_STK_SIZE];   /* 任务堆栈 */
StaticTask_t StartTaskTCB;                    /* 任务控制块 */
static TaskHandle_t StartTask_Handler = NULL; /* 创建任务的任务句柄 */
static void StartTaskCreate(void *parameter); /* 创建任务的任务函数名 */

/***
 * TASK1任务配置
 * 包括: 任务优先级 堆栈大小 任务堆栈 任务控制块 任务句柄 任务函数
 */
#define TASK1_PRIO 1                        /* 任务优先级 */
#define TASK1_STK_SIZE 128                   /* 任务堆栈大小，字为单位，1字等于4字节 */
StackType_t Task1TaskStack[TASK1_STK_SIZE]; /* 任务堆栈 */
StaticTask_t Task1TaskTCB;                  /* 任务控制块 */
static TaskHandle_t Task1_Handle = NULL;    /* 任务1的任务句柄 */
static void Task1(void *parameter);         /* 任务1的任务函数名 */

/***
 * TASK2任务配置
 * 包括: 任务优先级 堆栈大小 任务堆栈 任务控制块 任务句柄 任务函数
 */
#define TASK2_PRIO 3                        /* 任务优先级 */
#define TASK2_STK_SIZE 128                   /* 任务堆栈大小，字为单位，1字等于4字节 */
StackType_t Task2TaskStack[TASK2_STK_SIZE]; /* 任务堆栈 */
StaticTask_t Task2TaskTCB;                  /* 任务控制块 */
static TaskHandle_t Task2_Handle = NULL;    /* 任务2的任务句柄 */
static void Task2(void *parameter);         /* 任务2的任务函数名 */

/***
 * TASK3任务配置
 * 包括: 任务优先级 堆栈大小 任务堆栈 任务控制块 任务句柄 任务函数
 */
#define TASK3_PRIO 4                        /* 任务优先级 */
#define TASK3_STK_SIZE 128                   /* 任务堆栈大小，字为单位，1字等于4字节 */
StackType_t Task3TaskStack[TASK3_STK_SIZE]; /* 任务堆栈 */
StaticTask_t Task3TaskTCB;                  /* 任务控制块 */
static TaskHandle_t Task3_Handle = NULL;    /* 任务3的任务句柄 */
static void Task3(void *parameter);         /* 任务3的任务函数名 */

/***
 * TASK4任务配置
 * 包括: 任务优先级 堆栈大小 任务堆栈 任务控制块 任务句柄 任务函数
 */
#define TASK4_PRIO 7                        /* 任务优先级 */
#define TASK4_STK_SIZE 128                   /* 任务堆栈大小，字为单位，1字等于4字节 */
StackType_t Task4TaskStack[TASK4_STK_SIZE]; /* 任务堆栈 */
StaticTask_t Task4TaskTCB;                  /* 任务控制块 */
static TaskHandle_t Task4_Handle = NULL;    /* 任务4的任务句柄 */
static void Task4(void *parameter);         /* 任务4的任务函数名 */

/***
 * TASK5任务配置 -- 获取任务信息
 * 包括: 任务优先级 堆栈大小 任务堆栈 任务控制块 任务句柄 任务函数
 */
#define TASK5_PRIO 6                        /* 任务优先级 */
#define TASK5_STK_SIZE 128                   /* 任务堆栈大小，字为单位，1字等于4字节 */
StackType_t Task5TaskStack[TASK5_STK_SIZE]; /* 任务堆栈 */
StaticTask_t Task5TaskTCB;                  /* 任务控制块 */
static TaskHandle_t Task5_Handle = NULL;    /* 任务5的任务句柄 */
static void Task5(void *parameter);         /* 任务5的任务函数名 */

/***
 * TASK5任务配置-- 配置信息创建
 * 包括: 任务优先级 堆栈大小 任务堆栈 任务控制块 任务句柄 任务函数
 */
#define TASK6_PRIO 5                        /* 任务优先级 */
#define TASK6_STK_SIZE 64                   /* 任务堆栈大小，字为单位，1字等于4字节 */
StackType_t Task6TaskStack[TASK6_STK_SIZE]; /* 任务堆栈 */
StaticTask_t Task6TaskTCB;                  /* 任务控制块 */
static TaskHandle_t Task6_Handle = NULL;    /* 任务5的任务句柄 */
static void Task6(void *parameter);         /* 任务5的任务函数名 */
/**************************************************************************/

//#if Static_Create_task

///**********************************************************
// * @funcName ：vApplicationGetIdleTaskMemory
// * @brief    ：获取空闲任务地任务堆栈和任务控制块内存
// * @param    ：ppxIdleTaskTCBBuffer  (任务控制块内存)
// * @param    ：ppxIdleTaskStackBuffer(任务堆栈内存)
// * @param    ：pulIdleTaskStackSize  (任务堆栈大小)
// * @retval   ：void
// * @details  ：
// * @fn       ：
// *               获取空闲任务地任务堆栈和任务控制块内存，因为本例程使用的
// *           静态内存，因此空闲任务的任务堆栈和任务控制块的内存就应该有用
// *           户来提供，FreeRTOS提供了接口函数vApplicationGetIdleTaskMemory()
// *           实现此函数即可。
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
// *@funcName ：vApplicationGetTimerTaskMemory
// *@brief    ：获取定时器服务任务的任务堆栈和任务控制块内存
// *@param    ：
// *            ppxTimerTaskTCBBuffer    任务控制块内存
// *            ppxTimerTaskStackBuffer  任务堆栈内存
// *            pulTimerTaskStackSize    任务堆栈大小
// *@retval   ：void
// *@fn       ：
// *               获取定时器服务任务的任务堆栈和任务控制块内存，因为本例程使用的
// *           静态内存，因此定时器服务任务的任务堆栈和任务控制块的内存就应该有用
// *           户来提供，FreeRTOS提供了接口函数vApplicationGetTimerTaskMemory()
// *           实现此函数即可。
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

//    /* 软件定时器1 --- 单次定时器 */
//    timer1_handle = xTimerCreate("timer1",
//                                 500,
//                                 pdFALSE,
//                                 (void *)1,
//                                 timer1_callback);

//    /* 软件定时器2 --- 周期定时器 */
//    timer2_handle = xTimerCreate("timer2",
//                                 2000,
//                                 pdTRUE,
//                                 (void *)2,
//                                 timer2_callback);
//#endif

//#if IsLaunchQueueCreate
//    /* key_queue队列的创建 */
//    key_queue = xQueueCreate(2, sizeof(uint8_t)); /* 队列长度为2，成员大小为sizeof(uint8_t) */
//    if (key_queue != NULL)
//        printf("key_queue队列创建成功！！\r\n");
//    else
//        printf("key_queue队列创建失败！！\r\n");

//    /* big_date_queue队列的创建 */
//    big_date_queue = xQueueCreate(1, sizeof(char *)); /* 队列长度为1，成员大小为sizeof(char*) */
//    if (big_date_queue != NULL)
//        printf("big_date_queue队列创建成功！！\r\n");
//    else
//        printf("big_date_queue队列创建失败！！\r\n");
//#endif

//#if IsLaunchSemaphoreCreateBinary
//    /* 创建二值信号量 */
//    semphore_handle = xSemaphoreCreateBinary();
//    if (semphore_handle != NULL)
//        printf("二值信号量创建成功！！！\r\n");
//    else
//        printf("二值信号量创建失败！！！\r\n");
//#endif

//#if IsLaunchSemaphoreCreateCounting
//    /* 创建计数型信号量 */
//    count_semphore_handle = xSemaphoreCreateCounting(100, 0); /* 计数最大值为100，初始值为0 */
//    if (count_semphore_handle != NULL)
//        printf("计数型信号量创建成功！！！\r\n");
//    else
//        printf("计数型信号量创建失败！！！\r\n");
//#endif

//#if IsLaunchSemaphoreCreateMutex
//    /* 创建互斥信号量 */
//    mutex_semphore_handle = xSemaphoreCreateMutex(); /* 创建互斥信号量，并且主动释放一次信号量 */
//    if (mutex_semphore_handle != NULL)
//        printf("互斥信号量创建成功！！！\r\n");
//    else
//        printf("互斥信号量创建失败！！！\r\n");
//#endif

//#if IsLaunchQueueCreateSet
//    /* 创建队列集 */
//    queueset_handle = xQueueCreateSet(2); /* 创建队列集，可以存放2个队列 */
//    if (queueset_handle != NULL)
//        printf("队列集创建成功！！\r\n");
//    else
//        printf("队列集创建失败！！\r\n");

//    /* 将二值信号量和队列添加到队列集 */
//    xQueueAddToSet(queue_handle, queueset_handle);
//    xQueueAddToSet(semphore_handle, queueset_handle);
//#endif

//#if IsLaunchEventGroupCreate
//    /* 创建事件标志组 */
//    eventgroup_handle = xEventGroupCreate();
//    if (eventgroup_handle != NULL)
//        printf("事件标志组创建成功！！\r\n");
//    else
//        printf("事件标志组创建失败！！\r\n");
//#endif
//}
//#endif

void freertos_head(void)
{
#if Dynamic_Create_task
#if Dynamic_Create_task_define
		printf("开启动态任务创建方式。\n");
    BaseType_t xReturn = pdFALSE; /* 定义一个创建信息返回值 pdPASS：成功 */

    /* 动态创建创建任务的任务 */
    xReturn = xTaskCreate(
        (TaskFunction_t)StartTaskCreate,     /* 任务函数 */
        (const char *)"StartTaskCreate",     /* 任务名称 */
        (uint32_t)START_STK_SIZE,            /* 任务堆栈大小 */
        (void *)NULL,                        /* 传递给任务函数的参数 */
        (UBaseType_t)START_PRIO,             /* 任务优先级 */
        (TaskHandle_t *)&StartTask_Handler); /* 任务句柄 */

    if (xReturn == pdPASS)
        printf("StartTaskCreate任务创建成功! 剩余堆内存: %lu bytes\r\n", xPortGetFreeHeapSize());
    else
        printf("StartTaskCreate任务创建失败!\r\n");

#endif
#endif

#if Static_Create_task

    /* 静态创建创建任务的任务 */
    StartTask_Handler = xTaskCreateStatic(
        (TaskFunction_t)StartTaskCreate, /* 任务函数 */
        (const char *)"StartTaskCreate", /* 任务名称 */
        (uint32_t)START_STK_SIZE,        /* 任务堆栈大小 */
        (void *)NULL,                    /* 传递给任务函数的参数 */
        (UBaseType_t)START_PRIO,         /* 任务优先级 */
        (StackType_t *)StartTaskStack,   /* 任务堆栈 */
        (StaticTask_t *)&StartTaskTCB);  /* 任务控制块 */

    if (StartTask_Handler != NULL)
        printf("StartTaskCreate任务创建成功!\r\n");
    else
        printf("StartTaskCreate任务创建失败!\r\n");

#endif

    vTaskStartScheduler(); // 启动任务调度器
		
		// 若执行到这里，说明调度器启动失败！
    printf("调度器启动失败！\n");
    while (1);
}


/**********************************************************
 * @funcName ：StartTaskCreate
 * @brief    ：用于创建任务的任务
 * @param    ：void* parameter(传入参数,未用到)
 * @retval   ：void
 * @details  ：
 * @fn       ：
 *            这个任务创建函数是专门用来创建任务函数的，我
 *        们会把它当任务创建，当它把其他任务创建完成后，我
 *        们会我们会把该任务销毁。
 ************************************************************/
static void StartTaskCreate(void *pvParameters)
{
	  BaseType_t xReturn = pdPASS; /* 定义一个创建信息返回值 pdPASS：成功 */
    taskENTER_CRITICAL(); /* 进入临界区，创建任务过程我们必须保证在临界区 */
	
#if IsLaunchCreateMessage
    CreateMessage();
#endif
	
		mutex_semphore_handle = xSemaphoreCreateMutex(); /* 创建互斥信号量，并且主动释放一次信号量 */
    if (mutex_semphore_handle != NULL)
        printf("互斥信号量创建成功！！！\r\n");
    else
        printf("互斥信号量创建失败！！！\r\n");

#if Dynamic_Create_task
#if Dynamic_Create_task_define

		printf("进入创建任务函数。\n");
    /* 动态创建Task1任务 */
    xReturn = xTaskCreate(
        (TaskFunction_t)Task1,          /* 任务函数 */
        (const char *)"Task1",          /* 任务名称 */
        (uint32_t)TASK1_STK_SIZE,       /* 任务堆栈大小 */
        (void *)NULL,                   /* 传递给任务函数的参数 */
        (UBaseType_t)TASK1_PRIO,        /* 任务优先级 */
        (TaskHandle_t *)&Task1_Handle); /* 任务句柄 */

    if (xReturn == pdPASS)
        printf("Task1任务创建成功! 剩余堆内存: %lu bytes\r\n", xPortGetFreeHeapSize());
    else
        printf("Task1任务创建失败!\r\n");

    /* 动态创建Task2任务 */
    xReturn = xTaskCreate(
        (TaskFunction_t)Task2,          /* 任务函数 */
        (const char *)"Task2",          /* 任务名称 */
        (uint32_t)TASK2_STK_SIZE,       /* 任务堆栈大小 */
        (void *)NULL,                   /* 传递给任务函数的参数 */
        (UBaseType_t)TASK2_PRIO,        /* 任务优先级 */
        (TaskHandle_t *)&Task2_Handle); /* 任务句柄 */

    if (xReturn == pdPASS)
        printf("Task2任务创建成功! 剩余堆内存: %lu bytes\r\n", xPortGetFreeHeapSize());
    else
        printf("Task2任务创建失败!\r\n");

//    /* 动态创建Task3任务 */
//    xReturn = xTaskCreate(
//        (TaskFunction_t)Task3,          /* 任务函数 */
//        (const char *)"Task3",          /* 任务名称 */
//        (uint32_t)TASK3_STK_SIZE,       /* 任务堆栈大小 */
//        (void *)NULL,                   /* 传递给任务函数的参数 */
//        (UBaseType_t)TASK3_PRIO,        /* 任务优先级 */
//        (TaskHandle_t *)&Task3_Handle); /* 任务句柄 */

//    if (xReturn == pdPASS)
//        printf("Task3任务创建成功! 剩余堆内存: %lu bytes\r\n", xPortGetFreeHeapSize());
//    else
//        printf("Task3任务创建失败!\r\n");

//    /* 动态创建Task4任务 */
//    xReturn = xTaskCreate(
//        (TaskFunction_t)Task4,          /* 任务函数 */
//        (const char *)"Task4",          /* 任务名称 */
//        (uint32_t)TASK4_STK_SIZE,       /* 任务堆栈大小 */
//        (void *)NULL,                   /* 传递给任务函数的参数 */
//        (UBaseType_t)TASK4_PRIO,        /* 任务优先级 */
//        (TaskHandle_t *)&Task4_Handle); /* 任务句柄 */

//    if (xReturn == pdPASS)
//        printf("Task4任务创建成功! 剩余堆内存: %lu bytes\r\n", xPortGetFreeHeapSize());
//    else
//        printf("Task4任务创建失败!\r\n");

//    /* 动态创建Task5任务 */
//    xReturn = xTaskCreate(
//        (TaskFunction_t)Task5,          /* 任务函数 */
//        (const char *)"Task5",          /* 任务名称 */
//        (uint32_t)TASK5_STK_SIZE,       /* 任务堆栈大小 */
//        (void *)NULL,                   /* 传递给任务函数的参数 */
//        (UBaseType_t)TASK5_PRIO,        /* 任务优先级 */
//        (TaskHandle_t *)&Task5_Handle); /* 任务句柄 */

//    if (xReturn == pdPASS)
//        printf("Task5任务创建成功! 剩余堆内存: %lu bytes\r\n", xPortGetFreeHeapSize());
//    else
//        printf("Task5任务创建失败!\r\n");

//    /* 动态创建Task6任务 */
//    xReturn = xTaskCreate(
//        (TaskFunction_t)Task6,          /* 任务函数 */
//        (const char *)"Task6",          /* 任务名称 */
//        (uint32_t)TASK6_STK_SIZE,       /* 任务堆栈大小 */
//        (void *)NULL,                   /* 传递给任务函数的参数 */
//        (UBaseType_t)TASK6_PRIO,        /* 任务优先级 */
//        (TaskHandle_t *)&Task6_Handle); /* 任务句柄 */

//    if (xReturn == pdPASS)
//        printf("Task6任务创建成功!\r\n");
//    else
//        printf("Task6任务创建失败!\r\n");
#endif
#endif

#if Static_Create_task
    /* 静态创建Task1任务 */
    Task1_Handle = xTaskCreateStatic(
        (TaskFunction_t)Task1,          /* 任务函数 */
        (const char *)"Task1",          /* 任务名称 */
        (uint32_t)TASK1_STK_SIZE,       /* 任务堆栈大小 */
        (void *)NULL,                   /* 传递给任务函数的参数 */
        (UBaseType_t)TASK1_PRIO,        /* 任务优先级 */
        (StackType_t *)Task1TaskStack,  /* 任务堆栈 */
        (StaticTask_t *)&Task1TaskTCB); /* 任务控制块 */

    if (Task1_Handle != NULL)
        printf("Task1任务创建成功!\r\n");
    else
        printf("Task1任务创建失败!\r\n");

    /* 静态创建Task2任务 */
    Task2_Handle = xTaskCreateStatic(
        (TaskFunction_t)Task2,          /* 任务函数 */
        (const char *)"Task2",          /* 任务名称 */
        (uint32_t)TASK2_STK_SIZE,       /* 任务堆栈大小 */
        (void *)NULL,                   /* 传递给任务函数的参数 */
        (UBaseType_t)TASK2_PRIO,        /* 任务优先级 */
        (StackType_t *)Task2TaskStack,  /* 任务堆栈 */
        (StaticTask_t *)&Task2TaskTCB); /* 任务控制块 */

    if (Task2_Handle != NULL)
        printf("Task2任务创建成功!\r\n");
    else
        printf("Task2任务创建失败!\r\n");

    /* 静态创建Task3任务 */
    Task3_Handle = xTaskCreateStatic(
        (TaskFunction_t)Task3,          /* 任务函数 */
        (const char *)"Task3",          /* 任务名称 */
        (uint32_t)TASK3_STK_SIZE,       /* 任务堆栈大小 */
        (void *)NULL,                   /* 传递给任务函数的参数 */
        (UBaseType_t)TASK3_PRIO,        /* 任务优先级 */
        (StackType_t *)Task3TaskStack,  /* 任务堆栈 */
        (StaticTask_t *)&Task3TaskTCB); /* 任务控制块 */

    if (Task3_Handle != NULL)
        printf("Task3任务创建成功!\r\n");
    else
        printf("Task3任务创建失败!\r\n");

#endif

		vTaskDelete(NULL);   /* 删除开始任务 */
    taskEXIT_CRITICAL(); /* 退出临界区 */

}

static void Task1(void *parameter)
{

    while (1)
    {
			if(xSemaphoreTake(mutex_semphore_handle, portMAX_DELAY) == pdTRUE)
			{
				//MPU6050_GetData(MPU6050_iic, &AX, &AY, &AZ, &GX, &GY, &GZ);
        //OLED_ShowStr(OLED_iic, 0, 3, (unsigned char *)"MPU6050X:", OLED_8x16); // 测试6*8字符
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
//				OLED_ShowSignedNum_C8T6(2, 1, AX, 5);					//OLED显示数据
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
				
				printf("task1运行\n");
				// 释放互斥锁
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
				printf("task2运行\n");
				
				// 释放互斥锁
        xSemaphoreGive(mutex_semphore_handle);
			}
        vTaskDelay(10);
    }
}

///* 时间片轮回，一个时间片大小，取决为滴答定时器中断频率。*/
//static void Task3(void *parameter)
//{
//    uint32_t task3_num = 0;
//    while (1)
//    {
//        taskENTER_CRITICAL(); /* 进入临界区 */
//        printf("task3运行次数：%d\r\n", ++task3_num);
//        taskEXIT_CRITICAL(); /* 退出临界区 */
//        vTaskDelay(100);     /* 延时100个tick */
//    }
//}

//static void Task4(void *parameter)
//{
//    uint32_t task4_num = 0;
//    while (1)
//    {
//        taskENTER_CRITICAL(); /* 进入临界区 */
//        printf("task4运行次数：%d\r\n", ++task4_num);
//        taskEXIT_CRITICAL(); /* 退出临界区 */
//			  delay_ms(1000);
//        vTaskDelay(100);     /* 延时100个tick */
//    }
//}

//char task_buff[500]; /* 清单信息缓存区 */
//char task_buff_time[500];
///**********************************************************
// * @funcName ：Task5
// * @brief    ：任务5
// * @param    ：void* parameter(传入参数,未用到)
// * @retval   ：void
// * @details  ：
// * @fn       ：使用查询FreeRTOS任务状态函数
// ************************************************************/
//static void Task5(void *parameter)
//{
//    UBaseType_t priority_num = 0;    /* 任务优先级值 */
//    UBaseType_t task_num = 0;        /* 任务数量 */
//    UBaseType_t task_num2 = 0;       /* 任务数量 */
//    TaskStatus_t *status_array = 0;  /* 所有任务状态信息 */
//    TaskStatus_t *status_array2 = 0; /* 指定的任务所有状态信息 */
//    TaskHandle_t task_handle = 0;    /* 某个任务的任务句柄 */
//    UBaseType_t task_stack_min = 0;  /* 剩余堆栈大小 */
//    int state = 0;                   /* 指定任务的状态信息 */

//    uint8_t i = 0;

//    /* 修改指定任务的优先级 */
//    vTaskPrioritySet(Task3_Handle, 6);

//    /* 获取指定任务优先级值，NULL表示获取自身的 */
//    priority_num = uxTaskPriorityGet(NULL);
//    printf("task1任务优先级为%ld\r\n", priority_num);

//    /* 获取当前系统中任务数量 */
//    task_num = uxTaskGetNumberOfTasks();
//    printf("任务数量：%ld\r\n", task_num);

//    /* 获取所有任务的状态信息 */
//    status_array = mymalloc(0, (sizeof(TaskStatus_t) * task_num)); /* 在SRAM的内存管理中申请空间 */
//    task_num2 = uxTaskGetSystemState(status_array, task_num, NULL);
//    printf("任务名\t\t任务优先级\t任务编号\r\n");
//    for (i = 0; i < task_num2; i++)
//    {
//        printf("%s\t\t%ld\t%ld\r\n",
//               status_array[i].pcTaskName,
//               status_array[i].uxCurrentPriority,
//               status_array[i].xTaskNumber);
//    }

//    /* 获取指定任务的所有信息v*/
//    status_array2 = mymalloc(SRAMIN, sizeof(TaskStatus_t));
//    vTaskGetInfo(Task2_Handle, status_array2, pdTRUE, eInvalid);
//    printf("任务名：%s\r\n", status_array2->pcTaskName);
//    printf("任务优先级：%ld\r\n", status_array2->uxCurrentPriority);
//    printf("任务编号：%ld\r\n", status_array2->xTaskNumber);
//    printf("任务状态：%d\r\n", status_array2->eCurrentState);

//    /* 通过任务名获取任务句柄 */
//    task_handle = xTaskGetHandle("Task1");
//    printf("任务句柄：%#x\r\n", (int)task_handle);
//    printf("task1的任务句柄：%#x\r\n", (int)Task1_Handle);

//    /* 获取指定任务的状态 */
//    eTaskGetState(Task2_Handle);
//    state = eTaskGetState(Task2_Handle);
//    printf("当前task2的任务状态为：%d\r\n", state);

//    /* 以清单形式获取所有任务状态信息 */
//    vTaskList(task_buff);
//    printf("%s\r\n", task_buff);
//		


//    while (1)
//    {
//        /* 获取剩余堆栈大小 */
//        task_stack_min = uxTaskGetStackHighWaterMark(Task2_Handle);
//        printf("task2历史剩余最小堆栈为%ld\r\n", task_stack_min);
//			
//				/* 以清单形式获取所有任务运行统计时间  */
//				vTaskGetRunTimeStats(task_buff_time);
//				printf("%s\r\n", task_buff_time);
//        vTaskDelay(800); /* 延时800个tick */
//    }
//}

///**********************************************************
// * @funcName ：Task6
// * @brief    ：任务六的配置信息传递函数
// * @param    ：void * parameter(传入参数,未用到)
// * @retval   ：void
// * @details  ：
// * @fn       ：
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
//        /*消息队列信息传递与创建、释放*/
//        err_Q = xQueueSend(big_date_queue, &buf, portMAX_DELAY);
//        if (err_Q != pdTRUE)
//            printf("key_queue队列发送失败\r\n");
//        else
//            printf("key_queue队列发送成功\r\n");
//        

//        err_Q = xQueueReceive(big_date_queue, &buf, portMAX_DELAY);
//        if (err_Q != pdTRUE)
//            printf("big_date_queue队列读取失败\r\n");
//        else
//            printf("数据：%s\r\n", buf);

//        /*二值信号信息传递与创建、释放*/
//        err_S = xSemaphoreTake(semphore_handle, portMAX_DELAY); /* 获取信号量并死等 */
//        if (err_S == pdTRUE)
//            printf("获取信号量成功\r\n");
//        else
//            printf("已超时%d\r\n", ++i);

//        if (semphore_handle != NULL)
//        {
//            err_S = xSemaphoreGive(semphore_handle); /* 释放二值信号量 */
//            if (err_S == pdPASS)
//                printf("信号量释放成功！！\r\n");
//            else
//                printf("信号量释放失败！！\r\n");
//        }

//        /*计数型信号信息传递与创建、释放*/
//        err_C = xSemaphoreTake(count_semphore_handle, portMAX_DELAY); /* 获取信号量并死等 */
//        if (err_C == pdTRUE)
//            printf("信号量的计数值为：%d\r\n", (int)uxSemaphoreGetCount(count_semphore_handle));
//        else
//            printf("计数型信号量获取失败\r\n");

//        if (count_semphore_handle != NULL)
//        {
//            xSemaphoreGive(count_semphore_handle); /* 释放信号量 */
//        }

//        /*互斥型信号(减少了优先级翻转影响)信息传递与创建、释放*/
//        err_M = xSemaphoreTake(mutex_semphore_handle, portMAX_DELAY); /* 获取信号量并死等 */
//        if (err_M == pdTRUE)
//            printf("获取信号量成功\r\n");
//        else
//            printf("获取信号量失败\r\n");

//        if (mutex_semphore_handle != NULL)
//        {
//            xSemaphoreGive(mutex_semphore_handle); /* 释放信号量 */
//        }

//        /*队列集信号信息传递与创建、释放*/
//        /* 查询队列集信息 */
//        member_handle = xQueueSelectFromSet(queueset_handle, portMAX_DELAY);
//        /* 识别队列集中是二值信号量还是消息队列有数据 */
//        if (member_handle == queue_handle)
//        {
//            /* 获取队列集中消息队列的数据 */
//            xQueueReceive(member_handle, &i, portMAX_DELAY);
//            printf("获取到的队列数据为：%d\r\n", i);
//        }
//        else if (member_handle == semphore_handle)
//        {
//            /* 获取队列集中二值信号量的数据 */
//            xSemaphoreTake(member_handle, portMAX_DELAY);
//            printf("获取信号量成功！！\r\n");
//        }
//        /* 向队列集中的消息队列发送数据 */
//        err_S_Q = xQueueSend(queue_handle, &i, portMAX_DELAY);
//        if (err_S_Q == pdPASS)
//            printf("往队列queue_handle写入数据成功！！\r\n");
//        else
//            printf("往队列queue_handle写入数据失败！！\r\n");
//        /* 向队列集中的二值信号量释放信号量 */
//        err_S_S = xSemaphoreGive(semphore_handle);
//        if (err_S_S == pdPASS)
//            printf("释放信号量成功！！\r\n");
//        else
//            printf("释放信号量失败！！\r\n");

//        /*事件标志组信号信息传递与创建、释放*/
//        event_bit = xEventGroupWaitBits(eventgroup_handle,       /* 事件标志组句柄 */
//                                        EVENTBIT_0 | EVENTBIT_1, /* 等待事件标志组的bit0和bit1位 */
//                                        pdTRUE,                  /* 成功等待到事件标志位后，清除事件标志组中的bit0和bit1位 */
//                                        pdTRUE,                  /* 等待事件标志组的bit0和bit1位都置1,就成立 */
//                                        portMAX_DELAY);          /* 死等 */
//        printf("等待到的事件标志位值为：%#x\r\n", event_bit);

//        xEventGroupSetBits(eventgroup_handle, EVENTBIT_0); /* 将事件标志组的bit0位置1 */
//        xEventGroupSetBits(eventgroup_handle, EVENTBIT_1); /* 将事件标志组的bit1位置1 */

//        /*任务通知信号信息传递与创建、释放*/
//        // 模拟消息队列
//        xTaskNotifyWait(0, 0xFFFFFFFF, &noyify_val, portMAX_DELAY);
//        printf("任务通知模拟消息邮箱发送，发送的键值为：%d\r\n", i);
//        xTaskNotify(Task5_Handle, i, eSetValueWithOverwrite);
//        // 模拟二值信号量
//        reveice = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
//        printf("接收任务通知成功，模拟获取二值信号量！\r\n");
//        printf("任务通知模拟二值信号量释放！\r\n");
//        xTaskNotifyGive(Task5_Handle);
//        // 模拟计数信号量
//        reveice = ulTaskNotifyTake(pdFALSE, portMAX_DELAY);
//        printf("任务通知模拟计数型信号量释放！\r\n");
//        xTaskNotifyGive(Task5_Handle);
//        // 模拟事件标志组
//        xTaskNotifyWait(0, 0xFFFFFFFF, &notify_val_R, portMAX_DELAY);
//        /* 事件1 */
//        if (notify_val_R & EVENTBIT_0)
//            event_bit_R |= EVENTBIT_0;
//        /* 事件2 */
//        if (notify_val_R & EVENTBIT_1)
//            event_bit_R |= EVENTBIT_1;
//        /* 事件1和事件2都发生 */
//        if (event_bit_R == (EVENTBIT_0 | EVENTBIT_1))
//        {
//            printf("任务通知模拟事件标志组接收成功！！\r\n");
//            event_bit_R = 0;
//        }
//        printf("将bit0位置1\r\n");
//        xTaskNotify(Task5_Handle, EVENTBIT_0, eSetBits);

//        vTaskDelay(10); /* 延时100个tick */
//    }
//}
///**********************************************************
// * @funcName ：timer1_callback
// * @brief    ：软件定时器1的超时回调函数
// * @param    ：TimerHandle_t pxTimer(传入参数,未用到)
// * @retval   ：void
// * @details  ：
// * @fn       ：
// ************************************************************/
//void timer1_callback(TimerHandle_t pxTimer)
//{
//    static uint32_t timer = 0;
//    printf("timer1的运行次数：%d\r\n", ++timer);
//}

///**********************************************************
// * @funcName ：timer2_callback
// * @brief    ：软件定时器2的超时回调函数
// * @param    ：TimerHandle_t pxTimer(传入参数,未用到)
// * @retval   ：void
// * @details  ：
// * @fn       ：
// ************************************************************/
//void timer2_callback(TimerHandle_t pxTimer)
//{
//    static uint32_t timer = 0;
//    printf("timer2的运行次数：%d\r\n", ++timer);
//}

///**********************************************************
// * @funcName ：PRE_SLEEP_PROCESSING
// * @brief    ：进入低功耗前所需要执行的操作回调函数
// * @param    ：void
// * @retval   ：void
// * @details  ：
// * @fn       ：
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
// * @funcName ：POST_SLEEP_PROCESSING
// * @brief    ：退出低功耗后所需要执行的操作回调函数
// * @param    ：void
// * @retval   ：void
// * @details  ：
// * @fn       ：
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
