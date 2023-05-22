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

/** How many bytes to print per line in a hexdump */
#define DUMP_BYTES_PER_LINE   16

/** LCD Log top margin */
#define LOG_Y_OFFSET         120

/** LCD Log left margin */
#define LOG_X_OFFSET          10

/** LCD advance Y direction */
#define LOG_Y_ADVANCE         10

/** LCD Log prefix pixel width */
#define LOG_X_TEXT_OFFSET     49

/** Serial Log prefix width */
#define LOG_START_WIDTH        8

/** Image Logo X offset */
#define LOGO_X_OFFSET         30

/** Image Logo Y Offset */
#define LOGO_Y_OFFSET         50

/** Text Logo X Offset */
#define LOGO_TEXT_X_OFFSET    56

//! @cond Doxygen_Suppress

/** Highlight colors of the various log levels */
const RGB565 _log_color[] PROGMEM =
{
	COLOR_CYAN,
	COLOR_GREEN,
	COLOR_YELLOW,
	COLOR_RED,
	COLOR_MAGENTA
};

/** Log level [INIT] string */
const char _init[] PROGMEM = "INIT";

/** Log level [DEBUG] string */
const char _debug[] PROGMEM = "DEBUG";

/** Log level [WARN] string */
const char _warn[] PROGMEM = "WARN";

/** Log level [ERROR] string */
const char _error[] PROGMEM = "ERROR";

/** Log level [PANIC] string */
const char _panic[] PROGMEM = "PANIC";

/** Array of log level strings */
const char *const _log_msg[] PROGMEM =
{
	_init,
	_debug,
	_warn,
	_error,
	_panic
};

//! @endcond

/** Flag that indicates if the LCD has been initialized */
static bool _lcd_initialized;

/** Current log y position */
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

			if(_lcd_initialized)
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

		if(_lcd_initialized)
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
	for(i = 0; i < len; i += DUMP_BYTES_PER_LINE, addr += DUMP_BYTES_PER_LINE)
	{
		snprintf(buf, sizeof(buf), "%08X | ", addr);
		serial_tx_str(buf);
		for(j = 0; j < DUMP_BYTES_PER_LINE; ++j)
		{
			snprintf(buf, sizeof(buf), "%02X ", data8[i + j]);
			serial_tx_str(buf);
		}

		serial_tx_str("| ");
		for(j = 0; j < DUMP_BYTES_PER_LINE; ++j)
		{
			c = data8[i + j];
			serial_tx(isprint(c) ? c : '.');
		}

		serial_tx('\n');
	}

	serial_tx('\n');
}

void logger_lcd_init(void)
{
	_lcd_initialized = true;

	lcd_logo_P(
		LOGO_X_OFFSET,
		LOGO_Y_OFFSET,
		LOGO_TINY_WIDTH,
		LOGO_TINY_HEIGHT,
		logo_tiny);

	lcd_logo_P(
		LOGO_X_OFFSET + LOGO_TEXT_X_OFFSET,
		LOGO_Y_OFFSET + LOGO_TINY_HEIGHT / 2 - LOGO_TEXT_HEIGHT / 2,
		LOGO_TEXT_WIDTH,
		LOGO_TEXT_HEIGHT,
		logo_text);
}
