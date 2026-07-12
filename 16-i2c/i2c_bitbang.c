/*
 * 软件模拟 I²C 主机：完整复现 START / 地址+R/W / ACK / 数据 /
 * REPEATED START / NACK / STOP 时序（打印每一步），并驱动虚拟从机。
 * 对应文章：《I²C协议详解与实战指南》第2节"完整通信过程"
 */
#include "i2c_bitbang.h"
#include "i2c_slave_sim.h"
#include <stdio.h>

/* 这些"时序原语"在真实硬件上是操纵 SCL/SDA GPIO；这里用打印表达。 */
static void i2c_start(void)         { puts("  START"); }
static void i2c_repeated_start(void){ puts("  REPEATED START"); }
static void i2c_stop(void)          { puts("  STOP"); }
static void i2c_send_byte(const char *what, uint8_t b) { printf("  发送 %s: 0x%02X\n", what, b); }
static void i2c_recv_byte(uint8_t b)                   { printf("  接收数据: 0x%02X\n", b); }
static void i2c_ack(bool ack)       { puts(ack ? "  <- ACK" : "  <- NACK"); }

void i2c_init(void) { puts("[I2C] 初始化 SCL/SDA 为开漏，配置上拉。"); }

bool i2c_write_reg(uint8_t dev_addr, uint8_t reg_addr, const uint8_t *data, uint16_t len)
{
    printf("[I2C 写] 设备0x%02X 寄存器0x%02X 长度%u\n", dev_addr, reg_addr, len);
    i2c_start();
    i2c_send_byte("地址+W", (uint8_t)((dev_addr << 1) | 0));
    bool ok = slave_receive_write(dev_addr, reg_addr, data, len);
    i2c_ack(ok);
    if (!ok) { i2c_stop(); return false; }
    i2c_send_byte("寄存器地址", reg_addr); i2c_ack(true);
    for (uint16_t i = 0; i < len; ++i) { i2c_send_byte("数据", data[i]); i2c_ack(true); }
    i2c_stop();
    return true;
}

bool i2c_read_reg(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint16_t len)
{
    printf("[I2C 读] 设备0x%02X 寄存器0x%02X 长度%u\n", dev_addr, reg_addr, len);
    /* 写阶段：设置寄存器指针，不发 STOP */
    i2c_start();
    i2c_send_byte("地址+W", (uint8_t)((dev_addr << 1) | 0)); i2c_ack(true);
    i2c_send_byte("寄存器地址", reg_addr); i2c_ack(true);
    /* 读阶段：REPEATED START + 地址+R */
    i2c_repeated_start();
    i2c_send_byte("地址+R", (uint8_t)((dev_addr << 1) | 1)); i2c_ack(true);
    bool ok = slave_handle_read(dev_addr, reg_addr, data, len);
    if (!ok) { i2c_stop(); return false; }
    for (uint16_t i = 0; i < len; ++i) {
        i2c_recv_byte(data[i]);
        i2c_ack(i != len - 1);   /* 最后一个字节主机回 NACK */
    }
    i2c_stop();
    return true;
}
