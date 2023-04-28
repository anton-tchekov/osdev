/**
 * @file    format.h
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    23.04.2023
 * @brief   String formatting
 */

#ifndef __FORMAT_H__
#define __FORMAT_H__

#include <types.h>
#include <stdarg.h>

/**
 * @brief TODO
 *
 * @param str TODO
 * @param format TODO
 * @return TODO
 */
i32 sprintf(char *str, const char *format, ...);

/**
 * @brief TODO
 *
 * @param str TODO
 * @param n TODO
 * @param format TODO
 * @return TODO
 */
i32 snprintf(char *str, u32 n, const char *format, ...);

int vsnprintf(char* str, size_t size, const char* format, va_list arg);

#endif /* __FORMAT_H__ */
