/**
 * @file    adc.c
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    16.05.2023
 */

#include <adc.h>
#include <logger.h>
#include <avr/io.h>
#include <avr/pgmspace.h>

void adc_init(void)
{
	ADMUX = (1 << REFS0);
	ADCSRA = (1 << ADPS1) | (1 << ADPS0);
	ADCSRA |= (1 << ADEN);
	adc_read(ADC_CHANNEL_NOISE);
	log_boot_P(LOG_INIT, PSTR("ADC initialized"));
}

u16 adc_read(u8 channel)
{
	ADMUX = (ADMUX & ~(0x1F)) | (channel & 0x1F);
	ADCSRA |= (1 << ADSC);
	while (ADCSRA & (1 << ADSC)) {}
	return ADCW;
}
