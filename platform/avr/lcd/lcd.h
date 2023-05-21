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

/** Color definitions in RGB565 format */
#define COLOR_RED      0xF800
#define COLOR_YELLOW   0xFFE0
#define COLOR_GREEN    0x07E0
#define COLOR_CYAN     0x07FF
#define COLOR_BLUE     0x001F
#define COLOR_MAGENTA  0xF81F

#define COLOR_WHITE    0xFFFF
#define COLOR_BLACK    0x0000

/** Width and height of the display */
#define LCD_HEIGHT     480
#define LCD_WIDTH      320

typedef u16 RGB565;

/**
 * @brief Set LCD backligh brightness
 *
 * @param backlight Backlight brightness from 0-255
 */
void lcd_backlight(u8 value);

/**
 * @brief TODO
 */
bool lcd_initialized(void);

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
 * @brief Draw
 *
 * @param x X-Coordinate
 * @param y Y-Coordinate
 * @param w Width
 * @param h height
 * @param color Fill color
 */
void lcd_rect(u16 x, u16 y, u16 w, u16 h, RGB565 color);

/**
 * @brief Draw an image with a callback
 *
 * @param x X-Coordinate
 * @param y Y-Coordinate
 * @param w Width
 * @param h Height
 * @param color_callback Callback function that receives x and y offsets from
 *                       inside the rectangle and returns RGB565 color
 */
void lcd_window(u16 x, u16 y, u16 w, u16 h, RGB565 (*color_callback)(u16, u16));

/**
 * @brief TODO
 *
 * @param x
 * @param y
 * @param w
 * @param h
 * @param image
 */
void lcd_logo_P(u16 x, u16 y, u16 w, u16 h, const u8 *image);

/**
 * @brief TODO
 *
 * @param x
 * @param y
 * @param fg
 * @param bg
 * @param c
 * @return u16
 */
u8 lcd_char(u16 x, u16 y, RGB565 fg, RGB565 bg, char c);

/**
 * @brief TODO
 *
 * @param x
 * @param y
 * @param fg
 * @param bg
 * @param s
 * @return
 */
u16 lcd_string(u16 x, u16 y, RGB565 fg, RGB565 bg, const char *s);

/**
 * @brief TODO
 *
 * @param x
 * @param y
 * @param fg
 * @param bg
 * @param s
 * @return u16
 */
u16 lcd_string_P(u16 x, u16 y, RGB565 fg, RGB565 bg, const char *s);

#endif /* __LCD_H__ */
