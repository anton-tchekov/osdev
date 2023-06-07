/**
 * @file    gpio.c
 * @author  Anton Tchekov
 * @version 0.1
 * @date    20.05.2023
 */

#include "gpio.h"

void gpio_configure(void)
{
	/* LCD */
	LCD_RST_DIR |= (1 << LCD_RST_PIN);
	LCD_CS_DIR |= (1 << LCD_CS_PIN);
	LCD_DC_DIR |= (1 << LCD_DC_PIN);
	LCD_BL_DIR |= (1 << LCD_BL_PIN);
	LCD_DESELECT;

	/* SD */
	SD_CS_DIR |= (1 << SD_CS_PIN);
	SD_DESELECT;

	/* XMEM */
	XMEM_CS_DIR |=
		(1 << XMEM_CS_0_PIN) |
		(1 << XMEM_CS_1_PIN) |
		(1 << XMEM_CS_2_PIN);

	XMEM_DESELECT_ALL;

	/* KBD */
	PS2_CLOCK_PORT_OUT |= (1 << PS2_CLOCK_PIN);
	PS2_DATA_PORT_OUT |= (1 << PS2_DATA_PIN);

	/* SPI */
	SPI_DIR |= (1 << SPI_MOSI) | (1 << SPI_SCK);
}

void deselect_all(void)
{
	SD_DESELECT;
	XMEM_DESELECT_ALL;
	LCD_DESELECT;
}
