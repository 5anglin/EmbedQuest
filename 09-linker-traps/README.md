# 09 链接：6个链接陷阱（多文件工程）

对应文章：《C语言链接：揭秘程序员最易踩的6个链接陷阱》

## 文件
- `config.h` / `config.c` —— 头文件只做 `extern` 声明，变量唯一定义放 `.c`。
- `motor.c` / `valve.c` —— `static` 模块私有变量，避免同名符号冲突。
- `main.c` —— 主程序。

## 构建与运行
```bash
make run     # gcc -o linker_demo main.c config.c motor.c valve.c
```

## 要点
- 声明（`extern`）与定义分离，防止"重复定义"链接错误。
- 模块私有用 `static`；头文件加 `#ifndef` 防护罩；函数原型带完整参数类型。
