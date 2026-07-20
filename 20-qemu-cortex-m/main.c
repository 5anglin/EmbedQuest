/*
 * QEMU 可运行的 Cortex-M 示例（ARM 半主机 semihosting）
 * 配套文章：《ARM Cortex-M启动揭秘》《ARM Cortex-M中断处理全解析》
 *
 * 真板上 LED 靠 GPIO 点亮；在 QEMU 里没有真 LED，于是用半主机把"点灯/熄灯"
 * 打印到宿主机终端，从而"看得见"程序在跑。printf 通过 newlib 的 rdimon
 * (semihosting) 走到 QEMU 控制台。
 *
 *   构建： make
 *   运行： make run        (调用 qemu-system-arm，无需真硬件)
 */
#include <stdio.h>
#include <stdint.h>

/* newlib rdimon 提供：初始化半主机文件句柄，之后 printf 才能输出 */
extern void initialise_monitor_handles(void);

static void delay(volatile uint32_t n)
{
    while (n--) {
        __asm__ volatile ("nop");
    }
}

int main(void)
{
    initialise_monitor_handles();

    printf("=== Cortex-M blinky in QEMU (semihosting) ===\n");
    for (int i = 1; i <= 5; ++i) {
        printf("LED ON   (blink %d)\n", i);
        delay(100000);
        printf("LED OFF  (blink %d)\n", i);
        delay(100000);
    }
    printf("done\n");
    return 0;   /* 返回后 Reset_Handler 调用 exit()，QEMU 借半主机退出 */
}
