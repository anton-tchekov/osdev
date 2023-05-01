/**
 * @file    time.h
 * @author  Tim Gabrikowski, Anton YTchekov
 * @version 0.1
 * @date    26.04.2023
 * @brief   Time and Date functions
 */

#ifndef __TIME_H__
#define __TIME_H__

#include <types.h>

/**
 * @brief Gets the current date and time
 *
 * @param now Output parameter DateTime struct (see above)
 */
void datetime_now(DateTime *now);

/**
 * @brief Gets the number of milliseconds since startup
 *
 * @return Number of milliseconds (overflows after approximately 50 days)
 */
u32 millis(void);

#endif /* __TIME_H__ */
