# 嵌入式 C 语言实战代码库（Embedded C Cookbook）

![CI](https://github.com/volo-me/EmbedQuest/actions/workflows/ci.yml/badge.svg)

本仓库将公众号文章中出现的代码片段**补全为完整、可编译、可运行的示例**，并按文章分目录组织，方便对照文章学习与二次开发。

> 原始文章（Markdown）全部保留在 [`docs/`](docs/) 目录下，便于对照阅读。

## 特点

- **可编译可运行**：所有纯 C 示例都能用 `gcc -Wall -Wextra` 干净编译并运行。
- **硬件示例可仿真**：涉及 STM32 寄存器、NVIC、UART、I²C、FreeRTOS 的示例，都配了一层极简的 **PC 端模拟层（mock）**，让底层逻辑可以在电脑上直接跑起来观察行为，而不必烧录真机。
- **一键构建**：顶层 `Makefile` 递归构建 / 运行 / 清理全部示例。

## 快速开始

```bash
# 构建全部示例
make

# 构建并依次运行全部示例
make run

# 清理所有产物
make clean
```

单独构建某一章：

```bash
cd 11-fsm
make run
```

> 依赖：`gcc`（或 `clang`）与 `make`。若要跑真机固件示例（启动文件、链接脚本），需另装 `arm-none-eabi-gcc` 工具链——相关文件已在注释中标明"仅作参考，不在 PC 上编译"。

## 目录索引

| 目录 | 对应文章 | 内容 |
|------|----------|------|
| [`01-c-undefined-behavior`](01-c-undefined-behavior/) | 这段"简单"的C代码，竟然藏着这么多秘密 | 未初始化变量 / 栈残留 / 未定义行为 |
| [`02-lexical-traps`](02-lexical-traps/) | C语言词法暗礁：5大死亡陷阱 | `=`vs`==`、位运算vs逻辑、最大吞噬、八进制、多字符常量 |
| [`03-semantic-traps-1`](03-semantic-traps-1/) | C语言语义地雷① | 指针/数组、野指针、数组退化、sizeof、malloc 检查 |
| [`04-semantic-traps-2`](04-semantic-traps-2/) | C语言语义地雷② | 差一错误、求值顺序、短路、整数溢出、main 返回值 |
| [`05-syntax-traps`](05-syntax-traps/) | C语言语法暗黑森林：6个语法刺客 | 复杂声明、优先级、悬空分号、switch、格式串、悬空else |
| [`06-preprocessor-horrors`](06-preprocessor-horrors/) | C语言预处理惊魂记 | 宏参数括号、do-while(0)、宏类型陷阱 |
| [`07-preprocessor-toolkit`](07-preprocessor-toolkit/) | C语言预处理：嵌入式瑞士军刀 | 字符串化/连接/两层宏、分级日志系统 |
| [`08-library-traps`](08-library-traps/) | C语言库函数陷阱 | getchar返回int、rb+文件更新、setvbuf、errno |
| [`09-linker-traps`](09-linker-traps/) | C语言链接：6个链接陷阱 | 声明vs定义、static、头文件防护（多文件工程） |
| [`10-portability-traps`](10-portability-traps/) | C语言可移植性陷阱 | stdint、char符号性、移位、空指针、取模偏差、大小写 |
| [`11-fsm`](11-fsm/) | 万物皆可状态机 | switch-case / 函数指针表 / LED 状态机三种实现 |
| [`12-cortex-m-startup`](12-cortex-m-startup/) | ARM Cortex-M 启动揭秘 | 启动流程仿真 + 向量表 + 链接脚本参考 |
| [`13-cortex-m-interrupts`](13-cortex-m-interrupts/) | ARM Cortex-M 中断处理全解析 | ISR设标志+主循环处理+临界区（UART 仿真） |
| [`14-hardfault-debug`](14-hardfault-debug/) | HardFault 破案指南 | coredump 生成器：解析 CFSR、寄存器快照、重启 |
| [`15-embedded-architectures`](15-embedded-architectures/) | 嵌入式软件架构十八般武艺 | 超级循环/前后台/事件驱动/分层/RTOS 五种架构 |
| [`16-i2c`](16-i2c/) | I²C 协议详解与实战 | bit-bang 主机 + 虚拟 EEPROM 从机（完整时序） |
| [`17-debug-nine-rules`](17-debug-nine-rules/) | Debug 九大法则 | 调试辅助宏 + "分而治之"二分定位实战 |
| [`18-code-quality`](18-code-quality/) | 还在996？五招提升代码质量 | .clang-format / .clang-tidy / 警告集 / 静态分析脚本 |
| [`19-arm-firmware-stm32`](19-arm-firmware-stm32/) | 启动揭秘 + 中断处理（真机落地） | **arm-none-eabi-gcc** 交叉编译的 STM32F103 裸机点灯固件 |
| [`20-qemu-cortex-m`](20-qemu-cortex-m/) | 启动揭秘（模拟器落地） | **QEMU** 可直接运行的 Cortex-M3 半主机示例，无需真硬件 |

## 关于硬件示例的"模拟层"

真机代码（如 `USART1->DR`、`NVIC_SetPriority`、`xQueueSend`）无法在 PC 上直接运行。本仓库的做法是：**保留与文章一致的上层逻辑**，把最底层的寄存器读写/RTOS API 替换成打印或内存模拟。这样你既能在电脑上跑通、观察数据流，又能清楚看到移植到真机时需要替换的边界在哪里（通常就是各目录下的 `*_mock.h` / `platform_mock.h` / `freertos_mock.h`）。

## 持续集成（GitHub Actions）

`.github/workflows/ci.yml` 定义了三个 job，推送/PR 时自动运行：

1. **build-and-run** —— 用 `gcc` 执行 `make all` + `make run`，保证全部主机示例可编译可运行。
2. **static-analysis** —— `cppcheck`（硬门禁，配 `.cppcheck-suppressions` 放行教学用的故意错误）+ `clang-tidy`（信息性，不阻断）。
3. **build-firmware** —— 安装 `gcc-arm-none-eabi`，交叉编译 `19-arm-firmware-stm32` 固件，并把 `.elf/.bin/.hex` 作为构建产物上传。

## 真机固件（arm-none-eabi-gcc）

`19-arm-firmware-stm32/` 是一个可烧录到 STM32F103C8（Blue Pill）的最小裸机工程，把第 12/13 章的启动与寄存器原理落到真板：

```bash
sudo apt install gcc-arm-none-eabi
make -C 19-arm-firmware-stm32          # 生成 blinky.elf/.bin/.hex
make -C 19-arm-firmware-stm32 flash    # 用 st-flash 烧录（需 stlink-tools）
```

> 顶层 `make` 只构建主机示例，不含固件（避免没装 ARM 工具链的机器构建失败）；固件请单独 `make -C 19-arm-firmware-stm32`。

## 用模拟器本地运行（QEMU，无需真板）

`20-qemu-cortex-m/` 是一个可在 **QEMU** 里直接跑的 Cortex-M3 程序，用 ARM 半主机把输出打到终端：

```bash
sudo apt install gcc-arm-none-eabi qemu-system-arm   # macOS: brew install qemu + gcc-arm-embedded
make -C 20-qemu-cortex-m run
```

会看到 5 次"LED ON/OFF"打印后自动退出——完全不用真硬件即可验证启动流程与运行逻辑。

## 许可证

本仓库代码以 [MIT License](LICENSE) 开源。原文文章版权归原作者所有，`docs/` 目录仅用于对照学习。
