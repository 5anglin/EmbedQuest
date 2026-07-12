/* 虚拟 EEPROM 从设备实现 */
#include "i2c_slave_sim.h"
#include <string.h>

#define EEPROM_SIZE 256
static uint8_t s_addr7 = 0x50;
static uint8_t s_mem[EEPROM_SIZE];

void slave_set_address(uint8_t addr7) { s_addr7 = addr7; }

bool slave_receive_write(uint8_t addr7, uint8_t reg, const uint8_t *data, uint16_t len)
{
    if (addr7 != s_addr7) return false;                 /* 地址不匹配 -> NACK */
    for (uint16_t i = 0; i < len; ++i)
        s_mem[(reg + i) % EEPROM_SIZE] = data[i];
    return true;                                        /* ACK */
}

bool slave_handle_read(uint8_t addr7, uint8_t reg, uint8_t *out, uint16_t len)
{
    if (addr7 != s_addr7) return false;
    for (uint16_t i = 0; i < len; ++i)
        out[i] = s_mem[(reg + i) % EEPROM_SIZE];
    return true;
}
