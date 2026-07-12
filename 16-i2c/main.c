/*
 * I²C 演示：主机向虚拟 EEPROM(地址0x50) 的寄存器 0x03 写 0xAA，再读回验证。
 * 对应文章：《I²C协议详解与实战指南》
 *
 * 编译： gcc -Wall -Wextra -o i2c_demo main.c i2c_bitbang.c i2c_slave_sim.c
 */
#include "i2c_bitbang.h"
#include "i2c_slave_sim.h"
#include <stdio.h>

int main(void)
{
    puts("=== I²C 通信演示（bit-bang 主机 + 虚拟EEPROM）===\n");
    i2c_init();
    slave_set_address(0x50);

    uint8_t wr = 0xAA;
    i2c_write_reg(0x50, 0x03, &wr, 1);

    puts("");
    uint8_t rd = 0;
    i2c_read_reg(0x50, 0x03, &rd, 1);

    printf("\n结果: 写入 0x%02X, 读回 0x%02X -> %s\n",
           wr, rd, wr == rd ? "一致 ✓" : "不一致 ✗");
    return 0;
}
