/*
 * 中断向量表（参考实现，面向 arm-none-eabi-gcc，不在 PC 上编译）
 * 对应文章：《ARM Cortex-M启动揭秘……》"中断向量表"
 *
 * 说明：向量表必须放在 .isr_vector 段，并由链接器脚本置于 Flash 起始处。
 */
#include <stdint.h>

extern uint32_t _estack;          /* 由链接器脚本定义：栈顶 */
void Reset_Handler(void);

/* 弱定义的默认异常处理，实际项目中可被覆盖 */
void Default_Handler(void)        { while (1) { } }
#define WEAK_ALIAS __attribute__((weak, alias("Default_Handler")))

void NMI_Handler(void)            WEAK_ALIAS;
void HardFault_Handler(void)      WEAK_ALIAS;
void MemManage_Handler(void)      WEAK_ALIAS;   /* M0/M0+ 没有此高级异常 */
void BusFault_Handler(void)       WEAK_ALIAS;   /* M0/M0+ 没有此高级异常 */
void UsageFault_Handler(void)     WEAK_ALIAS;   /* M0/M0+ 没有此高级异常 */
void SVC_Handler(void)            WEAK_ALIAS;
void DebugMon_Handler(void)       WEAK_ALIAS;
void PendSV_Handler(void)         WEAK_ALIAS;
void SysTick_Handler(void)        WEAK_ALIAS;

/* 部分外部中断示例 */
void WWDG_IRQHandler(void)        WEAK_ALIAS;
void PVD_IRQHandler(void)         WEAK_ALIAS;

__attribute__((section(".isr_vector")))
void (* const g_pfnVectors[])(void) = {
    (void (*)(void))(&_estack),   /* 0. 栈顶地址 (MSP 初值) */
    Reset_Handler,                /* 1. 复位 */
    NMI_Handler,                  /* 2. NMI */
    HardFault_Handler,            /* 3. HardFault */
    MemManage_Handler,            /* 4. */
    BusFault_Handler,             /* 5. */
    UsageFault_Handler,           /* 6. */
    0, 0, 0, 0,                   /* 7-10. 保留 */
    SVC_Handler,                  /* 11. */
    DebugMon_Handler,             /* 12. */
    0,                            /* 13. 保留 */
    PendSV_Handler,               /* 14. */
    SysTick_Handler,              /* 15. */
    WWDG_IRQHandler,              /* 16. */
    PVD_IRQHandler,               /* 17. */
    /* ... 依具体 MCU 型号继续 ... */
};
