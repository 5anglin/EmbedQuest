/*
 * "分而治之"实战：用二分查找定位数组中第一个坏元素（法则四）
 * 对应文章：《Debug：玄学还是科学？九大法则……》
 *
 * 编译： gcc -Wall -Wextra -o bisect_demo bisect_demo.c
 */
#include "debug_helpers.h"
#include <stdbool.h>

/* 假设：某工序后，数据数组本应全部 <= 阈值；出现一个越界值就是 bug。
 * 用二分把"最后一次正常"和"第一次异常"的边界逼出来。 */
static bool value_ok(int v) { return v <= 100; }

static int find_first_bad(const int *arr, int n)
{
    int lo = 0, hi = n;         /* [lo, hi) 中定位第一个坏元素 */
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        CHECKPOINT("bisect step");
        TRACE("检查区间 [%d,%d), mid=%d, 值=%d", lo, hi, mid, arr[mid]);
        if (value_ok(arr[mid]))
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;                  /* == n 表示没有坏元素 */
}

int main(void)
{
    int data[] = { 10, 20, 30, 40, 50, 60, 150, 200, 250 };  /* 从下标07起均越界 */
    int n = (int)(sizeof(data) / sizeof(data[0]));

    puts("=== 分而治之：二分定位第一个越界值 ===");
    int idx = find_first_bad(data, n);
    DBG_ASSERT(idx >= 0 && idx <= n);
    if (idx < n)
        printf("第一个坏元素在下标 %d，值 = %d\n", idx, data[idx]);
    else
        puts("未发现坏元素");
    return 0;
}
