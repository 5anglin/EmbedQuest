/*
 * 调试辅助宏：断言、追踪、二分定位辅助
 * 对应文章：《Debug：玄学还是科学？九大法则助你快速定位软件缺陷》
 *
 * 体现"法则三：眼见为实(观察而非空想)"与"法则四：分而治之"。
 */
#ifndef DEBUG_HELPERS_H
#define DEBUG_HELPERS_H

#include <stdio.h>
#include <stdlib.h>

/* 追踪打印：带文件/行号（法则三：把内部状态"看"出来） */
#define TRACE(fmt, ...) \
    fprintf(stderr, "[TRACE %s:%d] " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)

/* 断言：验证假设，一旦不成立立即暴露（法则七：检查假设） */
#define DBG_ASSERT(expr)                                                      \
    do {                                                                      \
        if (!(expr)) {                                                        \
            fprintf(stderr, "[ASSERT FAILED %s:%d] %s\n",                     \
                    __FILE__, __LINE__, #expr);                               \
            abort();                                                          \
        }                                                                     \
    } while (0)

/* 检查点：法则五"每次只改一处"配合，标记代码是否执行到某处 */
#define CHECKPOINT(name) TRACE("checkpoint: %s", name)

#endif /* DEBUG_HELPERS_H */
