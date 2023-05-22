/**
 * @file    lcd.c
 * @author  Anton Tchekov
 * @version 0.2
 * @date    17.05.2023
 */

#include <lcd.h>
#include <logger.h>
#include <spi.h>
#include <gpio.h>
#include <xmem.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

enum
{
	L2R_U2D,
	L2R_D2U,
	R2L_U2D,
	R2L_D2U,
	U2D_L2R,
	U2D_R2L,
	D2U_L2R,
	D2U_R2L
};

/** LCD orientation*/
#define SCAN_DIR       L2R_U2D

/** Default font width in pixels */
#define FONT_WIDTH    5

/** Default font height in pixels */
#define FONT_HEIGHT   7

/** Default 5x7 monospace font */
static const u8 _font5x7[] PROGMEM =
{
	0x00, 0x00, 0x00, 0x00, 0x00, /*   */
	0x00, 0x00, 0x5F, 0x00, 0x00, /* ! */
	0x00, 0x07, 0x00, 0x07, 0x00, /* " */
	0x14, 0x7F, 0x14, 0x7F, 0x14, /* # */
	0x24, 0x2A, 0x7F, 0x2A, 0x12, /* $ */
	0x23, 0x13, 0x08, 0x64, 0x62, /* % */
	0x36, 0x49, 0x55, 0x22, 0x50, /* & */
	0x00, 0x00, 0x03, 0x00, 0x00, /* ' */
	0x00, 0x1C, 0x22, 0x41, 0x00, /* ( */
	0x00, 0x41, 0x22, 0x1C, 0x00, /* ) */
	0x14, 0x08, 0x3E, 0x08, 0x14, /* * */
	0x08, 0x08, 0x3E, 0x08, 0x08, /* + */
	0x00, 0x50, 0x30, 0x00, 0x00, /* , */
	0x08, 0x08, 0x08, 0x08, 0x08, /* - */
	0x00, 0x60, 0x60, 0x00, 0x00, /* . */
	0x20, 0x10, 0x08, 0x04, 0x02, /* / */

	0x3E, 0x51, 0x49, 0x45, 0x3E, /* 0 */
	0x00, 0x42, 0x7F, 0x40, 0x00, /* 1 */
	0x62, 0x51, 0x49, 0x49, 0x46, /* 2 */
	0x22, 0x41, 0x49, 0x49, 0x36, /* 3 */
	0x18, 0x14, 0x12, 0x7F, 0x10, /* 4 */
	0x27, 0x45, 0x45, 0x45, 0x39, /* 5 */
	0x3C, 0x4A, 0x49, 0x49, 0x30, /* 6 */
	0x01, 0x71, 0x09, 0x05, 0x03, /* 7 */
	0x36, 0x49, 0x49, 0x49, 0x36, /* 8 */
	0x06, 0x49, 0x49, 0x29, 0x1E, /* 9 */

	0x00, 0x36, 0x36, 0x00, 0x00, /* : */
	0x00, 0x56, 0x36, 0x00, 0x00, /* ; */
	0x08, 0x14, 0x22, 0x41, 0x00, /* < */
	0x14, 0x14, 0x14, 0x14, 0x14, /* = */
	0x00, 0x41, 0x22, 0x14, 0x08, /* > */
	0x02, 0x01, 0x51, 0x09, 0x06, /* ? */
	0x32, 0x49, 0x79, 0x41, 0x3E, /* @ */

	0x7C, 0x12, 0x11, 0x12, 0x7C, /* A */
	0x41, 0x7F, 0x49, 0x49, 0x36, /* B */
	0x3E, 0x41, 0x41, 0x41, 0x22, /* C */
	0x41, 0x7F, 0x41, 0x41, 0x3E, /* D */
	0x7F, 0x49, 0x49, 0x49, 0x41, /* E */
	0x7F, 0x09, 0x09, 0x09, 0x01, /* F */
	0x3E, 0x41, 0x41, 0x49, 0x7A, /* G */
	0x7F, 0x08, 0x08, 0x08, 0x7F, /* H */
	0x00, 0x41, 0x7F, 0x41, 0x00, /* I */
	0x20, 0x40, 0x41, 0x3F, 0x01, /* J */
	0x7F, 0x08, 0x14, 0x22, 0x41, /* K */
	0x7F, 0x40, 0x40, 0x40, 0x40, /* L */
	0x7F, 0x02, 0x0C, 0x02, 0x7F, /* M */
	0x7F, 0x04, 0x08, 0x10, 0x7F, /* N */
	0x3E, 0x41, 0x41, 0x41, 0x3E, /* O */
	0x7F, 0x09, 0x09, 0x09, 0x06, /* P */
	0x3E, 0x41, 0x51, 0x21, 0x5E, /* Q */
	0x7F, 0x09, 0x19, 0x29, 0x46, /* R */
	0x26, 0x49, 0x49, 0x49, 0x32, /* S */
	0x01, 0x01, 0x7F, 0x01, 0x01, /* T */
	0x3F, 0x40, 0x40, 0x40, 0x3F, /* U */
	0x1F, 0x20, 0x40, 0x20, 0x1F, /* V */
	0x3F, 0x40, 0x38, 0x40, 0x3F, /* W */
	0x63, 0x14, 0x08, 0x14, 0x63, /* X */
	0x07, 0x08, 0x70, 0x08, 0x07, /* Y */
	0x61, 0x51, 0x49, 0x45, 0x43, /* Z */

	0x00, 0x7F, 0x41, 0x41, 0x00, /* [ */
	0x02, 0x04, 0x08, 0x10, 0x20, /* \ */
	0x00, 0x41, 0x41, 0x7F, 0x00, /* ] */
	0x04, 0x02, 0x01, 0x02, 0x04, /* ^ */
	0x40, 0x40, 0x40, 0x40, 0x40, /* _ */
	0x00, 0x01, 0x02, 0x04, 0x00, /* ` */

	0x20, 0x54, 0x54, 0x54, 0x78, /* a */
	0x7F, 0x48, 0x44, 0x44, 0x38, /* b */
	0x38, 0x44, 0x44, 0x44, 0x20, /* c */
	0x38, 0x44, 0x44, 0x48, 0x7F, /* d */
	0x38, 0x54, 0x54, 0x54, 0x18, /* e */
	0x08, 0x7E, 0x09, 0x01, 0x02, /* f */
	0x08, 0x54, 0x54, 0x54, 0x3C, /* g */
	0x7F, 0x08, 0x04, 0x04, 0x78, /* h */
	0x00, 0x48, 0x7D, 0x40, 0x00, /* i */
	0x20, 0x40, 0x44, 0x3D, 0x00, /* j */
	0x7F, 0x10, 0x28, 0x44, 0x00, /* k */
	0x00, 0x41, 0x7F, 0x40, 0x00, /* l */
	0x7C, 0x04, 0x78, 0x04, 0x78, /* m */
	0x7C, 0x08, 0x04, 0x04, 0x78, /* n */
	0x38, 0x44, 0x44, 0x44, 0x38, /* o */
	0x7C, 0x14, 0x14, 0x14, 0x08, /* p */
	0x08, 0x14, 0x14, 0x18, 0x7C, /* q */
	0x7C, 0x08, 0x04, 0x04, 0x08, /* r */
	0x48, 0x54, 0x54, 0x54, 0x20, /* s */
	0x04, 0x3F, 0x44, 0x40, 0x20, /* t */
	0x3C, 0x40, 0x40, 0x20, 0x7C, /* u */
	0x1C, 0x20, 0x40, 0x20, 0x1C, /* v */
	0x3C, 0x40, 0x30, 0x40, 0x3C, /* w */
	0x44, 0x28, 0x10, 0x28, 0x44, /* x */
	0x0C, 0x50, 0x50, 0x50, 0x3C, /* y */
	0x44, 0x64, 0x54, 0x4C, 0x44, /* z */

	0x00, 0x08, 0x36, 0x41, 0x00, /* { */
	0x00, 0x00, 0x7F, 0x00, 0x00, /* | */
	0x00, 0x41, 0x36, 0x08, 0x00, /* } */
	0x10, 0x08, 0x08, 0x10, 0x08  /* ~ */
};

/** Reverse gamma correction table */
static const u8 PROGMEM _gamma[] =
{
	  0,  21,  28,  34,  39,  43,  46,  50,  53,  56,  59,  61,  64,  66,  68,  70,
	 72,  74,  76,  78,  80,  82,  84,  85,  87,  89,  90,  92,  93,  95,  96,  98,
	 99, 101, 102, 103, 105, 106, 107, 109, 110, 111, 112, 114, 115, 116, 117, 118,
	119, 120, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135,
	136, 137, 138, 139, 140, 141, 142, 143, 144, 144, 145, 146, 147, 148, 149, 150,
	151, 151, 152, 153, 154, 155, 156, 156, 157, 158, 159, 160, 160, 161, 162, 163,
	164, 164, 165, 166, 167, 167, 168, 169, 170, 170, 171, 172, 173, 173, 174, 175,
	175, 176, 177, 178, 178, 179, 180, 180, 181, 182, 182, 183, 184, 184, 185, 186,
	186, 187, 188, 188, 189, 190, 190, 191, 192, 192, 193, 194, 194, 195, 195, 196,
	197, 197, 198, 199, 199, 200, 200, 201, 202, 202, 203, 203, 204, 205, 205, 206,
	206, 207, 207, 208, 209, 209, 210, 210, 211, 212, 212, 213, 213, 214, 214, 215,
	215, 216, 217, 217, 218, 218, 219, 219, 220, 220, 221, 221, 222, 223, 223, 224,
	224, 225, 225, 226, 226, 227, 227, 228, 228, 229, 229, 230, 230, 231, 231, 232,
	232, 233, 233, 234, 234, 235, 235, 236, 236, 237, 237, 238, 238, 239, 239, 240,
	240, 241, 241, 242, 242, 243, 243, 244, 244, 245, 245, 246, 246, 247, 247, 248,
	248, 249, 249, 249, 250, 250, 251, 251, 252, 252, 253, 253, 254, 254, 255, 255
};

/* --- PRIVATE --- */

/**
 * @brief Perform gamma correction on a color channel
 *
 * @param value Input color channel value
 * @return Gamma corrected output value
 */
static inline u8 _gamma_correction(u8 value)
{
	return pgm_read_byte(_gamma + value);
}

/**
 * @brief Reset the LCD
 */
static void _lcd_reset(void)
{
	LCD_RST_1;
	_delay_ms(500);
	LCD_RST_0;
	_delay_ms(500);
	LCD_RST_1;
	_delay_ms(500);
}

/**
 * @brief Write an LCD register
 *
 * @param reg Register value
 */
static void _lcd_write_reg(u8 reg)
{
	LCD_DC_0;
	LCD_CS_0;
	spi_xchg(reg);
	LCD_CS_1;
}

/**
 * @brief Send data to the LCD
 *
 * @param data Data value
 */
static void _lcd_write_data(u8 data)
{
	LCD_DC_1;
	LCD_CS_0;
	spi_xchg(0);
	spi_xchg(data);
	LCD_CS_1;
}

/**
 * @brief Start an LCD drawing rectangle
 *
 * @param x X-Coordinate
 * @param y Y-Coordinate
 * @param w Width
 * @param h Height
 */
static void _lcd_window_start(u16 x, u16 y, u16 w, u16 h)
{
	u16 x_end, y_end;

	x_end = x + w - 1;
	y_end = y + h - 1;

	spi_fast();

	_lcd_write_reg(0x2A);
	_lcd_write_data(x >> 8);
	_lcd_write_data(x);
	_lcd_write_data(x_end >> 8);
	_lcd_write_data(x_end);
	_lcd_write_reg(0x2B);
	_lcd_write_data(y >> 8);
	_lcd_write_data(y);
	_lcd_write_data(y_end >> 8);
	_lcd_write_data(y_end);
	_lcd_write_reg(0x2C);
	LCD_DC_1;
	LCD_CS_0;
}

/**
 * @brief Send the color of one pixel to the LCD
 *
 * @param data RGB565 color
 */
static inline void _lcd_pixel(RGB565 data)
{
	spi_xchg(data >> 8);
	spi_xchg(data);
}

/**
 * @brief End LCD drawing rectangle
 */
static inline void _lcd_window_end(void)
{
	LCD_CS_1;
}

/**
 * @brief Send LCD initialization commands
 */
static void _lcd_init_reg(void)
{
	_lcd_write_reg(0xF9);
	_lcd_write_data(0x00);
	_lcd_write_data(0x08);
	_lcd_write_reg(0xC0);
	_lcd_write_data(0x19);
	_lcd_write_data(0x1a);
	_lcd_write_reg(0xC1);
	_lcd_write_data(0x45);
	_lcd_write_data(0x00);
	_lcd_write_reg(0xC2);
	_lcd_write_data(0x33);
	_lcd_write_reg(0xC5);
	_lcd_write_data(0x00);
	_lcd_write_data(0x28);
	_lcd_write_reg(0xB1);
	_lcd_write_data(0xA0);
	_lcd_write_data(0x11);
	_lcd_write_reg(0xB4);
	_lcd_write_data(0x02);
	_lcd_write_reg(0xB6);
	_lcd_write_data(0x00);
	_lcd_write_data(0x42);
	_lcd_write_data(0x3B);
	_lcd_write_reg(0xB7);
	_lcd_write_data(0x07);
	_lcd_write_reg(0xE0);
	_lcd_write_data(0x1F);
	_lcd_write_data(0x25);
	_lcd_write_data(0x22);
	_lcd_write_data(0x0B);
	_lcd_write_data(0x06);
	_lcd_write_data(0x0A);
	_lcd_write_data(0x4E);
	_lcd_write_data(0xC6);
	_lcd_write_data(0x39);
	_lcd_write_data(0x00);
	_lcd_write_data(0x00);
	_lcd_write_data(0x00);
	_lcd_write_data(0x00);
	_lcd_write_data(0x00);
	_lcd_write_data(0x00);
	_lcd_write_reg(0xE1);
	_lcd_write_data(0x1F);
	_lcd_write_data(0x3F);
	_lcd_write_data(0x3F);
	_lcd_write_data(0x0F);
	_lcd_write_data(0x1F);
	_lcd_write_data(0x0F);
	_lcd_write_data(0x46);
	_lcd_write_data(0x49);
	_lcd_write_data(0x31);
	_lcd_write_data(0x05);
	_lcd_write_data(0x09);
	_lcd_write_data(0x03);
	_lcd_write_data(0x1C);
	_lcd_write_data(0x1A);
	_lcd_write_data(0x00);
	_lcd_write_reg(0xF1);
	_lcd_write_data(0x36);
	_lcd_write_data(0x04);
	_lcd_write_data(0x00);
	_lcd_write_data(0x3C);
	_lcd_write_data(0x0F);
	_lcd_write_data(0x0F);
	_lcd_write_data(0xA4);
	_lcd_write_data(0x02);
	_lcd_write_reg(0xF2);
	_lcd_write_data(0x18);
	_lcd_write_data(0xA3);
	_lcd_write_data(0x12);
	_lcd_write_data(0x02);
	_lcd_write_data(0x32);
	_lcd_write_data(0x12);
	_lcd_write_data(0xFF);
	_lcd_write_data(0x32);
	_lcd_write_data(0x00);
	_lcd_write_reg(0xF4);
	_lcd_write_data(0x40);
	_lcd_write_data(0x00);
	_lcd_write_data(0x08);
	_lcd_write_data(0x91);
	_lcd_write_data(0x04);
	_lcd_write_reg(0xF8);
	_lcd_write_data(0x21);
	_lcd_write_data(0x04);
	_lcd_write_reg(0x3A);
	_lcd_write_data(0x55);
}

/**
 * @brief Set LCD orientation
 *
 * @param scan_dir LCD orientation (see enum above)
 */
static void _lcd_set_gram_scan_dir(u8 scan_dir)
{
	u16 a, b;
	switch(scan_dir)
	{
	case L2R_U2D:
		a = 0x08;
		b = 0x22;
		break;

	case L2R_D2U:
		a = 0x08;
		b = 0x62;
		break;

	case R2L_U2D:
		a = 0x08;
		b = 0x02;
		break;

	case R2L_D2U:
		a = 0x08;
		b = 0x42;
		break;

	case U2D_L2R:
		a = 0x28;
		b = 0x22;
		break;

	case U2D_R2L:
		a = 0x28;
		b = 0x02;
		break;

	case D2U_L2R:
		a = 0x28;
		b = 0x62;
		break;

	case D2U_R2L:
		a = 0x28;
		b = 0x42;
		break;
	}

	_lcd_write_reg(0xB6);
	_lcd_write_data(0x00);
	_lcd_write_data(b);
	_lcd_write_reg(0x36);
	_lcd_write_data(a);
}

/* --- PUBLIC --- */
void lcd_backlight(u8 value)
{
	/* TODO: Write PWM Value */
	(void)value;
}

void lcd_init(u8 backlight, RGB565 bg)
{
	/* Initialize LCD */
	log_boot_P(LOG_INIT, PSTR("LCD driver starting"));
	spi_fast();
	_lcd_reset();
	lcd_backlight(backlight);
	_lcd_init_reg();
	_lcd_set_gram_scan_dir(SCAN_DIR);
	_delay_ms(200);
	_lcd_write_reg(0x11);
	_delay_ms(120);
	_lcd_write_reg(0x29);

	lcd_rect(0, 0, LCD_WIDTH, LCD_HEIGHT, bg);

	log_boot_P(LOG_INIT, PSTR("LCD driver initialized"));
}

void lcd_rect(u16 x, u16 y, u16 w, u16 h, RGB565 color)
{
	u16 w0, h0;

	_lcd_window_start(x, y, w, h);
	for(h0 = 0; h0 < h; ++h0)
	{
		for(w0 = 0; w0 < w; ++w0)
		{
			_lcd_pixel(color);
		}
	}

	_lcd_window_end();
}

RGB565 lcd_color(u8 r, u8 g, u8 b)
{
	return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

void lcd_logo_P(u16 x, u16 y, u16 w, u16 h, const u8 *image)
{
	u16 w0, h0;

	_lcd_window_start(x, y, w, h);
	for(h0 = 0; h0 < h; ++h0)
	{
		for(w0 = 0; w0 < w; ++w0)
		{
			_lcd_pixel(lcd_color(pgm_read_byte(&image[h0 * w + w0]), 0, 0));
		}
	}

	_lcd_window_end();
}

u8 lcd_char(u16 x, u16 y, RGB565 fg, RGB565 bg, char c)
{
	u8 w0, h0;
	const u8 *start;

	start = _font5x7 + (c - 32) * FONT_WIDTH;

	_lcd_window_start(x, y, FONT_WIDTH, FONT_HEIGHT);
	for(h0 = 0; h0 < FONT_HEIGHT; ++h0)
	{
		for(w0 = 0; w0 < FONT_WIDTH; ++w0)
		{
			_lcd_pixel(((pgm_read_byte(start + w0) >> h0) & 1) ? fg : bg);
		}
	}

	_lcd_window_end();
	return FONT_WIDTH + 1;
}

u16 lcd_string(u16 x, u16 y, RGB565 fg, RGB565 bg, const char *s)
{
	u16 i;
	char c;

	i = 0;
	while((c = *s++))
	{
		i += lcd_char(x + i, y, fg, bg, c);
	}

	return i;
}

u16 lcd_string_P(u16 x, u16 y, RGB565 fg, RGB565 bg, const char *s)
{
	u16 i;
	char c;

	i = 0;
	while((c = pgm_read_byte(s++)))
	{
		i += lcd_char(x + i, y, fg, bg, c);
	}

	return i;
}

void lcd_image_rgba(u16 x, u16 y, u16 w, u16 h, u32 addr)
{

}

void lcd_image_rgb(u16 x, u16 y, u16 w, u16 h, u32 addr)
{

}

void lcd_image_rgb565(u16 x, u16 y, u16 w, u16 h, u32 addr)
{

}

void lcd_image_grayscale(
	u16 x, u16 y, u16 w, u16 h, RGB565 fg, RGB565 bg, u32 addr)
{

}

void lcd_image_1bit(
	u16 x, u16 y, u16 w, u16 h, RGB565 fg, RGB565 bg, u32 addr)
{
	u8 byte, stride, bit_mask;
	u16 x0, y0, byte_offset;
	u8 image[64];

	stride = (w + 7) / 8;
	for(y0 = y; y0 < y + h; ++y0)
	{
		xmem_read(addr, image, stride);
		_lcd_window_start(x, y0, w, 1);
		bit_mask = 0x80;
		byte_offset = 0;
		for(x0 = x; x0 < x + w; ++x0)
		{
			if(bit_mask == 0x80)
			{
				byte = image[byte_offset++];
				bit_mask = 1;
			}

			_lcd_pixel((byte & bit_mask) ? fg : bg);
			bit_mask <<= 1;
		}

		_lcd_window_end();
		addr += stride;
	}
}
