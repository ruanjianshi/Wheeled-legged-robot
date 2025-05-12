#ifndef __FREERTOSHANDLE_H
#define __FREERTOSHANDLE_H

#include "FreeRTOS.h"
#include "task.h"
#include "led.h"
#include "usart.h"
#include "systick.h"
#include "delay.h"
#include "oled_show.h"
#include "malloc.h"
#include "mpu6050.h"
#include "queue.h"
#include "event_groups.h"
#include "semphr.h"
//如果没有添加头文件，会导致数据读取错误，数据乱飘。
#include "as5600.h"
#include "foc.h"
#include "bluetooth.h"

#define Static_Create_task configSUPPORT_STATIC_ALLOCATION
#define Dynamic_Create_task configSUPPORT_DYNAMIC_ALLOCATION
#define Dynamic_Create_task_define 1
#define IsLaunchSoft_timer 0
#define IsLaunchCreateMessage 0
#define IsLaunchQueueCreate 0
#define IsLaunchSemaphoreCreateBinary 0
#define IsLaunchSemaphoreCreateCounting 0
#define IsLaunchSemaphoreCreateMutex 0
#define IsLaunchQueueCreateSet 0
#define IsLaunchEventGroupCreate 0
#define IsLaunchFreeRTOS 0

void freertos_head(void);
#endif
