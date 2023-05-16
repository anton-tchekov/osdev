/**
 * @file    timer.h
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    01.05.2023
 * @brief   Timer and RTC module
 */

#ifndef __TIMER_H__
#define __TIMER_H__

#include <types.h>

void timer_init(void);
u32 millis(void);

#endif /* __TIMER_H__ */
