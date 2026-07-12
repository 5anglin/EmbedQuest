# 18 代码质量：五招告别 996

对应文章：《还在996？教你五招，代码质量飞升，告别无休止的 Debug！》

本目录是方法论的**配套配置文件**（无可执行程序），可直接拷进你自己的工程。

## 文件
- `.clang-format` —— 统一代码风格（第四招：代码格式化）。用 `clang-format -i file.c`。
- `.clang-tidy` —— 静态分析规则集（第二招：静态分析）。
- `warnings.mk` —— 推荐的编译器警告集合（第一招：警告一个都不放过）。在你的 Makefile 里 `include warnings.mk`。
- `run_static_analysis.sh` —— 一键跑 cppcheck + clang-tidy。

## 五招速记
1. 编译器 Warning 一个都不放过（`-Wall -Wextra -Wpedantic`，追求零警告）
2. 静态分析（cppcheck / clang-tidy / PVS-Studio …）
3. 控制代码复杂度（函数<100行、圈复杂度<15、嵌套<4层）
4. 自动化代码格式化（clang-format）
5. Code Review（集思广益）
