/**
 * @file    adc.c
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    16.05.2023
 * @brief   Read analog inputs
 */

/** Channel with noise used for random number generator */
#define ADC_CHANNEL_NOISE 5

/**
 * @brief Read the analog value of a channel
 *
 * @param channel The analog channel to read
 * @return The 10-bit analog value (0-1023)
 */
static u16 adc_read(u8 channel)
{
	ADMUX = (ADMUX & ~(0x1F)) | (channel & 0x1F);
	ADCSRA |= (1 << ADSC);
	while (ADCSRA & (1 << ADSC)) {}
	return ADCW;
}

/**
 * @brief Initialize the ADC module
 */
static void adc_init(void)
{
	ADMUX = (1 << REFS0);
	ADCSRA = (1 << ADPS1) | (1 << ADPS0);
	ADCSRA |= (1 << ADEN);
	adc_read(ADC_CHANNEL_NOISE);
}
