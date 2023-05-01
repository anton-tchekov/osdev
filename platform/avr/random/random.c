/**
 * @file    random.c
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    01.05.2023
 */

#include <random.h>
#include <adc.h>
#include <stdlib.h>

void random_init(void)
{
	srand(adc_read(ADC_CHANNEL_NOISE));
}

u32 random(void)
{
	return rand();
}
