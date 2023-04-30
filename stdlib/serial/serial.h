/**
 * @file    serial.h
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    23.04.2023
 * @brief   Serial (UART) interface access
 */

#ifndef __SERIAL_H__
#define __SERIAL_H__

#include <types.h>

/**
 * @brief TODO
 *
 * @param buf TODO
 * @param n TODO
 * @return TODO
 */
i32 serial_write(const void *buf, i32 n);

/**
 * @brief Read up to `n` bytes from the serial interface
 *
 * @param buf Pointer to buffer to
 * @param n Number of bytes to read
 * @return Number of bytes read
 */
i32 serial_read(const void *buf, i32 n);

/**
 * @brief Formatted debug output to serial interface
 *
 * @param format Format string
 */
void debug_print(const char *format, ...);

#endif /* __SERIAL_H__ */
