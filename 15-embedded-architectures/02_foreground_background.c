/*
 * 架构二：前后台系统 (中断前台 + 主循环后台)，含 SPSC 无锁环形缓冲区
 * 对应文章：《嵌入式软件架构的"十八般武艺"》
 * 编译： gcc -Wall -Wextra -o foreground_background 02_foreground_background.c
 */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define BUFFER_SIZE 16
typedef struct {
    uint16_t data[BUFFER_SIZE];
    volatile uint32_t head;   /* 仅中断更新 */
    volatile uint32_t tail;   /* 仅主循环更新 */
} RingBuffer_t;

static RingBuffer_t AdcBuffer;

/* 前台：ADC 中断（PC 上由模拟器调用） */
static void ADC_IRQHandler(uint16_t sample)
{
    uint32_t next_head = (AdcBuffer.head + 1) % BUFFER_SIZE;
    if (next_head != AdcBuffer.tail) {           /* 未满才写 */
        AdcBuffer.data[AdcBuffer.head] = sample;
        AdcBuffer.head = next_head;
    }
}

/* 后台：安全取数据 */
static bool GetADCData(uint16_t *out)
{
    if (AdcBuffer.head == AdcBuffer.tail) return false;  /* 空 */
    *out = AdcBuffer.data[AdcBuffer.tail];
    AdcBuffer.tail = (AdcBuffer.tail + 1) % BUFFER_SIZE;
    return true;
}

static void ProcessADCData(uint16_t v) { printf("  处理ADC采样: %u\n", v); }

int main(void)
{
    puts("=== 前后台系统演示 ===");
    /* 模拟前台连续产生 5 个采样 */
    for (uint16_t i = 0; i < 5; ++i) ADC_IRQHandler((uint16_t)(1000 + i));

    /* 后台主循环消费 */
    uint16_t v;
    while (GetADCData(&v)) ProcessADCData(v);
    puts("缓冲区已清空。");
    return 0;
}
