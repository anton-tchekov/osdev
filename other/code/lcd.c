
void lcd_image_rgb565(u16 x, u16 y, u16 w, u16 h, u32 addr)
{
	u16 x0, y0, stride;
	u16 *ci, image[LCD_WIDTH];

	stride = w * sizeof(RGB565);
	for(y0 = y; y0 < y + h; ++y0)
	{
		xmem_read(addr, image, stride);
		_lcd_window_start(x, y0, w, 1);
		ci = image;
		for(x0 = x; x0 < x + w; ++x0)
		{
			_lcd_pixel(*ci++);
		}

		_lcd_window_end();
		addr += stride;
	}
}

/**
 * @brief Mix two colors according to a ratio. A ratio of 255 means 100% of
 *        the first color and 0% of the second color will be mixed together.
 *
 * @param color1 First ABGR color
 * @param color2 Second ABGR color
 * @param ratio Merge ratio from 0-255
 * @return Merged RGB565 color value
 */
static RGB565 _color_merge(u32 color1, u32 color2, u16 ratio)
{
	u8 r1, g1, b1, r2, g2, b2;

	r1 = _abgr_r(color1);
	g1 = _abgr_g(color1);
	b1 = _abgr_b(color1);

	r2 = _abgr_r(color2);
	g2 = _abgr_g(color2);
	b2 = _abgr_b(color2);

	return lcd_color(
		(r1 * ratio + r2 * (255 - ratio)) / 255,
		(g1 * ratio + g2 * (255 - ratio)) / 255,
		(b1 * ratio + b2 * (255 - ratio)) / 255);
}

void lcd_image_grayscale(
	u16 x, u16 y, u16 w, u16 h, u32 addr, u32 fg, u32 bg)
{
	u16 x0, y0;
	u8 *ci, image[LCD_WIDTH];

	for(y0 = y; y0 < y + h; ++y0)
	{
		xmem_read(addr, image, w);
		_lcd_window_start(x, y0, w, 1);
		ci = image;
		for(x0 = x; x0 < x + w; ++x0)
		{
			_lcd_pixel(_color_merge(fg, bg, *ci++));
		}

		_lcd_window_end();
		addr += w;
	}
}

void lcd_image_1bit(
	u16 x, u16 y, u16 w, u16 h, u32 addr, RGB565 fg, RGB565 bg)
{
	u8 byte, stride, bit_mask;
	u16 x0, y0;
	u8 *ci, image[(LCD_WIDTH + 7) / 8];

	stride = (w + 7) / 8;
	for(y0 = y; y0 < y + h; ++y0)
	{
		xmem_read(addr, image, stride);
		_lcd_window_start(x, y0, w, 1);
		bit_mask = 0x80;
		ci = image;
		for(x0 = x; x0 < x + w; ++x0)
		{
			if(bit_mask == 0x80)
			{
				byte = *ci++;
				bit_mask = 1;
			}

			_lcd_pixel((byte & bit_mask) ? fg : bg);
			bit_mask <<= 1;
		}

		_lcd_window_end();
		addr += stride;
	}
}

/**
 * @file    lcd.h
 * @author  Anton Tchekov
 * @version 0.2
 * @date    17.05.2023
 */

#ifndef __LCD_H__
#define __LCD_H__

void lcd_backlight(u8 value);


void lcd_init(u8 backlight, RGB565 bg);

RGB565 lcd_color(u8 r, u8 g, u8 b);

void lcd_rect(u16 x, u16 y, u16 w, u16 h, RGB565 color);

void lcd_logo_P(u16 x, u16 y, u16 w, u16 h, const u8 *image);

u8 lcd_char(u16 x, u16 y, RGB565 fg, RGB565 bg, char c);

u16 lcd_string(u16 x, u16 y, RGB565 fg, RGB565 bg, const char *s);

u16 lcd_string_P(u16 x, u16 y, RGB565 fg, RGB565 bg, const char *s);

/**
 * @brief Draw an RGB565 image from XMEM
 *
 * @param x X-Coordinate
 * @param y Y-Coordinate
 * @param w Width
 * @param h Height
 * @param addr XMEM-Address
 */
void lcd_image_rgb565(u16 x, u16 y, u16 w, u16 h, u32 addr);

/**
 * @brief Draw a colored Grayscale image from XMEM
 *
 * @param x X-Coordinate
 * @param y Y-Coordinate
 * @param w Width
 * @param h Height
 * @param fg Foreground color
 * @param bg Background color
 * @param addr XMEM-Address
 */
void lcd_image_grayscale(
	u16 x, u16 y, u16 w, u16 h, u32 addr, u32 fg, u32 bg);

/**
 * @brief Draw a colored 1-bit image from XMEM
 *
 * @param x X-Coordinate
 * @param y Y-Coordinate
 * @param w Width
 * @param h Height
 * @param fg Foreground color
 * @param bg Background color
 * @param addr XMEM-Address
 */
void lcd_image_1bit(
	u16 x, u16 y, u16 w, u16 h, u32 addr, RGB565 fg, RGB565 bg);

#endif /* __LCD_H__ */



/**
 * @brief System call to draw an RGB565 image
 *
 * @param args args[0]: X-Coordinate
 *             args[1]: Y-Coordinate
 *             args[2]: Width
 *             args[3]: Height
 *             args[4]: Pointer to RGB565 image
 * @return Non-zero on error
 */
static u8 syscall_gfx_image_rgb565(u32 *args)
{
	u32 image, bytes;
	u16 x, y, w, h;

	x = args[0];
	y = args[1];
	w = args[2];
	h = args[3];
	if(_gfx_check_bounds(x, y, w, h))
	{
		return 1;
	}

	image = args[4];
	bytes = (u32)2 * (u32)w * (u32)h;
	if(_memory_check_bounds(image, bytes))
	{
		return 1;
	}

	env_gfx_image_rgb565(x, y, w, h, image);
	return 0;
}

/**
 * @brief System call to draw a grayscale image
 *
 * @param args args[0]: Pointer to rectangle struct
 *             args[1]: Pointer to grayscale image
 *             args[2]: ABGR Foreground color
 *             args[3]: ABGR Background color
 * @return Non-zero on error
 */
static u8 syscall_gfx_image_grayscale(u32 *args)
{
	u16 x, y, w, h;
	u32 image, bytes, rect_addr, fg, bg;
	Rectangle rect;

	rect_addr = args[0];
	if(_memory_check_bounds(rect_addr, sizeof(rect)))
	{
		return 1;
	}

	env_memory_read(rect_addr, &rect, sizeof(rect));
	x = rect.X;
	y = rect.Y;
	w = rect.W;
	h = rect.H;
	if(_gfx_check_bounds(x, y, w, h))
	{
		return 1;
	}

	image = args[1];
	bytes = (u32)w * (u32)h;
	if(_memory_check_bounds(image, bytes))
	{
		return 1;
	}

	fg = args[2];
	bg = args[3];
	env_gfx_image_grayscale(x, y, w, h, image, fg, bg);
	return 0;
}

/**
 * @brief System call to draw a 1-bit-per-pixel image
 *
 * @param args args[0]: Pointer to rectangle struct
 *             args[1]: Pointer to 1bpp image
 *             args[2]: ABGR Foreground color
 *             args[3]: ABGR Background color
 * @return Non-zero on error
 */
static u8 syscall_gfx_image_1bit(u32 *args)
{
	u16 x, y, w, h;
	u32 image, bytes, rect_addr, fg, bg;
	Rectangle rect;

	rect_addr = args[0];
	if(_memory_check_bounds(rect_addr, sizeof(rect)))
	{
		return 1;
	}

	env_memory_read(rect_addr, &rect, sizeof(rect));
	x = rect.X;
	y = rect.Y;
	w = rect.W;
	h = rect.H;
	if(_gfx_check_bounds(x, y, w, h))
	{
		return 1;
	}

	image = args[1];
	bytes = (u32)w * (u32)h;
	if(_memory_check_bounds(image, bytes))
	{
		return 1;
	}

	fg = args[2];
	bg = args[3];
	env_gfx_image_1bit(x, y, w, h, image, fg, bg);
	return 0;
}



