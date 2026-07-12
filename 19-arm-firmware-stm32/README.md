# 19 真机固件示例：STM32F103C8（Blue Pill）裸机点灯

这是一个**可用 `arm-none-eabi-gcc` 交叉编译、可烧录到真板**的最小裸机工程，
把第 12 章《启动揭秘》和第 13 章《中断处理》里的原理落到真实硬件上。

## 文件
- `startup_stm32f103.c` —— 向量表 + `Reset_Handler`（复制 `.data`、清零 `.bss`、`SystemInit`、跳转 `main`）。
- `system_stm32f103.c` —— `SystemInit`（本例保持复位默认 HSI 8MHz）。
- `main.c` —— 直接操作寄存器让 PC13（板载 LED，低电平点亮）闪烁，不依赖 HAL/CMSIS。
- `STM32F103C8.ld` —— 链接脚本（64KB Flash / 20KB RAM）。
- `Makefile` —— 交叉编译并生成 `.elf/.bin/.hex`。

## 前置工具
```bash
# Ubuntu/Debian
sudo apt install gcc-arm-none-eabi
# 烧录二选一：
sudo apt install stlink-tools      # 提供 st-flash
sudo apt install openocd           # 提供 openocd
```

## 构建
```bash
make          # 生成 blinky.elf / blinky.bin / blinky.hex 并打印 size
```
典型输出（`arm-none-eabi-size`）：`text` 几百字节，`data`/`bss` 极小。

## 烧录
```bash
make flash        # 用 st-flash 写入 0x08000000
# 或
make flash-ocd    # 用 openocd + ST-Link
```

## 移植说明
- 换其它 STM32：改 `STM32F103C8.ld` 的 Flash/RAM 大小、`Makefile` 的 `-mcpu`（如 M4 用 `cortex-m4`）、以及 `main.c` 里的外设寄存器与 LED 引脚。
- 本例为教学用最小实现；产品工程一般直接用厂商 CMSIS 设备头文件与官方 startup/链接脚本。
