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
#define LCD_HEIGHT     320
#define LCD_WIDTH      480

/**
 * @brief Initialize LCD
 */
void lcd_init(void);

#endif /* __LCD_H__ */
