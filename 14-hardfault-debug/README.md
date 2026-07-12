# 14 HardFault 破案指南（coredump）

对应文章：《嵌入式调试进阶：从手动到自动的HardFault破案指南》

## 文件
- `hardfault_coredump.c` —— **可在 PC 运行**的轻量级 coredump 生成器仿真：解析 `CFSR` 判断故障类型、打印异常入栈的寄存器快照、（模拟）保存 coredump、无调试器时复位。

## 构建与运行
```bash
make run
```

## 要点
- 别把 `HardFault_Handler` 留成 `while(1)`：生产环境应采集信息并重启。
- 通过 `.map`/`.elf` 符号表做后处理，把地址翻译成函数名+行号。
