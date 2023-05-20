/**
 * @file    logger.h
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    16.05.2023
 * @brief   Boot message logger
 */

#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <types.h>

/**
 * @brief TODO
 *
 * @param msg TODO
 * @param ... TODO
 */
void log_boot_P(const char *msg, ...);

/**
 * @brief TODO
 *
 * @param msg TODO
 * @param ... TODO
 */
void panic(const char *msg, ...);

/**
 * @brief TODO
 *
 * @param addr TODO
 * @param data TODO
 * @param len TODO
 */
void memory_dump(u32 addr, const void *data, u16 len);

#endif /* __LOGGER__ */
