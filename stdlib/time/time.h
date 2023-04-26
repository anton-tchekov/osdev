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

/** DateTime struct containing date and time info */
typedef struct
{
	/** Year, for example 2023 */
	i32 Year;

	/** Month, value from 1-12 */
	i32 Month;

	/** Day, value from 1-31 */
	i32 Day;

	/** Hour, value from 0-23 */
	i32 Hour;

	/** Minute, value from 0-59 */
	i32 Minute;

	/** Second, value from 0-59 */
	i32 Second;
} DateTime;

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
