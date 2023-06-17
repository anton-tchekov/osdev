/**
 * @file    random.c
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    16.05.2023
 * @brief   Random number generator
 */

static void random_init(void)
{
	srand(adc_read(ADC_CHANNEL_NOISE));
}

static u16 random_get(void)
{
	return rand();
}
