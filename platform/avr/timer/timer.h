/**
 * @file    timer.h
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    01.05.2023
 * @brief   Timer module
 */

#ifndef __TIMER_H__
#define __TIMER_H__

#include <types.h>

/**
 * @brief Initialize hardware timer/counter
 */
void timer_init(void);

/**
 * @brief Get milliseconds since startup
 *
 * @return Number of milliseconds
 */
u32 millis(void);

#endif /* __TIMER_H__ */
