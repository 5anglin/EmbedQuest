/*
 * 正确的头文件：只做"声明"，用 include guard 防重复包含
 * 对应文章：《C语言链接：揭秘程序员最易踩的6个链接陷阱》
 *
 * 关键点：
 *  - 头文件里用 extern 声明全局变量（陷阱2），不要在此定义变量（会多重定义）
 *  - 函数原型带完整参数类型（陷阱4）
 *  - #ifndef 防护罩（陷阱6）
 */
#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* 声明：只说存在，不占地盘 */
extern int debug_level;

/* 函数原型：参数类型写全，即使是 void */
void   config_init(void);
int    config_get_debug_level(void);
uint8_t calibrate(uint8_t mode);   /* 形参类型明确，避免整数截断意外 */

#ifdef __cplusplus
}
#endif

#endif /* CONFIG_H */
