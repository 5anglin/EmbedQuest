# 12 ARM Cortex-M 启动揭秘

对应文章：《ARM Cortex-M启动揭秘：从芯片上电那一刻到你的main()函数》

## 文件
- `startup_sim.c` —— **可在 PC 运行**的启动流程仿真：复制 `.data`、清零 `.bss`、`SystemInit`、`__libc_init_array`、跳转 `main`，按真实顺序打印。
- `vector_table.c` —— 中断向量表参考实现（面向 `arm-none-eabi-gcc`，**不在 PC 编译**）。
- `gcc_linker_script.ld` —— 链接脚本参考（定义 `_sidata/_sdata/_edata/_sbss/_ebss` 等符号）。

## 构建与运行
```bash
make run     # 仅构建 startup_sim
```

## 要点
- `.data`/`.bss` 的初始化必须在任何其他函数调用之前完成。
- 向量表第 0 项是栈顶 MSP，第 1 项是 `Reset_Handler`。
