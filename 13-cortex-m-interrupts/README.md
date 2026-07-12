# 13 ARM Cortex-M 中断处理全解析

对应文章：《ARM Cortex-M中断处理全解析》

## 文件
- `platform_mock.h` —— 极简的 UART/中断开关模拟层（对应 CMSIS/LL）。
- `interrupt_demo.c` —— **可在 PC 运行**：模拟串口收到字节触发 ISR，ISR 只入环形缓冲并设标志，主循环处理数据；含临界区保护。

## 构建与运行
```bash
make run
```

## 要点
- ISR 短小、无阻塞：读硬件、清标志、设事件即可。
- 共享变量加 `volatile` 并用临界区（`__disable_irq`/`BASEPRI`/信号量）保护。
