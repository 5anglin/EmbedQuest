/*
 * 预处理实用宏：字符串化 / 连接（两层展开）
 * 对应文章：《C语言预处理：嵌入式开发的瑞士军刀》
 */
#ifndef PP_UTILS_H
#define PP_UTILS_H

/* 两层宏：先展开参数，再字符串化 */
#define PP_STRINGIFY_IMPL(x) #x
#define PP_STRINGIFY(x)      PP_STRINGIFY_IMPL(x)

/* 两层宏：先展开参数，再连接 */
#define PP_CONCAT_IMPL(x, y) x##y
#define PP_CONCAT(x, y)      PP_CONCAT_IMPL(x, y)

#endif /* PP_UTILS_H */
