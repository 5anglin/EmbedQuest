/*
 * 软件模拟(bit-bang) I²C 主机接口
 * 对应文章：《I²C协议详解与实战指南》
 */
#ifndef I2C_BITBANG_H
#define I2C_BITBANG_H

#include <stdint.h>
#include <stdbool.h>

void    i2c_init(void);
/* 主机写：向 dev_addr 的 reg_addr 写入 len 字节 */
bool    i2c_write_reg(uint8_t dev_addr, uint8_t reg_addr, const uint8_t *data, uint16_t len);
/* 主机读：从 dev_addr 的 reg_addr 读出 len 字节（含 REPEATED START） */
bool    i2c_read_reg(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint16_t len);

#endif /* I2C_BITBANG_H */
