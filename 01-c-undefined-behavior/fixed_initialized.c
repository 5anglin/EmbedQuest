/*
 * 正确写法：始终初始化局部变量
 * 对应文章：《这段"简单"的C代码……》"重视警告，远离坑"
 *
 * 编译： gcc -Wall -Wextra -Werror -O2 -o fixed_initialized fixed_initialized.c
 * 这份代码可以在 -Werror 下零警告通过。
 */
#include <stdio.h>

static int square_plus_base(int base)
{
    int result = base;   /* 明确初始化 */
    result = result * result + base;
    return result;
}

int main(void)
{
    int values[] = {5, 6, 7};
    for (size_t i = 0; i < sizeof(values) / sizeof(values[0]); ++i) {
        printf("f(%d) = %d\n", values[i], square_plus_base(values[i]));
    }
    return 0;
}
