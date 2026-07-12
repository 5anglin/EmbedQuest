# 15 嵌入式软件架构的"十八般武艺"

对应文章：《嵌入式软件架构的"十八般武艺"》

## 文件（均可在 PC 运行）
- `01_super_loop.c` —— 超级循环。
- `02_foreground_background.c` —— 前后台系统 + SPSC 无锁环形缓冲区（ADC 采集）。
- `03_event_driven.c` —— 事件驱动（事件队列 + 函数指针分发）。
- `04_layered.c` —— 分层架构（HAL 接口 + 驱动 + 服务层 + 应用层）。
- `05_rtos_freertos.c` + `freertos_mock.h` —— RTOS 多任务架构（FreeRTOS 风格，配 API 桩顺序演示数据流）。

## 构建与运行
```bash
make run
```

## 要点
- 没有万能架构：按实时性、资源、可维护性、团队经验综合选择，常是多种模式组合。
