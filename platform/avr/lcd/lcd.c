/**
 * @file    lcd.c
 * @author  Anton Tchekov
 * @version 0.2
 * @date    17.05.2023
 */

#include <lcd.h>
#include <logger.h>
#include <spi.h>
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

#define SCAN_DIR       L2R_U2D

#define LCD_RST_DIR    DDRD
#define LCD_RST_OUT    PORTD
#define LCD_RST_PIN   7

#define LCD_CS_DIR     DDRC
#define LCD_CS_OUT     PORTC
#define LCD_CS_PIN    0

#define LCD_DC_DIR     DDRC
#define LCD_DC_OUT     PORTC
#define LCD_DC_PIN    1

#define LCD_CS_0        LCD_CS_OUT &= ~(1 << LCD_CS_PIN)
#define LCD_CS_1        LCD_CS_OUT |= (1 << LCD_CS_PIN)

#define LCD_DC_0        LCD_DC_OUT &= ~(1 << LCD_DC_PIN)
#define LCD_DC_1        LCD_DC_OUT |= (1 << LCD_DC_PIN)

#define LCD_RST_0       LCD_RST_OUT &= ~(1 << LCD_RST_PIN)
#define LCD_RST_1       LCD_RST_OUT |= (1 << LCD_RST_PIN)

/* --- PRIVATE --- */
static void _lcd_configure_gpio(void)
{
	LCD_RST_DIR |= (1 << LCD_RST_PIN);
	LCD_CS_DIR |= (1 << LCD_CS_PIN);
	LCD_DC_DIR |= (1 << LCD_DC_PIN);
}

static void _lcd_configure_spi(void)
{
	/* F_SPI = F_CPU / 2 */
	SPCR &= ~((1 << SPR1) | (1 << SPR0));
	SPSR |= (1 << SPI2X);
}

static void _lcd_reset(void)
{
	LCD_RST_1;
	_delay_ms(500);
	LCD_RST_0;
	_delay_ms(500);
	LCD_RST_1;
	_delay_ms(500);
}

static void _lcd_write_reg(u8 reg)
{
	LCD_DC_0;
	LCD_CS_0;
	spi_xchg(reg);
	LCD_CS_1;
}

static void _lcd_write_data(u8 data)
{
	LCD_DC_1;
	LCD_CS_0;
	spi_xchg(0);
	spi_xchg(data);
	LCD_CS_1;
}

static void _lcd_window_start(u16 x, u16 y, u16 w, u16 h)
{
	u16 x_end, y_end;

	x_end = x + w - 1;
	y_end = y + h - 1;

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

static inline void _lcd_pixel(u16 data)
{
	spi_xchg(data >> 8);
	spi_xchg(data);
}

static inline void _lcd_window_end(void)
{
	LCD_CS_1;
}

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
	_lcd_configure_gpio();
	_lcd_configure_spi();

	_lcd_reset();
	lcd_backlight(backlight);
	_lcd_init_reg();
	_lcd_set_gram_scan_dir(SCAN_DIR);
	_delay_ms(200);
	_lcd_write_reg(0x11);
	_delay_ms(120);
	_lcd_write_reg(0x29);

	lcd_rect(0, 0, LCD_WIDTH, LCD_HEIGHT, bg);

	log_boot_P(PSTR("LCD driver initialized"));
}

void lcd_rect(u16 x, u16 y, u16 w, u16 h, RGB565 color)
{
	u16 w0, h0;

	_lcd_configure_spi();
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
