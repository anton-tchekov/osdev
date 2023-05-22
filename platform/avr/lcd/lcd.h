/**
 * @file    lcd.h
 * @author  Anton Tchekov
 * @version 0.2
 * @date    17.05.2023
 * @brief   LCD Driver
 */

#ifndef __LCD_H__
#define __LCD_H__

#include <types.h>

/* Color definitions in RGB565 format */

/** Red Color as RGB565 */
#define COLOR_RED      0xF800

/** Yellow Color as RGB565 */
#define COLOR_YELLOW   0xFFE0

/** Green Color as RGB565 */
#define COLOR_GREEN    0x07E0

/** Cyan Color as RGB565 */
#define COLOR_CYAN     0x07FF

/** Blue Color as RGB565 */
#define COLOR_BLUE     0x001F

/** Magenta Color as RGB565 */
#define COLOR_MAGENTA  0xF81F

/** White Color as RGB565 */
#define COLOR_WHITE    0xFFFF

/** Black Color as RGB565 */
#define COLOR_BLACK    0x0000

/** Height of the display */
#define LCD_HEIGHT     480

/** Width of the display */
#define LCD_WIDTH      320

/** RGB565 data type */
typedef u16 RGB565;

/**
 * @brief Set LCD backlight brightness
 *
 * @param value Backlight brightness from 0-255
 */
void lcd_backlight(u8 value);

/**
 * @brief Initialize LCD
 *
 * @param backlight Backlight brightness from 0-255
 * @param bg Init background color
 */
void lcd_init(u8 backlight, RGB565 bg);

/**
 * @brief Convert RGB888 to RGB565
 *
 * @param r Red channel 0-255
 * @param g Green channel 0-255
 * @param b Blue channel 0-255
 * @return RGB565 color
 */
RGB565 lcd_color(u8 r, u8 g, u8 b);

/**
 * @brief Draw rectangle
 *
 * @param x X-Coordinate
 * @param y Y-Coordinate
 * @param w Width
 * @param h height
 * @param color Fill color
 */
void lcd_rect(u16 x, u16 y, u16 w, u16 h, RGB565 color);

/**
 * @brief Draw boot logo(s)
 *
 * @param x X-Coordinate
 * @param y Y-Coordinate
 * @param w Width
 * @param h Height
 * @param image Image data (grayscale, red channel)
 */
void lcd_logo_P(u16 x, u16 y, u16 w, u16 h, const u8 *image);

/**
 * @brief Draw a character in the standard font
 *
 * @param x X-Coordinate
 * @param y Y-Coordinate
 * @param fg Foreground text color
 * @param bg Background color
 * @param c ASCII character
 * @return Width of the character that was drawn
 */
u8 lcd_char(u16 x, u16 y, RGB565 fg, RGB565 bg, char c);

/**
 * @brief Draw a string in the default font
 *
 * @param x X-Coordinate
 * @param y Y-Coordinate
 * @param fg Foreground text color
 * @param bg Background color
 * @param s ASCII string
 * @return Width of the string that was drawn
 */
u16 lcd_string(u16 x, u16 y, RGB565 fg, RGB565 bg, const char *s);

/**
 * @brief Draw a string from program memory in the default font
 *
 * @param x X-Coordinate
 * @param y Y-Coordinate
 * @param fg Foreground text color
 * @param bg Background color
 * @param s ASCII string in PROGMEM
 * @return Width of the string that was drawn
 */
u16 lcd_string_P(u16 x, u16 y, RGB565 fg, RGB565 bg, const char *s);

/**
 * @brief Draw an RGBA image from XMEM
 *
 * @param x X-Coordinate
 * @param y Y-Coordinate
 * @param w Width
 * @param h Height
 * @param addr XMEM-Address
 */
void lcd_image_rgba(u16 x, u16 y, u16 w, u16 h, u32 addr);

/**
 * @brief Draw an RGB image from XMEM
 *
 * @param x X-Coordinate
 * @param y Y-Coordinate
 * @param w Width
 * @param h Height
 * @param addr XMEM-Address
 */
void lcd_image_rgb(u16 x, u16 y, u16 w, u16 h, u32 addr);

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
	u16 x, u16 y, u16 w, u16 h, RGB565 fg, RGB565 bg, u32 addr);

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
	u16 x, u16 y, u16 w, u16 h, RGB565 fg, RGB565 bg, u32 addr);

#endif /* __LCD_H__ */
