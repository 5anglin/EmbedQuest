#include "config.h"
#include <stdio.h>

static uint32_t valve_counter;   /* 与 motor.c 中同名的 counter 互不干扰 */

void valve_tick(void)
{
    valve_counter += 2;
}

void valve_report(void)
{
    printf("valve_counter = %u\n", valve_counter);
}
