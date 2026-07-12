/*
 * 模拟 I²C 从设备（虚拟 EEPROM），供 bit-bang 主机在 PC 上驱动
 */
#ifndef I2C_SLAVE_SIM_H
#define I2C_SLAVE_SIM_H
#include <stdint.h>
#include <stdbool.h>

/* SCL/SDA 线状态（线与逻辑：任意设备拉低即为低）由主机通过这些回调驱动 */
void    slave_set_address(uint8_t addr7);
/* 主机把一个完整的写事务交给从机：reg + data */
bool    slave_receive_write(uint8_t addr7, uint8_t reg, const uint8_t *data, uint16_t len);
/* 主机发起读事务：从机返回数据 */
bool    slave_handle_read(uint8_t addr7, uint8_t reg, uint8_t *out, uint16_t len);

#endif
