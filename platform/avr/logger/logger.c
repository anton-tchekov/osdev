/**
 * @file    logger.c
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    16.05.2023
 */

#include <logger.h>
#include <serial.h>
#include <lcd.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <stdarg.h>
#include <stdio.h>
#include <ctype.h>

#define MEMORY_DUMP_WIDTH   16

#define LOG_Y_OFFSET       120
#define LOG_X_OFFSET        10
#define LOG_Y_ADVANCE       10
#define LOG_X_TEXT_OFFSET   49
#define LOG_START_WIDTH      8

const RGB565 _log_color[] PROGMEM =
{
	COLOR_CYAN,
	COLOR_GREEN,
	COLOR_YELLOW,
	COLOR_RED,
	COLOR_MAGENTA
};

const char _init[] PROGMEM = "INIT";
const char _debug[] PROGMEM = "DEBUG";
const char _warn[] PROGMEM = "WARN";
const char _error[] PROGMEM = "ERROR";
const char _panic[] PROGMEM = "PANIC";

const char *const _log_msg[] PROGMEM =
{
	_init,
	_debug,
	_warn,
	_error,
	_panic
};

static u16 _log_y = LOG_Y_OFFSET;

static void _lcd_log(LogLevel level, const char *msg)
{
	if(level != LOG_EXT)
	{
		u8 len;
		u16 x;
		const char *level_str;

		if(level != LOG_NONE && level != LOG_OFFSET)
		{
			level_str = (const char *)pgm_read_word(&_log_msg[level]);

			serial_tx('[');
			serial_tx_str_P(level_str);
			serial_tx(']');

			len = 2 + strlen_P(level_str);
			while(len < LOG_START_WIDTH)
			{
				serial_tx(' ');
				++len;
			}

			if(lcd_initialized())
			{
				RGB565 fg;

				fg = pgm_read_word(&_log_color[level]);
				x = LOG_X_OFFSET;
				x += lcd_char(x, _log_y, COLOR_WHITE, COLOR_BLACK, '[');
				x += lcd_string_P(x, _log_y, fg, COLOR_BLACK, level_str);
				lcd_char(x, _log_y, COLOR_WHITE, COLOR_BLACK, ']');
			}
		}
		else if(level == LOG_OFFSET)
		{
			for(len = 0; len < LOG_START_WIDTH; ++len)
			{
				serial_tx(' ');
			}
		}

		if(lcd_initialized())
		{
			const char *s;
			char c;

			x = LOG_X_TEXT_OFFSET;
			if(level == LOG_NONE)
			{
				x = 0;
			}

			s = msg;
			while((c = *s++))
			{
				if(c == '\n')
				{
					x = 0;
					_log_y += LOG_Y_ADVANCE;
				}
				else
				{
					x += lcd_char(LOG_X_OFFSET + x, _log_y,
						COLOR_WHITE, COLOR_BLACK, c);
				}
			}

			_log_y += LOG_Y_ADVANCE;
		}
	}

	serial_tx_str(msg);
}

void log_boot_P(LogLevel level, const char *msg, ...)
{
	char buf[128];
	va_list v;

	va_start(v, msg);
	vsnprintf_P(buf, sizeof(buf), msg, v);
	va_end(v);

	_lcd_log(level, buf);
	serial_tx('\n');
}

void panic(const char *msg, ...)
{
	char buf[128];
	va_list v;

	va_start(v, msg);
	vsnprintf_P(buf, sizeof(buf), msg, v);
	va_end(v);

	_lcd_log(LOG_PANIC, buf);
	serial_tx('\n');
	strcpy_P(buf, PSTR("\n>>> RESET REQUIRED\n"));
	_lcd_log(LOG_NONE, buf);

	/* Disable interrupt */
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
