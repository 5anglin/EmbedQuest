/*
 * C语言词法暗礁：5大死亡陷阱（可运行演示）
 * 对应文章：《C语言词法暗礁：嵌入式开发者必须绕过的5大死亡陷阱》
 *
 * 每个陷阱都给出"错误写法(注释说明)"与"正确写法(实际运行)"。
 * 编译： gcc -Wall -Wextra -o lexical_traps lexical_traps.c
 */
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define CRITICAL_TEMP 85

/* 陷阱1：= 与 == —— 使用 Yoda 写法把常量放左边，误写成 = 会被编译器报错 */
static void trap_assign_vs_equal(int battery_temp)
{
    /* 错误： while (battery_temp = CRITICAL_TEMP)  // 恒为真 */
    if (CRITICAL_TEMP == battery_temp) {            /* 常量左置 */
        puts("[1] 达到临界温度，触发热失控保护");
    } else {
        puts("[1] 温度正常");
    }
}

/* 陷阱2：位运算 & 误当逻辑 && */
static void trap_bitwise_vs_logical(int limit_a, int limit_b)
{
    /* 错误： if (limit_a & limit_b)  // 位与，语义完全不同 */
    if ((limit_a != 0) && (limit_b != 0)) {
        puts("[2] 两个限位开关都触发 -> 安全联锁");
    } else {
        puts("[2] 未同时触发");
    }
}

/* 陷阱3：相邻运算符的"最大吞噬(maximal munch)"解析。
 *        a 除以 *ptr 若写成没有空格的形式，词法器会把 斜杠+星号 当成注释开头，
 *        因此务必用空格分隔相邻运算符。 */
static void trap_maximal_munch(int a, int *ptr)
{
    /* 错误： if (a/ *ptr == X)  写成 a/‍*ptr 会被解析成注释 */
    if (a / *ptr == 5) {   /* 用空格分隔相邻运算符 */
        puts("[3] a / *ptr == 5");
    } else {
        puts("[3] a / *ptr != 5");
    }
}

/* 陷阱4：整型常量的八进制陷阱 —— 前导0是八进制 */
static void trap_octal_literal(void)
{
    int wrong = 0123;   /* 实际是 83，不是 123 */
    int right = 123;
    printf("[4] 0123(八进制)=%d, 123(十进制)=%d\n", wrong, right);
}

/* 陷阱5：多字符字符常量 vs 字符串 */
static void trap_char_vs_string(void)
{
    /* 错误： set_config('HIGH');  // 'HIGH' 是实现定义的多字符常量(int) */
    const char *config = "HIGH";   /* 用字符串字面量放入 .rodata */
    printf("[5] config = \"%s\", 长度 = %zu\n", config, strlen(config));
}

int main(void)
{
    int x = 2;
    puts("=== C语言词法暗礁演示 ===");
    trap_assign_vs_equal(CRITICAL_TEMP);
    trap_bitwise_vs_logical(1, 2);
    trap_maximal_munch(10, &x);
    trap_octal_literal();
    trap_char_vs_string();
    return 0;
}
