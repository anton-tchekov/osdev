/**
 * @file    time.h
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    22.05.2023
 * @brief   Timing functions
 */

#ifndef __TIME_H__
#define __TIME_H__

#include <types.h>

/**
 * @brief Gets the number of milliseconds since startup
 *
 * @return Number of milliseconds (overflows after approximately 50 days)
 */
u32 millis(void);

#endif /* __TIME_H__ */
