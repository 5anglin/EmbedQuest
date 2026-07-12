/*
 * C语言库函数陷阱：库函数调用生存指南（可运行演示）
 * 对应文章：《C语言库函数陷阱：库函数调用生存指南》
 *
 * 编译： gcc -Wall -Wextra -o library_traps library_traps.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/* 1. getchar 返回 int，不能用 char 接收（否则无法正确判断 EOF） */
static void demo_getchar_type(void)
{
    /* 正确： int ch; while ((ch = getchar()) != EOF) ... */
    const char *sample = "AB";           /* 模拟输入流，避免依赖交互 */
    for (const char *s = sample; *s; ++s) {
        int ch = (unsigned char)*s;      /* 用 int 承接 */
        putchar(ch);
    }
    putchar('\n');
    puts("[1] 始终用 int 承接 getchar()，比较 EOF 才安全");
}

/* 2. 顺序文件更新：以 rb+ 打开，先 fseek 定位、fread 验货、再 fwrite */
#pragma pack(push, 1)
typedef struct {
    int    id;
    char   name[16];
    double balance;
} User;
#pragma pack(pop)

static void demo_file_update(const char *path)
{
    /* 先写入 5 个用户 */
    FILE *fp = fopen(path, "wb");
    if (!fp) { perror("[2] fopen w"); return; }
    for (int i = 0; i < 5; ++i) {
        User u = { i, "", 100.0 * (i + 1) };
        snprintf(u.name, sizeof(u.name), "user%d", i);
        fwrite(&u, sizeof(u), 1, fp);
    }
    fclose(fp);

    /* 用 rb+ 精确更新第 4 个用户(index 3) */
    fp = fopen(path, "rb+");
    if (!fp) { perror("[2] fopen rb+"); return; }
    long pos = 3L * (long)sizeof(User);
    if (fseek(fp, pos, SEEK_SET) != 0) { fclose(fp); return; }

    User target;
    if (fread(&target, sizeof(User), 1, fp) != 1) { fclose(fp); return; }
    if (target.balance > 0) {
        target.balance *= 1.05;              /* 计息 */
        fseek(fp, pos, SEEK_SET);            /* 回滚指针再写 */
        fwrite(&target, sizeof(target), 1, fp);
        fflush(fp);
    }
    fclose(fp);
    printf("[2] 已更新 %s 中的第4个用户: %s 余额=%.2f\n",
           path, target.name, target.balance);
}

/* 3. 缓冲区：用 setvbuf 定制，缓冲区生命周期要长于文件流 */
static void demo_buffering(const char *path)
{
    FILE *fp = fopen(path, "w");
    if (!fp) { perror("[3] fopen"); return; }
    char *my_buffer = malloc(8192);
    if (my_buffer) {
        setvbuf(fp, my_buffer, _IOFBF, 8192);
        for (int i = 0; i < 1000; ++i)
            fprintf(fp, "Log entry %d\n", i);
    }
    fclose(fp);        /* 先关闭文件流，再释放缓冲区 */
    free(my_buffer);
    printf("[3] 已用自定义全缓冲写入 %s\n", path);
}

/* 4. errno：调用后立即保存，避免被后续调用覆盖；用 strerror 报告 */
static void demo_errno(void)
{
    FILE *fp = fopen("/definitely/not/exist.txt", "r");
    if (fp == NULL) {
        int saved = errno;                  /* 立即保存 */
        fprintf(stderr, "[4] open 失败: errno=0x%04X, %s\n", saved, strerror(saved));
    } else {
        fclose(fp);
    }
}

int main(void)
{
    puts("=== 库函数陷阱演示 ===");
    demo_getchar_type();
    demo_file_update("users.dat");
    demo_buffering("data.log");
    demo_errno();
    /* 清理演示文件 */
    remove("users.dat");
    remove("data.log");
    return 0;
}
