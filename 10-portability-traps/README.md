# 10 可移植性陷阱：跨平台渡劫指南

对应文章：《C语言可移植性陷阱：嵌入式工程师的跨平台渡劫指南》

## 文件
- `portability_traps.c` —— 用 `<stdint.h>`、`static_assert`、拒绝采样等写出可移植代码。

## 构建与运行
```bash
make run     # 建议再加 -Wconversion 观察隐式转换
```

## 覆盖的陷阱
- 固定位宽整型、`char` 符号性、无符号移位、空指针校验、`rand()%N` 取模偏差、`<ctype.h>` 大小写转换。
