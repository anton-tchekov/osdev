#include "terminal.h"
#include "stdlib.h"
#include "io.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_SIZE (VGA_HEIGHT * VGA_WIDTH)
#define VGA_ADDR 0xB8000

static u32 _terminal_x, _terminal_y;
static TerminalColor _terminal_fg, _terminal_bg;
static u16 *_terminal_buffer;

static inline u16 vga_entry(char c, u8 color)
{
	return (u16)c | (u16)color << 8;
}

static inline u8 vga_color(TerminalColor fg, TerminalColor bg)
{
	return fg | bg << 4;
}

static void cursor_enable(u8 cursor_start, u8 cursor_end)
{
	outb(0x3D4, 0x0A);
	outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);

	outb(0x3D4, 0x0B);
	outb(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end);
}

static void cursor_move(int x, int y)
{
	uint16_t pos = y * VGA_WIDTH + x;

	outb(0x3D4, 0x0F);
	outb(0x3D5, (u8)(pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (u8)((pos >> 8) & 0xFF));
}

static void cursor_disable(void)
{
	outb(0x3D4, 0x0A);
	outb(0x3D5, 0x20);
}

void terminal_init(void)
{
	_terminal_fg = TERMINAL_COLOR_LIGHT_GREY;
	_terminal_bg = TERMINAL_COLOR_BLACK;
	_terminal_buffer = (u16 *)VGA_ADDR;
	memset16(_terminal_buffer,
			 vga_entry(' ', vga_color(_terminal_fg, _terminal_bg)),
			 VGA_SIZE);

	cursor_enable(0, 15);
}

void terminal_set_color(TerminalColor fg, TerminalColor bg)
{
	_terminal_fg = fg;
	_terminal_bg = bg;
}

void terminal_put(char c, TerminalColor fg, TerminalColor bg, u32 x, u32 y)
{
	_terminal_buffer[y * VGA_WIDTH + x] = vga_entry(c, vga_color(fg, bg));
}

void terminal_char(char c)
{
	if (c == '\n')
	{
		_terminal_x = 0;
		++_terminal_y;
	}
	else
	{
		terminal_put(c, _terminal_fg, _terminal_bg, _terminal_x, _terminal_y);
		if (++_terminal_x == VGA_WIDTH)
		{
			_terminal_x = 0;
			++_terminal_y;
		}
	}

	if (_terminal_y >= VGA_HEIGHT)
	{
		/* Scroll Up */
		memmove(_terminal_buffer, _terminal_buffer + VGA_WIDTH,
				(VGA_SIZE - VGA_WIDTH) * sizeof(*_terminal_buffer));

		/* Clear last line */
		memset16(_terminal_buffer + VGA_SIZE - VGA_WIDTH,
				 vga_entry(' ', vga_color(_terminal_fg, _terminal_bg)), VGA_WIDTH);

		_terminal_y = VGA_HEIGHT - 1;
	}

	cursor_move(_terminal_x, _terminal_y);
}

void terminal_string(const char *str)
{
	char c;
	while ((c = *str++))
	{
		terminal_char(c);
	}
}
