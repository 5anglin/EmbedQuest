# 07 预处理：嵌入式开发的瑞士军刀

对应文章：《C语言预处理：嵌入式开发的瑞士军刀》

## 文件
- `pp_utils.h` —— `PP_STRINGIFY` / `PP_CONCAT`（两层宏，先展开再处理）。
- `log.h` —— 编译期裁剪的分级日志系统（DEBUG/INFO/WARN/ERROR）。
- `main.c` —— 综合演示。

## 构建与运行
```bash
make run
# 只保留 WARN 及以上： gcc -Wall -Wextra -DCURRENT_LOG_LEVEL=2 -o pp_demo main.c
# 查看宏展开：         gcc -E main.c
```
