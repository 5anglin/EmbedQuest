/*
 * 中断处理最佳实践演示："ISR 只设标志/存数据，主循环做处理"
 * 对应文章：《ARM Cortex-M中断处理全解析》
 *
 * 编译： gcc -Wall -Wextra -o interrupt_demo interrupt_demo.c
 * 运行：模拟串口陆续收到几个字节，触发中断，主循环处理。
 */
#include "platform_mock.h"
#include <stdio.h>

/* ---- 模拟层实体 ---- */
volatile uint32_t g_primask = 0;
MockUART_t MockUART1 = { 0, 0, 1 };   /* 使能 RXNE 中断 */

/* ---- 共享数据（ISR 与主循环共用，必须 volatile）---- */
#define RXBUF_SIZE 64
static volatile uint8_t  g_rxBuffer[RXBUF_SIZE];
static volatile uint8_t  g_writeIdx = 0;
static volatile uint8_t  g_readIdx  = 0;
static volatile uint8_t  g_rxFlag   = 0;

/* ---- 中断服务程序：短小、只做最紧急的事 ---- */
void USART1_IRQHandler(void)
{
    if (LL_USART_IsActiveFlag_RXNE(&MockUART1) && LL_USART_IsEnabledIT_RXNE(&MockUART1)) {
        uint8_t data = LL_USART_ReceiveData8(&MockUART1);   /* 读数据即清标志 */

        /* 临界区保护共享索引（模拟关中断） */
        uint32_t primask = __get_PRIMASK();
        __disable_irq();
        g_rxBuffer[g_writeIdx] = data;
        g_writeIdx = (uint8_t)((g_writeIdx + 1) % RXBUF_SIZE);
        if (!primask) __enable_irq();

        g_rxFlag = 1;   /* 通知主循环 */
    }
}

static void process_received_data(uint8_t data)
{
    printf("[主循环] 处理收到的字节: 0x%02X ('%c')\n", data,
           (data >= 32 && data < 127) ? data : '.');
}

/* 模拟外设：把一个字节塞进 DR 并触发中断 */
void mock_uart_feed_byte(uint8_t byte)
{
    MockUART1.dr = byte;
    MockUART1.rxne = 1;
    USART1_IRQHandler();   /* 硬件会自动向量跳转，这里手动调用模拟 */
}

int main(void)
{
    const char *incoming = "Hi!";
    puts("=== 中断处理演示（ISR 设标志，主循环处理）===");

    /* 模拟硬件陆续收到字节 */
    for (const char *p = incoming; *p; ++p)
        mock_uart_feed_byte((uint8_t)*p);

    /* 主循环：检查标志并处理缓冲区里的数据 */
    while (g_rxFlag) {
        g_rxFlag = 0;
        while (g_readIdx != g_writeIdx) {
            uint8_t d = g_rxBuffer[g_readIdx];
            g_readIdx = (uint8_t)((g_readIdx + 1) % RXBUF_SIZE);
            process_received_data(d);
        }
    }
    puts("全部数据处理完毕。");
    return 0;
}
