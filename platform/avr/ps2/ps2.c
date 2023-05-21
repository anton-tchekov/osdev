/**
 * @file    ps2.c
 * @author  Haron Nazari, Anton Tchekov
 * @version 0.1
 * @date    16.05.2023
 */

#include <ps2.h>
#include <logger.h>
#include <gpio.h>
#include <avr/pgmspace.h>

/** The byte that is currently being received */
static u8 _byte;

/**
 * The number of the current bit:
 *
 *   0: Start bit
 * 1-8: Data bit
 *   9: Parity bit
 *  10: Stop bit
 */
static u8 _bit;

void ps2_init(void)
{
	/* Enable external interrupt 0 (PORTB2) */
	EIMSK |= (1 << PS2_EXT_INTERRUPT);

	/* Trigger interrupt on falling edge */
	EICRA |= (1 << ISC01);

	log_boot_P(LOG_INIT, PSTR("PS/2 keyboard initialized"));
}

/**
 * @brief Get the PS/2 data line state
 *
 * @return 0 if LOW, non-zero if HIGH
 */
static inline u8 _ps2_data_read(void)
{
	/* Read keyboard data pin */
	return PS2_DATA_PORT_IN & (1 << PS2_DATA_PIN);
}

/**
 * @brief Called when a falling edge is detected on the PS/2 clock pin
 */
static inline void _ps2_clock_falling_edge(void)
{
	if(_bit == 0)
	{
		/* Start bit, reset byte */
		_byte = 0;
	}
	else if(_bit >= 1 && _bit <= 8)
	{
		/* Data bit */
		if(_ps2_data_read())
		{
			/* Set bit */
			_byte |= (1 << (_bit - 1));
		}
	}
	else if(_bit == 9)
	{
		/* Ignore parity bit */
		/* TODO: Check parity bit */
		ps2_event(_byte);
	}
	else if(_bit == 10)
	{
		/* Stop bit, reset bit counter */
		_bit = 0;

		/* Return immediately so it stays 0 */
		return;
	}

	++_bit;
}

/** External interrupt */
ISR(INT0_vect)
{
	_ps2_clock_falling_edge();
}
