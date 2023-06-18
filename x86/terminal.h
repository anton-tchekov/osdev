#ifndef __TERMINAL_H__
#define __TERMINAL_H__

#include "types.h"

typedef enum
{
	TERMINAL_COLOR_BLACK,
	TERMINAL_COLOR_BLUE,
	TERMINAL_COLOR_GREEN,
	TERMINAL_COLOR_CYAN,
	TERMINAL_COLOR_RED,
	TERMINAL_COLOR_MAGENTA,
	TERMINAL_COLOR_BROWN,
	TERMINAL_COLOR_LIGHT_GREY,
	TERMINAL_COLOR_DARK_GREY,
	TERMINAL_COLOR_LIGHT_BLUE,
	TERMINAL_COLOR_LIGHT_GREEN,
	TERMINAL_COLOR_LIGHT_CYAN,
	TERMINAL_COLOR_LIGHT_RED,
	TERMINAL_COLOR_LIGHT_MAGENTA,
	TERMINAL_COLOR_LIGHT_BROWN,
	TERMINAL_COLOR_WHITE
} TerminalColor;

void terminal_init(void);
void terminal_set_color(TerminalColor fg, TerminalColor bg);
void terminal_put(char c, TerminalColor fg, TerminalColor bg, u32 x, u32 y);
void terminal_char(char c);
void terminal_string(const char *str);

#endif /* __TERMINAL_H__ */
