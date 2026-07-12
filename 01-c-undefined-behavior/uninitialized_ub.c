/*
 * 未定义行为演示：未初始化的局部变量与栈"残留"
 * 对应文章：《这段"简单"的C代码，竟然藏着这么多你不知道的秘密！》
 *
 * 说明：本程序演示文章中的经典例子。使用未初始化变量属于"未定义行为(UB)",
 *      其输出依赖编译器、优化等级、平台，任何时候都不应依赖这种行为。
 *      用 gcc -Wall -Wextra 编译时会给出 -Wuninitialized / -Wunused-variable 警告，
 *      这正是文章强调的"重视警告"。
 *
 * 编译： gcc -Wall -Wextra -O0 -o uninitialized_ub uninitialized_ub.c
 */
#include <stdio.h>

/* foo() 在栈上留下一个值为 5 的整数，然后返回 */
static void foo(void)
{
    int a = 5;   /* 有意未使用：制造"栈残留" */
    int b;       /* 有意未初始化 */
    (void)a;
    (void)b;
}

/* bar() 使用一个未初始化的 x —— 它"可能"捡到 foo() 残留的值 */
static void bar(void)
{
    int x;                  /* UB: 未初始化 */
    printf("%d\n", x++);    /* UB: 读取未初始化的值 */
}

int main(void)
{
    puts("=== 未定义行为演示（输出不可依赖）===");
    foo();
    bar();
    bar();

    puts("\n重点：以上输出在不同编译器/优化等级下可能完全不同，");
    puts("     甚至崩溃。切勿在真实代码中依赖未初始化变量。");
    return 0;
}
