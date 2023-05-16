/**
 * @file    adc.h
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    16.05.2023
 * @brief   Read analog inputs
 */

#ifndef __ADC_H__
#define __ADC_H__

#include <types.h>

/** Channel with noise used for random number generator */
#define ADC_CHANNEL_NOISE 5

/**
 * @brief Initialize the ADC module
 */
void adc_init(void);

/**
 * @brief Read the analog value of a channel
 *
 * @param channel The analog channel to read
 * @return The 10-bit analog value (0-1023)
 */
u16 adc_read(u8 channel);

#endif /* __ADC_H__ */
