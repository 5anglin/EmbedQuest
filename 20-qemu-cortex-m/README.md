# 20 QEMU 可运行的 Cortex-M 示例（无需真硬件）

本目录不用真板，直接在 **QEMU 模拟器**里运行一个 Cortex-M3 程序。真板上 LED 靠
GPIO 点亮，这里改用 **ARM 半主机（semihosting）** 把"点灯/熄灯"打印到宿主机终端，
从而能在电脑上"看得见"程序在跑。

## 文件
- `startup.c` —— 向量表 + `Reset_Handler`（复制 `.data`、清零 `.bss`、跳 `main`、`exit`）。
- `main.c` —— 用 `printf`（经 newlib rdimon 半主机）打印 5 次"闪烁"，然后退出。
- `lm3s6965.ld` —— 链接脚本，匹配 QEMU 的 `lm3s6965evb` 内存布局。
- `Makefile` —— 交叉编译 + 一键 `make run` 起 QEMU。

## 前置工具
```bash
# Ubuntu/Debian
sudo apt install gcc-arm-none-eabi qemu-system-arm
# macOS (Homebrew)
brew install --cask gcc-arm-embedded && brew install qemu
```

## 构建并运行
```bash
make          # 生成 firmware.elf
make run      # 在 QEMU 里运行
```
预期输出（打印在终端）：
```
=== Cortex-M blinky in QEMU (semihosting) ===
LED ON   (blink 1)
LED OFF  (blink 1)
...
done
```
程序调用半主机 `SYS_EXIT`，QEMU 会自动退出（无需 Ctrl-A X）。

## 说明
- 选 `lm3s6965evb` 是因为它是 QEMU 对 Cortex-M3 支持最成熟、最稳定的机器之一。
- 若程序卡住不退出，可用 `Ctrl-A` 然后 `X` 强制退出 QEMU。
- 想换成 STM32 真机点灯，见隔壁 [`19-arm-firmware-stm32`](../19-arm-firmware-stm32/)。
