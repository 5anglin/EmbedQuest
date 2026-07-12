/*
 * C语言语义地雷（上）：指针/数组/内存的5大陷阱
 * 对应文章：《C语言语义地雷：'语义刺客'的硬件谋杀实录①》
 *
 * 编译： gcc -Wall -Wextra -o semantic_traps_1 semantic_traps_1.c
 */
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

/* 1. 指针 vs 数组：字符串字面量只读，不能修改 */
static void trap_string_literal(void)
{
    /* 错误： char *p = "hello"; p[3] = 'a';  // 修改只读区 -> 崩溃 */
    char p[] = "hello";   /* 用字符数组，内容可写 */
    p[3] = 'a';
    printf("[1] 修改后的字符数组: %s\n", p);
}

/* 2. 野指针：指针诞生时不绑定内存 */
static void trap_wild_pointer(void)
{
    /* 错误： char *ptr; printf("%s", ptr);  // 野指针 */
    static char comm_buf[128];
    char *ptr = comm_buf;               /* 绑定到静态缓冲区 */
    memset(ptr, 0, sizeof(comm_buf));   /* 清场 */
    strcpy(ptr, "safe buffer");
    printf("[2] ptr -> \"%s\"\n", ptr);
}

/* 3. 数组退化为指针：函数参数里 sizeof 得到的是指针大小 */
static void dma_config(const uint8_t *data_buf, size_t buf_size)
{
    /* 显式传入长度，避免 sizeof(data_buf) 得到指针大小 */
    assert(buf_size <= 256);
    printf("[3] DMA 将搬运 %zu 字节\n", buf_size);
    (void)data_buf;
}

/* 4. memset(ptr, 0, sizeof(ptr)) 只清了指针长度 */
struct Key {
    uint32_t id;
    uint8_t  type;
};
static void reset_key(struct Key *k)
{
    memset(k, 0, sizeof(struct Key));   /* 用类型求真实大小 */
}

/* 5. 非空指针 != 空字符串；malloc 可能失败 */
static void trap_alloc_check(void)
{
    char *log_buf = calloc(64, 1);
    if (log_buf != NULL) {
        strncpy(log_buf, "Engine Start", 63);
        log_buf[63] = '\0';
        printf("[5] 日志: %s\n", log_buf);
        free(log_buf);
    } else {
        puts("[5] 内存分配失败，进入安全停机");
    }
}

int main(void)
{
    uint8_t sensor_data[128];
    struct Key key = { 0x1234, 7 };

    puts("=== 语义地雷（上）演示 ===");
    trap_string_literal();
    trap_wild_pointer();
    dma_config(sensor_data, sizeof(sensor_data));   /* 正确传长度 */
    reset_key(&key);
    printf("[4] reset 后 key.id=%u key.type=%u\n", key.id, key.type);
    trap_alloc_check();
    printf("    ARRAY_SIZE(sensor_data) = %zu\n", ARRAY_SIZE(sensor_data));
    return 0;
}
