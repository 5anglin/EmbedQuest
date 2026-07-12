/*
 * C语言语义地雷（下）：边界/求值顺序/短路/溢出/main返回值
 * 对应文章：《C语言语义地雷：'语义刺客'的硬件谋杀实录②》
 *
 * 编译： gcc -Wall -Wextra -o semantic_traps_2 semantic_traps_2.c
 */
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <stdlib.h>

#define BUFFER_SIZE 10

/* 6. 差一错误：使用 < 而不是 <=，并用 ARRAY_SIZE */
static void trap_off_by_one(void)
{
    int buffer[BUFFER_SIZE];
    for (size_t i = 0; i < sizeof(buffer) / sizeof(buffer[0]); ++i) {
        buffer[i] = (int)i * 2;
    }
    printf("[6] buffer[0]=%d buffer[%d]=%d (无越界)\n",
           buffer[0], BUFFER_SIZE - 1, buffer[BUFFER_SIZE - 1]);
}

/* 7. 求值顺序未定义：不要在同一表达式里既读又写同一变量 */
static uint32_t g_adc = 100;
static uint32_t get_adc(void)      { return g_adc; }
static uint32_t get_next_adc(void) { return ++g_adc; }
static void trap_eval_order(void)
{
    /* 错误： printf("%u %u", adc, adc = get_next_adc());  // 顺序未定义 */
    uint32_t current = get_adc();
    uint32_t next = get_next_adc();     /* 显式解耦 */
    printf("[7] current=%u next=%u\n", current, next);
}

/* 8. 短路求值：先判空再解引用 */
static void trap_short_circuit(int *p)
{
    /* 错误： if (*p > 10 && p != NULL)  // 先解引用后判空 */
    if (p != NULL && *p > 10) {         /* 正确顺序 */
        puts("[8] *p > 10");
    } else {
        puts("[8] 指针为空或 *p <= 10");
    }
}

/* 9. 整数溢出：先用更宽类型计算再检查范围 */
static void trap_overflow(void)
{
    /* 错误： uint16_t t = 50000 * 2;  // 溢出为 34464 */
    uint32_t temp = (uint32_t)50000 * 2;
    assert(temp <= UINT16_MAX || temp > UINT16_MAX);  /* 演示：检测到超范围 */
    if (temp > UINT16_MAX) {
        printf("[9] 检测到溢出: %u 超过 uint16_t 上限 %u，改用 uint32_t 存储\n",
               temp, (unsigned)UINT16_MAX);
    } else {
        uint16_t torque = (uint16_t)temp;
        printf("[9] torque=%u\n", torque);
    }
}

int main(void)
{
    int v = 42;
    puts("=== 语义地雷（下）演示 ===");
    trap_off_by_one();
    trap_eval_order();
    trap_short_circuit(&v);
    trap_short_circuit(NULL);
    trap_overflow();
    /* 10. main 一定要有明确的 return，告知操作系统退出码 */
    return EXIT_SUCCESS;
}
