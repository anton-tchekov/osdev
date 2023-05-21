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

/** Log Level enum */
typedef enum
{
	LOG_INIT,
	LOG_DEBUG,
	LOG_WARN,
	LOG_ERROR,
	LOG_PANIC,
	LOG_OFFSET,
	LOG_NONE,
	LOG_EXT
} LogLevel;

/**
 * @brief Log boot message
 *
 * @param level Log level (see enum above)
 * @param msg Format string
 * @param ... Variable arguments
 */
void log_boot_P(LogLevel level, const char *msg, ...);

/**
 * @brief Trigger a kernel panic
 *
 * @param msg Format string
 * @param ... Variable arguments
 */
void panic(const char *msg, ...);

/**
 * @brief Print a hexdump of a block memory
 *
 * @param addr Start address only for printing
 * @param data Data buffer
 * @param len Buffer size in bytes
 */
void memory_dump(u32 addr, const void *data, u16 len);

#endif /* __LOGGER__ */
