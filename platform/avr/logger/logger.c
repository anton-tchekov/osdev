/**
 * @file    logger.c
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    16.05.2023
 */

#include <logger.h>
#include <serial.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <stdarg.h>
#include <stdio.h>

void log_boot_P(const char *msg, ...)
{
	char buf[128];
	va_list v;

	va_start(v, msg);
	vsnprintf_P(buf, sizeof(buf), msg, v);
	va_end(v);

	serial_tx_str_P(PSTR("[INIT] "));
	serial_tx_str(buf);
	serial_tx('\n');
}

void panic(const char *msg, ...)
{
	char buf[128];
	va_list v;

	va_start(v, msg);
	vsnprintf_P(buf, sizeof(buf), msg, v);
	va_end(v);

	serial_tx_str_P(PSTR("[PANIC] "));
	serial_tx_str(buf);
	serial_tx_str_P(PSTR("\n>>> RESET REQUIRED\n"));

	cli();

	/* Infinite loop */
	for(;;)
	{
	}
}
