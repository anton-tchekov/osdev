/**
 * @file    random.c
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    16.05.2023
 */

#include <random.h>
#include <adc.h>
#include <logger.h>
#include <stdlib.h>
#include <avr/pgmspace.h>

static const char _msg_rng[] PROGMEM = "RNG initialized";

/* TODO: Replace srand and rand with own random generator */

void random_init(void)
{
	srand(adc_read(ADC_CHANNEL_NOISE));
	log_boot(_msg_rng);
}

u32 random_get(void)
{
	return rand();
}
