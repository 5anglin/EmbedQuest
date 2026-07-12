/*
 * C语言语法暗黑森林：6个语法陷阱（可运行演示）
 * 对应文章：《C语言语法暗黑森林：6个让单片机原地爆炸的语法刺客》
 *
 * 编译： gcc -Wall -Wextra -Wempty-body -o syntax_traps syntax_traps.c
 */
#include <stdio.h>
#include <stdint.h>

/* 1. 复杂函数声明用 typedef 简化 */
typedef void (*ISR_Handler)(void);
static void demo_isr(void) { puts("[1] ISR 被调用"); }
static ISR_Handler get_handler(void) { return demo_isr; }

/* 2. 运算符优先级：~ 与 << ，位运算与逻辑运算都要加括号 */
static void trap_precedence(void)
{
    uint8_t port = 0;
    /* 错误： port |= ~0x01 << 3;  // << 优先级高于 ~ 的直觉容易出错 */
    port |= (uint8_t)(~(0x01u << 3));   /* 明确括号 */
    printf("[2] port = 0x%02X\n", port);
}

/* 3. 分号的死亡之吻：if 后误加分号 */
static void trap_stray_semicolon(int ok)
{
    /* 错误： if (ok);  { do_open(); }  // 分号让 if 变空语句 */
    if (ok) {           /* 大括号 + 无多余分号 */
        puts("[3] 验证通过 -> 开门");
    }
    puts("[3] 记录日志（总是执行）");
}

/* 4. switch 缺少 break 造成 fall-through */
enum { ERROR_OVERVOLT, ERROR_OVERCURRENT, ERROR_UNKNOWN };
static void trap_switch_fallthrough(int code)
{
    switch (code) {
    case ERROR_OVERVOLT:
        puts("[4] 过压 -> shutdown");
        break;                 /* break 是护心镜 */
    case ERROR_OVERCURRENT:
        puts("[4] 过流 -> shutdown");
        break;
    default:
        puts("[4] 未知错误");
        break;
    }
}

/* 5. 可变参数/格式串类型：显式强制类型转换 */
static uint64_t get_sensor_value(void) { return 0x1122334455667788ULL; }
static void trap_format_type(void)
{
    printf("[5] sensor = %llu\n", (unsigned long long)get_sensor_value());
}

/* 6. 悬空 else：else 总是绑定最近的 if，用大括号消除歧义 */
static void trap_dangling_else(int x, int y)
{
    if (x > 0) {
        if (y > 0) {
            puts("[6] x>0 且 y>0 -> led_on");
        }
    } else {
        puts("[6] x<=0 -> led_off");
    }
}

int main(void)
{
    puts("=== 语法暗黑森林演示 ===");
    get_handler()();
    trap_precedence();
    trap_stray_semicolon(1);
    trap_switch_fallthrough(ERROR_OVERCURRENT);
    trap_format_type();
    trap_dangling_else(1, 1);
    trap_dangling_else(-1, 1);
    return 0;
}
