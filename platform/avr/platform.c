/**
 * @file    platform.c
 * @author  Anton Tchekov
 * @version 0.1
 * @date    22.05.2023
 *
 * @brief   Platform specific functions for AVR
 */

#include <platform.h>
#include <serial.h>
#include <lcd.h>
#include <xmem.h>
#include <random.h>
#include <timer.h>

/* --- MEMORY --- */

u32 env_memory_size(void)
{
	return xmem_size();
}

void env_memory_read(u32 addr, void *data, u32 size)
{
	xmem_read(addr, data, size);
}

void env_memory_write(u32 addr, const void *data, u32 size)
{
	xmem_write(addr, data, size);
}

/* --- GRAPHICS --- */

/** Extract red channel from ABGR color */
static inline u8 _abgr_r(u32 color) { return (color >> 24) & 0xFF; }

/** Extract green channel from ABGR color */
static inline u8 _abgr_g(u32 color) { return (color >> 16) & 0xFF; }

/** Extract blue channel from ABGR color */
static inline u8 _abgr_b(u32 color) { return (color >>  8) & 0xFF; }

/** Convert ABGR8888 to RGB565 */
static RGB565 _abgr_to_rgb565(u32 color)
{
	return lcd_color(
		_abgr_r(color),
		_abgr_g(color),
		_abgr_b(color));
}

void env_gfx_rect(i32 x, i32 y, i32 w, i32 h, u32 color)
{
	lcd_rect(x, y, w, h, _abgr_to_rgb565(color));
}

void env_gfx_image_rgba(i32 x, i32 y, i32 w, i32 h, u32 addr)
{
	/*
	xmem_read(args[0], &r, sizeof(r));

	fg = _abgr_to_rgb565(args[2]);
	bg = _abgr_to_rgb565(args[3]);

	lcd_image_rgba(u16 x, u16 y, u16 w, u16 h, u32 addr);
	*/
}

void env_gfx_image_rgb(i32 x, i32 y, i32 w, i32 h, u32 addr)
{
	/*
	lcd_image_rgb(u16 x, u16 y, u16 w, u16 h, u32 addr);
	*/
}

void env_gfx_image_rgb565(i32 x, i32 y, i32 w, i32 h, u32 addr)
{
	/*
	lcd_image_rgb565(u16 x, u16 y, u16 w, u16 h, u32 addr);
	*/
}

void env_gfx_image_grayscale(
	i32 x, i32 y, i32 w, i32 h, u32 fg, u32 bg, u32 addr)
{
	/*
	lcd_image_grayscale(
		u16 x, u16 y, u16 w, u16 h, RGB565 fg, RGB565 bg, u32 addr);
	*/
}

void env_gfx_image_1bit(
	i32 x, i32 y, i32 w, i32 h, u32 fg, u32 bg, u32 addr)
{
	/*
	lcd_image_1bit(
		u16 x, u16 y, u16 w, u16 h, RGB565 fg, RGB565 bg, u32 addr);
	*/
}

/* --- SERIAL --- */

void env_serial_write(const void *data, u32 len)
{
	u32 i;
	const char *data8 = data;
	for(i = 0; i < len; ++i)
	{
		serial_tx(data8[i]);
	}
}

/* --- RANDOM --- */

u32 env_random_get(void)
{
	return random_get();
}

/* --- TIMER --- */

u32 env_millis(void)
{
	return millis();
}
