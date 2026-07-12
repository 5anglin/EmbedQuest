/*
 * 极简 FreeRTOS API 桩，用于让 RTOS 架构示例在 PC 上"编译通过并顺序演示"。
 * 注意：这不是真正的调度器，仅按注册顺序各跑一次任务体的一小段，用于讲解结构。
 */
#ifndef FREERTOS_MOCK_H
#define FREERTOS_MOCK_H
#include <stdint.h>
#include <stddef.h>
#include <string.h>

typedef long BaseType_t;
typedef unsigned long UBaseType_t;
typedef void *TaskHandle_t;
typedef void *QueueHandle_t;
typedef void *SemaphoreHandle_t;
typedef void (*TaskFunction_t)(void *);

#define pdTRUE        1
#define pdFALSE       0
#define pdPASS        1
#define portMAX_DELAY 0xFFFFFFFFUL
#define pdMS_TO_TICKS(ms) (ms)

/* 用静态存储模拟一个单元素队列 */
typedef struct { unsigned char buf[64]; size_t item; int full; } MockQueue_t;
static MockQueue_t g_mock_queue;

static inline QueueHandle_t xQueueCreate(UBaseType_t len, UBaseType_t item)
{ (void)len; g_mock_queue.item = item; g_mock_queue.full = 0; return &g_mock_queue; }
static inline SemaphoreHandle_t xSemaphoreCreateMutex(void) { static int m; return &m; }
static inline BaseType_t xSemaphoreTake(SemaphoreHandle_t s, uint32_t t){ (void)s;(void)t; return pdTRUE; }
static inline BaseType_t xSemaphoreGive(SemaphoreHandle_t s){ (void)s; return pdTRUE; }
static inline BaseType_t xQueueSend(QueueHandle_t q, const void *item, uint32_t t)
{ MockQueue_t *m=q;(void)t; memcpy(m->buf,item,m->item); m->full=1; return pdPASS; }
static inline BaseType_t xQueueReceive(QueueHandle_t q, void *out, uint32_t t)
{ MockQueue_t *m=q;(void)t; if(!m->full) return pdFALSE; memcpy(out,m->buf,m->item); m->full=0; return pdPASS; }
static inline void vTaskDelay(uint32_t t){ (void)t; }

#endif /* FREERTOS_MOCK_H */
