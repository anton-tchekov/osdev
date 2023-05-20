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
#include <ctype.h>

#define MEMORY_DUMP_WIDTH 16

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

void memory_dump(u32 addr, const void *data, u16 len)
{
	u8 j;
	const u8 *data8;
	u16 i;
	char c, buf[32];

	data8 = data;
	for(i = 0; i < len; i += MEMORY_DUMP_WIDTH, addr += MEMORY_DUMP_WIDTH)
	{
		snprintf(buf, sizeof(buf), "%08X | ", addr);
		serial_tx_str(buf);
		for(j = 0; j < MEMORY_DUMP_WIDTH; ++j)
		{
			snprintf(buf, sizeof(buf), "%02X ", data8[i + j]);
			serial_tx_str(buf);
		}

		serial_tx_str("| ");
		for(j = 0; j < MEMORY_DUMP_WIDTH; ++j)
		{
			c = data8[i + j];
			serial_tx(isprint(c) ? c : '.');
		}

		serial_tx('\n');
	}

	serial_tx('\n');
}
