/*
 * 预处理"瑞士军刀"综合演示
 * 对应文章：《C语言预处理：嵌入式开发的瑞士军刀》
 *
 * 编译（默认 INFO 等级）： gcc -Wall -Wextra -o pp_demo main.c
 * 打开 DEBUG 日志：       gcc -Wall -Wextra -DCURRENT_LOG_LEVEL=0 -o pp_demo main.c
 * 只运行预处理器查看展开： gcc -E main.c
 */
/* 演示时默认打开全部日志；命令行 -DCURRENT_LOG_LEVEL=N 可覆盖 */
#ifndef CURRENT_LOG_LEVEL
#define CURRENT_LOG_LEVEL 0
#endif
#include "log.h"
#include "pp_utils.h"
#include <stdio.h>

#define VERSION_MAJOR 1
#define VERSION_MINOR 4

int main(void)
{
    /* 相邻字符串自动拼接 */
    const char *banner = "Embedded" " " "C" " Cookbook";
    printf("banner: %s\n", banner);

    /* 字符串化：先展开 VERSION_* 再拼接 */
    printf("version: %s.%s\n", PP_STRINGIFY(VERSION_MAJOR), PP_STRINGIFY(VERSION_MINOR));

    /* 连接：生成变量名 config_1 */
    int PP_CONCAT(config_, 1) = 42;
    printf("config_1 = %d\n", config_1);

    /* 分级日志 */
    LOG_DEBUG("这是调试信息 value=%d\n", 100);
    LOG_INFO("系统已启动\n");
    LOG_WARN("电压偏低\n");
    LOG_ERROR("传感器无响应\n");

    return 0;
}
