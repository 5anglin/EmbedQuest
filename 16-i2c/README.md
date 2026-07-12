# 16 I²C 协议详解与实战

对应文章：《I²C协议详解与实战指南》

## 文件（均可在 PC 运行）
- `i2c_bitbang.h` / `i2c_bitbang.c` —— 软件模拟（bit-bang）I²C 主机，完整复现 START / 地址+R/W / ACK / REPEATED START / NACK / STOP 时序。
- `i2c_slave_sim.h` / `i2c_slave_sim.c` —— 虚拟 EEPROM 从设备。
- `main.c` —— 向 0x50 的寄存器 0x03 写 0xAA 再读回验证。

## 构建与运行
```bash
make run     # gcc -o i2c_demo main.c i2c_bitbang.c i2c_slave_sim.c
```

## 要点
- 主机读操作 = 写阶段设寄存器指针 + REPEATED START + 读阶段，最后一字节主机回 NACK。
- 真机上把打印的"时序原语"换成 SCL/SDA 的 GPIO 开漏操作即可。
