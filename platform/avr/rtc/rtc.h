/**
 * @file    rtc.c
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    16.05.2023
 * @brief   Real time clock module
 */

#ifndef __RTC_H__
#define __RTC_H__

#include <types.h>

/**
 * @brief Initialize real time clock
 */
void rtc_init(void);

/**
 * @brief Get the current date and time from the real time clock
 *
 * @param now Output parameter
 */
DateTime *datetime_now(DateTime *now);

#endif /* __RTC_H__ */
