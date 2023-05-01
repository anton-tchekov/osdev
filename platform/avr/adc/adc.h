/**
 * @file    adc.h
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    01.05.2023
 * @brief   Read Analog values
 */

#ifndef __ADC_H__
#define __ADC_H__

#include <types.h>

/** Channel with noise used for random number generator */
#define ADC_CHANNEL_NOISE 5

/**
 * @brief initialize the hardware channel for noise
 */
void adc_init(void);

/**
 * @brief Read the analog value of channel
 * @param channel the Analog channel to read
 * @return (u16) the analog value
 */
u16 adc_read(u8 channel);

#endif /* __ADC_H__ */