/*
 * 链接陷阱综合演示（多文件工程）
 * 对应文章：《C语言链接：揭秘程序员最易踩的6个链接陷阱》
 *
 * 编译： gcc -Wall -Wextra -o linker_demo main.c config.c motor.c valve.c
 */
#include "config.h"
#include <stdio.h>

/* 这些函数定义在各自的 .c 中，这里再次声明以便调用 */
void motor_tick(void);
void motor_report(void);
void valve_tick(void);
void valve_report(void);

int main(void)
{
    puts("=== 链接陷阱演示 ===");
    config_init();

    for (int i = 0; i < 3; ++i) { motor_tick(); valve_tick(); }
    motor_report();
    valve_report();

    /* 形参与实参类型匹配：传 uint8_t，不做隐式宽窄转换 */
    uint8_t r = calibrate((uint8_t)0x1);
    printf("calibrate(0x1) = 0x%02X\n", r);
    return 0;
}
