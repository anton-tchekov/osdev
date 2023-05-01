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
 * @brief Send `n` bytes over the serial interface
 *
 * @param buf Pointer to data buffer
 * @param n Number of bytes to write
 * @return Number of bytes written
 */
i32 serial_write(const void *buf, i32 n);

/**
 * @brief Formatted debug output to serial interface
 *
 * @param format Format string
 */
void debug_print(const char *format, ...);

#endif /* __SERIAL_H__ */
