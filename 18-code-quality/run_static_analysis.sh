#!/usr/bin/env bash
# 一键静态分析脚本（第二招）：cppcheck + clang-tidy
# 对应文章：《还在996？教你五招……》
# 用法： ./run_static_analysis.sh path/to/src
set -euo pipefail
SRC="${1:-.}"

echo "=== cppcheck ==="
if command -v cppcheck >/dev/null 2>&1; then
    cppcheck --enable=all --inconclusive --std=c11 \
             --suppress=missingIncludeSystem "$SRC"
else
    echo "未安装 cppcheck，可用 'apt install cppcheck' 或 'brew install cppcheck'"
fi

echo "=== clang-tidy ==="
if command -v clang-tidy >/dev/null 2>&1; then
    find "$SRC" -name '*.c' -print0 | xargs -0 -I{} clang-tidy {} -- -I"$SRC"
else
    echo "未安装 clang-tidy，可用 LLVM 工具链安装"
fi
