/**
 * @file    serial.c
 * @author  Anton Tchekov
 * @version 0.1
 * @date    16.05.2023
 */

#include "serial.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#define UART_BAUD        9600
#define _BAUD                (((F_CPU / (UART_BAUD * 16UL))) - 1)

void serial_init(void)
{
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
	UBRR0 = _BAUD;
}

void serial_tx(char c)
{
	/* TODO: Use interrupts and output buffer */
	while(!(UCSR0A & (1 << UDRE0))) {}
	UDR0 = c;
}

void serial_tx_str(const char *s)
{
	char c;
	while((c = *s++))
	{
		serial_tx(c);
	}
}

void serial_tx_str_P(const char *s)
{
	char c;
	while((c = pgm_read_byte(s++)))
	{
		serial_tx(c);
	}
}
