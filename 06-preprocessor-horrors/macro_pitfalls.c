/*
 * C语言预处理惊魂记：宏的4类陷阱与安全写法
 * 对应文章：《C语言预处理惊魂记：你的宏正在肢解单片机》
 *
 * 编译： gcc -Wall -Wextra -o macro_pitfalls macro_pitfalls.c
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* 1. 参数与整体都加括号（否则 SQUARE(a+1) 展开成 a+1*a+1） */
#define SQUARE(x) ((x) * (x))

/* GCC 语句表达式实现的、对参数只求值一次的 MAX */
#ifdef __GNUC__
#define MAX(a, b) ({ __typeof__(a) _a = (a); __typeof__(b) _b = (b); _a > _b ? _a : _b; })
#else
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif

/* 2. 多语句宏用 do { ... } while(0) 封装，避免悬空 else */
#define SAFE_DELETE(p) do { free((p)); (p) = NULL; } while (0)

/* 3. 带 do/while 的日志宏 */
#define LOG(msg) do { printf("[%s] %s\n", __TIME__, (msg)); } while (0)

/* 4. 用 typedef 而非宏定义指针类型（宏定义 INT_P a,b 只有 a 是指针） */
typedef int *int_ptr_t;

int main(void)
{
    puts("=== 预处理惊魂记：安全宏演示 ===");

    printf("[1] SQUARE(3+1) = %d (正确为16)\n", SQUARE(3 + 1));

    int a = 3, b = 7;
    printf("[1] MAX(3,7) = %d\n", MAX(a, b));

    LOG("system boot");

    /* do-while(0) 宏在 if/else 中也安全 */
    char *buf = malloc(16);
    if (buf)
        SAFE_DELETE(buf);
    else
        puts("alloc failed");
    printf("[2] SAFE_DELETE 后 buf == NULL ? %s\n", buf == NULL ? "yes" : "no");

    int_ptr_t p1, p2;   /* p1、p2 都是指针 */
    a = 10; b = 20;
    p1 = &a; p2 = &b;
    printf("[4] *p1=%d *p2=%d (两者都是指针)\n", *p1, *p2);

    return 0;
}
