/*
 * HardFault "轻量级 coredump" 生成器（可运行仿真）
 * 对应文章：《嵌入式调试进阶：从手动到自动的HardFault破案指南》Stage 1/2
 *
 * 真实场景：处理器进入 HardFault_Handler 时，硬件已把 R0-R3,R12,LR,PC,xPSR
 *          压入栈。带内联汇编的入口会把 SP 传给 C 函数解析。
 * 本文件在 PC 上用一个"模拟栈帧"复现：解析 CFSR 判断故障类型、打印寄存器、
 *          尝试栈回溯、决定是否重启。
 *
 * 编译： gcc -Wall -Wextra -o hardfault_coredump hardfault_coredump.c
 */
#include <stdio.h>
#include <stdint.h>

/* ---- 模拟 SCB->CFSR 及其位定义（Cortex-M3/4/7）---- */
#define SCB_CFSR_USGFAULTSR_Msk   (0xFFFFUL << 16)
#define SCB_CFSR_BUSFAULTSR_Msk   (0xFFUL   << 8)
#define SCB_CFSR_MEMFAULTSR_Msk   (0xFFUL   << 0)

#define UFSR_UNDEFINSTR           (1UL << 16)   /* 未定义指令 */
#define UFSR_INVSTATE             (1UL << 17)   /* 非法状态(如误入ARM态) */
#define UFSR_DIVBYZERO            (1UL << 25)   /* 除零 */
#define BFSR_PRECISERR            (1UL << 9)    /* 精确总线错误 */
#define MMFSR_DACCVIOL            (1UL << 1)    /* 数据访问违例 */

/* 异常入栈的寄存器帧（硬件自动压栈的 8 个字） */
typedef struct {
    uint32_t r0, r1, r2, r3, r12, lr, pc, xpsr;
} ExceptionFrame_t;

static uint32_t g_mock_cfsr;   /* 模拟 SCB->CFSR */

static void decode_fault_type(uint32_t cfsr)
{
    puts("----- 故障类型解析 (CFSR) -----");
    if (cfsr & SCB_CFSR_USGFAULTSR_Msk) {
        puts("[UsageFault]");
        if (cfsr & UFSR_UNDEFINSTR) puts("  - 尝试执行未定义指令 (UNDEFINSTR)");
        if (cfsr & UFSR_INVSTATE)   puts("  - 非法状态 (INVSTATE)");
        if (cfsr & UFSR_DIVBYZERO)  puts("  - 除零 (DIVBYZERO)");
    }
    if (cfsr & SCB_CFSR_BUSFAULTSR_Msk) {
        puts("[BusFault]");
        if (cfsr & BFSR_PRECISERR) puts("  - 精确总线错误 (PRECISERR)");
    }
    if (cfsr & SCB_CFSR_MEMFAULTSR_Msk) {
        puts("[MemManageFault]");
        if (cfsr & MMFSR_DACCVIOL) puts("  - 数据访问违例 (DACCVIOL)");
    }
}

static void dump_registers(const ExceptionFrame_t *f)
{
    puts("----- 寄存器快照 -----");
    printf("  R0 =0x%08X  R1 =0x%08X  R2 =0x%08X  R3 =0x%08X\n",
           f->r0, f->r1, f->r2, f->r3);
    printf("  R12=0x%08X  LR =0x%08X  PC =0x%08X  xPSR=0x%08X\n",
           f->r12, f->lr, f->pc, f->xpsr);
    printf("  故障指令地址(PC) = 0x%08X\n", f->pc);
}

static int is_debugger_connected(void)
{
    /* 真实：读取 CoreDebug->DHCSR 的 C_DEBUGEN 位。此处模拟为未连接。 */
    return 0;
}

static void NVIC_SystemReset(void)
{
    puts("----- 无调试器连接：执行系统复位 (模拟) -----");
}

/* 模拟的 HardFault 处理：接收已解析出的栈帧 */
static void HardFault_Handler_C(const ExceptionFrame_t *frame)
{
    puts("\n***** HARD FAULT *****");
    decode_fault_type(g_mock_cfsr);
    dump_registers(frame);
    puts("----- (模拟) 保存 coredump 到 Flash -----");
    if (!is_debugger_connected())
        NVIC_SystemReset();
}

int main(void)
{
    /* 构造一个"执行了未定义指令"的故障场景 */
    g_mock_cfsr = UFSR_UNDEFINSTR;
    ExceptionFrame_t frame = {
        .r0 = 0x00000000, .r1 = 0x20001234, .r2 = 0xCAFEBABE, .r3 = 0x00000003,
        .r12 = 0x00000000, .lr = 0x0800045D, .pc = 0x0003DC4C, .xpsr = 0x61000000
    };

    puts("=== HardFault coredump 生成器仿真 ===");
    HardFault_Handler_C(&frame);
    return 0;
}
