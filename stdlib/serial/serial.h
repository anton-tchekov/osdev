/**
 * @file    serial.h
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    23.04.2023
 * @brief   TODO
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
 * @brief TODO
 *
 * @param buf TODO
 * @param n TODO
 * @return TODO
 */
i32 serial_read(const void *buf, i32 n);

#endif /* __SERIAL_H__ */
