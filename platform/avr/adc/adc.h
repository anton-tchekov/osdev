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

#define ADC_CHANNEL_NOISE 5

void adc_init(void);

u16 adc_read(u8 channel);

#endif /* __ADC_H__ */