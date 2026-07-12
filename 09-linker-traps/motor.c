/*
 * static 修饰符：模块私有变量，避免与其他 .c 的同名全局变量冲突（陷阱3）
 */
#include "config.h"
#include <stdio.h>

static uint32_t motor_counter;   /* 本模块专属，不会与 valve.c 的同名符号冲突 */

void motor_tick(void)
{
    motor_counter++;
}

void motor_report(void)
{
    printf("motor_counter = %u (debug_level=%d)\n",
           motor_counter, config_get_debug_level());
}
