/**
 * @file    random.h
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    01.05.2023
 * @brief   Random number generator
 */

#ifndef __RANDOM_H__
#define __RANDOM_H__

#include <types.h>

/**
 * @brief Seeds the random module with hardware noise from the ADC.
 *        `adc_init` must be called before this function.
 */
void random_init(void);

/**
 * @brief Generate a pseudo-random number.
 *
 * @return The number
 */
u32 random_get(void);

#endif /* __RANDOM_H__ */
