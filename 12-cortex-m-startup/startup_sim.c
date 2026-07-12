/*
 * Cortex-M 启动流程"可运行"仿真
 * 对应文章：《ARM Cortex-M启动揭秘：从芯片上电那一刻到你的main()函数》
 *
 * 真实的 Reset_Handler 依赖链接器脚本符号(_sidata/_sdata/_edata/_sbss/_ebss)。
 * 本文件在 PC 上用普通数组模拟 Flash 中的 .data 初值、RAM 中的 .data/.bss 段，
 * 完整复现"复制 .data、清零 .bss、SystemInit、跳转 main"的顺序，便于理解。
 *
 * 编译： gcc -Wall -Wextra -o startup_sim startup_sim.c
 * 真实工程请配合同目录的 gcc_linker_script.ld 与 vector_table.c 使用。
 */
#include <stdio.h>
#include <stdint.h>
#include <string.h>

/* ---- 模拟"链接器符号"：Flash 中的 .data 初值 与 RAM 中的段 ---- */
#define DATA_WORDS 4
#define BSS_WORDS  4

static const uint32_t flash_data_init[DATA_WORDS] = { 0x11111111, 0x22222222,
                                                      0x33333333, 0x44444444 };
static uint32_t ram_data[DATA_WORDS];   /* 对应 .data 段 */
static uint32_t ram_bss[BSS_WORDS] = { 0xDEADBEEF, 0xDEADBEEF,
                                       0xDEADBEEF, 0xDEADBEEF }; /* 故意填脏值 */

/* 全局变量：应被启动代码正确初始化 */
int   g_initialized_var = 0;   /* .data 语义：需要从 Flash 复制初值 */
int   g_zero_var;              /* .bss  语义：需要清零 */

/* ---- 启动各阶段 ---- */
static void copy_data_section(void)
{
    memcpy(ram_data, flash_data_init, sizeof(ram_data));   /* .data: Flash -> RAM */
    puts("[Reset] 步骤2: 复制 .data 段 (Flash 初值 -> RAM)");
}

static void zero_bss_section(void)
{
    memset(ram_bss, 0, sizeof(ram_bss));                   /* .bss: 清零 */
    puts("[Reset] 步骤3: 清零 .bss 段");
}

static void SystemInit(void)
{
    puts("[Reset] 步骤5: SystemInit() 配置时钟/Flash等待周期/电源");
}

static void __libc_init_array(void)
{
    puts("[Reset] 步骤7: __libc_init_array() 运行 C/C++ 构造函数");
}

/* 用户入口 */
static int app_main(void)
{
    g_initialized_var = 0xA5;   /* 证明全局变量已可用 */
    g_zero_var += 1;
    puts("[main] 用户代码运行中……");
    printf("       ram_data[0]=0x%08X (应为0x11111111)\n", ram_data[0]);
    printf("       ram_bss[0] =0x%08X (应为0x00000000)\n", ram_bss[0]);
    return 0;
}

/* 模拟的 Reset_Handler，复现文章中的执行顺序 */
static void Reset_Handler(void)
{
    puts("[Reset] 步骤1: 设置堆栈指针 MSP、启用 FPU(若有)");
    copy_data_section();
    zero_bss_section();
    puts("[Reset] 步骤4: (可选) 配置 MPU");
    SystemInit();
    puts("[Reset] 步骤6: NVIC_SetPriorityGrouping()");
    __libc_init_array();
    puts("[Reset] 步骤8: 跳转到 main()\n");
    app_main();
    puts("\n[Reset] 步骤9: main 返回后进入 while(1)/低功耗");
}

int main(void)
{
    puts("=== Cortex-M 启动流程仿真（上电 -> main）===\n");
    Reset_Handler();
    return 0;
}
