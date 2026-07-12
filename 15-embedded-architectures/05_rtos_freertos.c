/*
 * 架构五：RTOS 多任务架构 (FreeRTOS 风格)
 * 对应文章：《嵌入式软件架构的"十八般武艺"》
 *
 * 说明：真实工程链接 FreeRTOS 内核。为便于在 PC 上讲解结构，本文件配合
 *      freertos_mock.h（API 桩）编译；它按顺序演示"采集->入队->控制出队"的数据流，
 *      不做真正的抢占式调度。
 *
 * 编译（讲解用）： gcc -Wall -Wextra -o rtos_demo 05_rtos_freertos.c
 */
#include "freertos_mock.h"
#include <stdio.h>
#include <stdint.h>

typedef struct { uint16_t value; } SensorData;

static QueueHandle_t     sensorDataQueue;
static SemaphoreHandle_t i2cBusMutex;

static void ReadSensorData(SensorData *d) { static uint16_t v = 500; d->value = v++; }
static void CalculateControl(const SensorData *d) { printf("  [Control] 依据采样 %u 计算控制量\n", d->value); }
static void UpdateActuators(void) { puts("  [Control] 更新执行器"); }

static void SensorTask(void)
{
    SensorData data;
    if (xSemaphoreTake(i2cBusMutex, portMAX_DELAY) == pdTRUE) {
        ReadSensorData(&data);
        xSemaphoreGive(i2cBusMutex);
        printf("  [Sensor] 采集到 %u，发送到控制任务\n", data.value);
        xQueueSend(sensorDataQueue, &data, portMAX_DELAY);
    }
}

static void ControlTask(void)
{
    SensorData data;
    if (xQueueReceive(sensorDataQueue, &data, portMAX_DELAY) == pdPASS) {
        CalculateControl(&data);
        UpdateActuators();
    }
}

int main(void)
{
    puts("=== RTOS 多任务架构演示（桩调度）===");
    sensorDataQueue = xQueueCreate(10, sizeof(SensorData));
    i2cBusMutex     = xSemaphoreCreateMutex();

    /* 真实工程：xTaskCreate(...) 然后 vTaskStartScheduler();
     * 这里按数据流顺序各执行一次以演示任务间通信。 */
    for (int i = 0; i < 3; ++i) {
        SensorTask();
        ControlTask();
    }
    return 0;
}
