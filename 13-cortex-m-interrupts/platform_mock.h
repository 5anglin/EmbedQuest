/*
 * 极简"寄存器/中断"模拟层，用于在 PC 上运行中断示例
 * 对应文章：《ARM Cortex-M中断处理全解析》
 *
 * 真实工程中这些由 CMSIS/HAL/LL 提供；这里用软件模拟以便 gcc 编译运行。
 */
#ifndef PLATFORM_MOCK_H
#define PLATFORM_MOCK_H

#include <stdint.h>

/* 模拟全局中断开关（对应 PRIMASK / __disable_irq / __enable_irq） */
extern volatile uint32_t g_primask;
static inline uint32_t __get_PRIMASK(void) { return g_primask; }
static inline void      __disable_irq(void) { g_primask = 1; }
static inline void      __enable_irq(void)  { g_primask = 0; }

/* 模拟一个 UART 外设：数据寄存器 + RXNE 标志 */
typedef struct {
    uint8_t  dr;        /* 数据寄存器 */
    uint8_t  rxne;      /* 接收非空标志 */
    uint8_t  rxne_ie;   /* 接收中断使能 */
} MockUART_t;

extern MockUART_t MockUART1;

static inline uint8_t LL_USART_IsActiveFlag_RXNE(MockUART_t *u)  { return u->rxne; }
static inline uint8_t LL_USART_IsEnabledIT_RXNE(MockUART_t *u)   { return u->rxne_ie; }
static inline uint8_t LL_USART_ReceiveData8(MockUART_t *u)       { u->rxne = 0; return u->dr; }

/* ISR 由外设模拟器调用 */
void USART1_IRQHandler(void);

/* 外设模拟器：模拟"硬件收到一个字节"并触发中断 */
void mock_uart_feed_byte(uint8_t byte);

#endif /* PLATFORM_MOCK_H */
