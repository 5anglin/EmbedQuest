/* 唯一定义：debug_level 只在这里"安家"（对应头文件的 extern 声明） */
#include "config.h"

int debug_level = 5;   /* 定义 */

void config_init(void)
{
    debug_level = 3;
}

int config_get_debug_level(void)
{
    return debug_level;
}

uint8_t calibrate(uint8_t mode)
{
    return (uint8_t)(mode & 0xFFu);
}
