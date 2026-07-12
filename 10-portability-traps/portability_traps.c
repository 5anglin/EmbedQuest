/*
 * C语言可移植性陷阱：跨平台渡劫指南（可运行演示）
 * 对应文章：《C语言可移植性陷阱：嵌入式工程师的跨平台渡劫指南》
 *
 * 编译： gcc -std=c11 -Wall -Wextra -Wconversion -o portability_traps portability_traps.c
 */
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <limits.h>
#include <ctype.h>
#include <assert.h>
#include <stdlib.h>

/* 3. 整数尺寸：用 <stdint.h> 固定位宽，用 static_assert 兜底 */
_Static_assert(sizeof(int32_t) == 4, "int32_t must be 4 bytes");
static void trap_integer_size(void)
{
    int32_t timer_count = 100000;
    printf("[3] timer_count=%" PRId32 ", sizeof(int32_t)=%zu\n",
           timer_count, sizeof(int32_t));
}

/* 4. char 符号性因平台而异：处理二进制一律用 uint8_t */
static void trap_char_signedness(void)
{
    uint8_t raw = 0xFFu;                 /* 无符号，值确定为 255 */
    printf("[4] raw(uint8_t)=%u ; (char)0xFF 在本平台=%d\n",
           raw, (int)(char)0xFF);
}

/* 5. 移位：对无符号类型移位，避免符号位污染/UB */
static void trap_shift(void)
{
    uint32_t value = UINT32_C(1) << 31;  /* 显式无符号 */
    printf("[5] (uint32_t)1<<31 = 0x%08" PRIX32 "\n", value);
}

/* 6. 空指针解引用前必须校验 */
static void trap_null_deref(int *p)
{
    if (p != NULL) {
        *p = 42;
        printf("[6] 已安全写入 *p=%d\n", *p);
    } else {
        puts("[6] 指针为空，拒绝解引用");
    }
}

/* 8. rand()%N 有取模偏差；用拒绝采样得到均匀分布 */
static uint32_t uniform_below(uint32_t n)
{
    uint32_t limit = RAND_MAX - (RAND_MAX % n);
    uint32_t r;
    do { r = (uint32_t)rand(); } while (r >= limit);
    return r % n;
}

/* 9. 大小写转换用 <ctype.h>，不要 c ^ 0x20（对非 ASCII 会出错） */
static void trap_case_convert(void)
{
    char c = 'A';
    if (isupper((unsigned char)c))
        c = (char)tolower((unsigned char)c);
    printf("[9] tolower('A')='%c'\n", c);
}

int main(void)
{
    int v = 0;
    puts("=== 可移植性陷阱演示 ===");
    trap_integer_size();
    trap_char_signedness();
    trap_shift();
    trap_null_deref(&v);
    trap_null_deref(NULL);
    printf("[8] 均匀随机(<100)示例: %u\n", uniform_below(100));
    trap_case_convert();
    return 0;
}
