/*
 * 分级日志系统（编译期裁剪）
 * 对应文章：《C语言预处理：嵌入式开发的瑞士军刀》"实战案例：构建灵活的日志系统"
 *
 * 用法：定义 CURRENT_LOG_LEVEL 后包含本头文件，低于该等级的日志会被编译成 (void)0。
 */
#ifndef LOG_H
#define LOG_H

#include <stdio.h>

#define LOG_LEVEL_DEBUG 0
#define LOG_LEVEL_INFO  1
#define LOG_LEVEL_WARN  2
#define LOG_LEVEL_ERROR 3

#ifndef CURRENT_LOG_LEVEL
#define CURRENT_LOG_LEVEL LOG_LEVEL_INFO
#endif

#define LOG_FMT(level, fmt) level " [%s:%s:%d] " fmt, __FILE__, __func__, __LINE__

#if CURRENT_LOG_LEVEL <= LOG_LEVEL_DEBUG
#define LOG_DEBUG(fmt, ...) printf(LOG_FMT("DEBUG", fmt), ##__VA_ARGS__)
#else
#define LOG_DEBUG(fmt, ...) ((void)0)
#endif

#if CURRENT_LOG_LEVEL <= LOG_LEVEL_INFO
#define LOG_INFO(fmt, ...) printf(LOG_FMT("INFO", fmt), ##__VA_ARGS__)
#else
#define LOG_INFO(fmt, ...) ((void)0)
#endif

#if CURRENT_LOG_LEVEL <= LOG_LEVEL_WARN
#define LOG_WARN(fmt, ...) printf(LOG_FMT("WARN", fmt), ##__VA_ARGS__)
#else
#define LOG_WARN(fmt, ...) ((void)0)
#endif

#if CURRENT_LOG_LEVEL <= LOG_LEVEL_ERROR
#define LOG_ERROR(fmt, ...) printf(LOG_FMT("ERROR", fmt), ##__VA_ARGS__)
#else
#define LOG_ERROR(fmt, ...) ((void)0)
#endif

#endif /* LOG_H */
