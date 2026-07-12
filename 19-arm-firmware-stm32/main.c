/*
 * 真机固件示例：STM32F103C8（Blue Pill）裸机点灯
 * 配套文章：《ARM Cortex-M启动揭秘》《ARM Cortex-M中断处理全解析》
 *
 * 直接操作寄存器让板载 LED（PC13，低电平点亮）闪烁；不依赖 HAL/CMSIS。
 * 用 arm-none-eabi-gcc 交叉编译，产物为 .elf/.bin/.hex，可烧录到真板。
 *   构建： make
 *   烧录： make flash        (需 st-flash 或 openocd，见 README)
 */
#include <stdint.h>

/* ---- STM32F103 寄存器（最小子集，直接内存映射）---- */
#define PERIPH_BASE      0x40000000UL
#define APB2PERIPH_BASE  (PERIPH_BASE + 0x10000UL)
#define AHBPERIPH_BASE   (PERIPH_BASE + 0x20000UL)

#define RCC_BASE         (AHBPERIPH_BASE + 0x1000UL)   /* 0x40021000 */
#define GPIOC_BASE       (APB2PERIPH_BASE + 0x1000UL)  /* 0x40011000 */

typedef struct {
    volatile uint32_t CR;
    volatile uint32_t CFGR;
    volatile uint32_t CIR;
    volatile uint32_t APB2RSTR;
    volatile uint32_t APB1RSTR;
    volatile uint32_t AHBENR;
    volatile uint32_t APB2ENR;
    volatile uint32_t APB1ENR;
    volatile uint32_t BDCR;
    volatile uint32_t CSR;
} RCC_TypeDef;

typedef struct {
    volatile uint32_t CRL;
    volatile uint32_t CRH;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t BRR;
    volatile uint32_t LCKR;
} GPIO_TypeDef;

#define RCC   ((RCC_TypeDef *)  RCC_BASE)
#define GPIOC ((GPIO_TypeDef *) GPIOC_BASE)

#define RCC_APB2ENR_IOPCEN  (1UL << 4)   /* GPIOC 时钟使能 */

/* 简单忙等延时（非精确，仅用于点灯演示） */
static void delay(volatile uint32_t count)
{
    while (count--) {
        __asm__ volatile ("nop");
    }
}

int main(void)
{
    /* 1. 使能 GPIOC 时钟 */
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;

    /* 2. 配置 PC13 为通用推挽输出，2MHz
     *    PC13 在 CRH 的第 5 组（bit[23:20]）：MODE=0b10(2MHz), CNF=0b00(推挽) => 0x2 */
    GPIOC->CRH &= ~(0xFUL << 20);
    GPIOC->CRH |=  (0x2UL << 20);

    /* 3. 闪烁：PC13 低电平点亮 LED */
    for (;;) {
        GPIOC->BSRR = (1UL << 13);        /* 置 1 -> LED 灭 */
        delay(500000);
        GPIOC->BSRR = (1UL << (13 + 16)); /* 复位(置0) -> LED 亮 */
        delay(500000);
    }
    /* 不会到达 */
}
