/**
 * @file    logger.h
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    16.05.2023
 * @brief   Boot message logger
 */

#ifndef __LOGGER_H__
#define __LOGGER_H__

void log_boot_P(const char *msg, ...);
void panic(const char *msg, ...);

#endif /* __LOGGER__ */
