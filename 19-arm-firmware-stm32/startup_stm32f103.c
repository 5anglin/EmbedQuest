/*
 * 启动文件（C 实现）：向量表 + Reset_Handler
 * 面向 arm-none-eabi-gcc / STM32F103C8。对应文章《ARM Cortex-M启动揭秘》。
 *
 * 硬件在复位时：从向量表[0]载入 MSP，从向量表[1]取 Reset_Handler 并跳转。
 * 因此 Reset_Handler 可以是普通 C 函数（进入时 SP 已就绪）。
 */
#include <stdint.h>

/* 链接器脚本提供的符号 */
extern uint32_t _sidata;   /* .data 在 Flash 中的加载地址 */
extern uint32_t _sdata;    /* .data 在 RAM 中的起始 */
extern uint32_t _edata;    /* .data 结束 */
extern uint32_t _sbss;     /* .bss 起始 */
extern uint32_t _ebss;     /* .bss 结束 */
extern uint32_t _estack;   /* 栈顶（RAM 末端）*/

int  main(void);
void SystemInit(void);
void Reset_Handler(void);
void Default_Handler(void);

/* 弱定义异常/中断，允许被具体实现覆盖 */
#define WEAK_ALIAS __attribute__((weak, alias("Default_Handler")))
void NMI_Handler(void)        WEAK_ALIAS;
void HardFault_Handler(void)  WEAK_ALIAS;
void MemManage_Handler(void)  WEAK_ALIAS;
void BusFault_Handler(void)   WEAK_ALIAS;
void UsageFault_Handler(void) WEAK_ALIAS;
void SVC_Handler(void)        WEAK_ALIAS;
void DebugMon_Handler(void)   WEAK_ALIAS;
void PendSV_Handler(void)     WEAK_ALIAS;
void SysTick_Handler(void)    WEAK_ALIAS;

/* 向量表：放入 .isr_vector 段，由链接脚本置于 Flash 起始 */
__attribute__((section(".isr_vector"), used))
void (* const g_pfnVectors[])(void) = {
    (void (*)(void))(&_estack),  /* 0  初始 MSP */
    Reset_Handler,               /* 1  复位 */
    NMI_Handler,                 /* 2 */
    HardFault_Handler,           /* 3 */
    MemManage_Handler,           /* 4 */
    BusFault_Handler,            /* 5 */
    UsageFault_Handler,          /* 6 */
    0, 0, 0, 0,                  /* 7-10 保留 */
    SVC_Handler,                 /* 11 */
    DebugMon_Handler,            /* 12 */
    0,                           /* 13 保留 */
    PendSV_Handler,              /* 14 */
    SysTick_Handler,             /* 15 */
    /* 16+ 外部中断可按需补充 */
};

void Reset_Handler(void)
{
    /* 1. 复制 .data 段：Flash -> RAM */
    uint32_t *src = &_sidata;
    uint32_t *dst = &_sdata;
    while (dst < &_edata) {
        *dst++ = *src++;
    }

    /* 2. 清零 .bss 段 */
    dst = &_sbss;
    while (dst < &_ebss) {
        *dst++ = 0;
    }

    /* 3. 系统初始化（时钟等） */
    SystemInit();

    /* 4. 跳转到 main */
    (void)main();

    /* 5. main 不应返回，兜底死循环 */
    for (;;) {
    }
}

void Default_Handler(void)
{
    for (;;) {
    }
}
