/**
 * @file    util.h
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    29.04.2023
 * @brief   Utility functions
 */

#ifndef __UTIL_H__
#define __UTIL_H__

#include <types.h>

/** Returns the length of an array in elements */
#define ARRLEN(A) (sizeof(A) / sizeof(*(A)))

/**
 * @brief Write a 32-bit little endian value to a buffer
 *
 * @param buf Buffer to write to
 * @param val 32-bit value to write as little endian
 */
void write_le_32(u8 *buf, u32 val);

/**
 * @brief Load a 32-bit little endian value from a buffer
 *
 * @param buf Buffer to read little endian value from
 * @return 32-bit result
 */
u32 load_le_32(u8 *buf);

#endif /* __UTIL_H__ */
