/**
 * @file    timer.c
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    01.05.2023
 * @brief   Millisecond timer module
 */

#include <types.h>

/** Millisecond counter */
static volatile u32 _ms;

/**
 * @brief Initialize hardware timer/counter
 */
static void timer_init(void)
{
	/* Initialize hardware timers */

	/* CTC Mode */
	TCCR0A = (1 << WGM01);

	/* Prescaler 64 */
	TCCR0B |= (1 << CS01) | (1 << CS00);

	/* 16000000 / 64 / 1000 = 250 */
	OCR0A = 250 - 1;

	/* Enable compare interrupt */
	TIMSK0 |= (1 << OCIE0A);

	log_boot_P(LOG_INIT, PSTR("Millisecond timer initialized"));
}

/**
 * @brief Get milliseconds since startup
 *
 * @return Number of milliseconds
 */
static inline u32 millis(void)
{
	/* Return number of milliseconds since boot */
	return _ms;
}

/** Timer0 compare interrupt */
ISR(TIMER0_COMPA_vect)
{
	/* Increment overflows after approx. 50 days */
	++_ms;
}
