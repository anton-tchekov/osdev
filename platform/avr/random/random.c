/* TODO: Documentation */

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
