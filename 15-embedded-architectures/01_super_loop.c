/*
 * 架构一：超级循环 (Super Loop)
 * 对应文章：《嵌入式软件架构的"十八般武艺"》
 * 编译： gcc -Wall -Wextra -o super_loop 01_super_loop.c
 */
#include <stdio.h>

static void SystemInit(void)     { puts("SystemInit"); }
static void PeripheralInit(void) { puts("PeripheralInit"); }
static void CheckInputs(int i)        { printf("  [%d] CheckInputs\n", i); }
static void ProcessData(int i)        { printf("  [%d] ProcessData\n", i); }
static void UpdateOutputs(int i)      { printf("  [%d] UpdateOutputs\n", i); }
static void HandleCommunication(int i){ printf("  [%d] HandleCommunication\n", i); }

int main(void)
{
    SystemInit();
    PeripheralInit();
    puts("进入超级循环（演示只跑3圈）:");
    for (int loop = 0; loop < 3; ++loop) {   /* 真实系统里是 while(1) */
        CheckInputs(loop);
        ProcessData(loop);
        UpdateOutputs(loop);
        HandleCommunication(loop);
    }
    return 0;
}
