/**
 * @file    gpio.h
 * @author  Anton Tchekov
 * @version 0.1
 * @date    20.05.2023
 * @brief   GPIO Config
 */

#ifndef __GPIO_H__
#define __GPIO_H__

#include <avr/io.h>

/* SPI */
#define SPI_DIR            DDRB
#define SPI_MOSI          3
#define SPI_MISO          4
#define SPI_SCK           5

/* LCD */
#define LCD_RST_DIR        DDRD
#define LCD_RST_OUT        PORTD
#define LCD_RST_PIN       7

#define LCD_CS_DIR         DDRC
#define LCD_CS_OUT         PORTC
#define LCD_CS_PIN        0

#define LCD_DC_DIR         DDRC
#define LCD_DC_OUT         PORTC
#define LCD_DC_PIN        1

#define LCD_CS_0            LCD_CS_OUT &= ~(1 << LCD_CS_PIN)
#define LCD_CS_1            LCD_CS_OUT |= (1 << LCD_CS_PIN)

#define LCD_DC_0            LCD_DC_OUT &= ~(1 << LCD_DC_PIN)
#define LCD_DC_1            LCD_DC_OUT |= (1 << LCD_DC_PIN)

#define LCD_RST_0           LCD_RST_OUT &= ~(1 << LCD_RST_PIN)
#define LCD_RST_1           LCD_RST_OUT |= (1 << LCD_RST_PIN)

/* SD */
#define SD_CS_DIR           DDRD
#define SD_CS_OUT           PORTD
#define SD_CS_PIN          4

#define SD_SELECT           SD_CS_OUT &= ~(1 << SD_CS_PIN)
#define SD_DESELECT         SD_CS_OUT |= (1 << SD_CS_PIN)

/* XMEM */
#define XMEM_CS_DIR         DDRB
#define XMEM_CS_OUT         PORTB
#define XMEM_CS_0_PIN      0
#define XMEM_CS_1_PIN      1
#define XMEM_CS_2_PIN      2

#define XMEM_SELECT(BANK)   XMEM_CS_OUT &= ~(1 << bank)
#define XMEM_DESELECT(BANK) XMEM_CS_OUT |= (1 << bank)
#define XMEM_DESELECT_ALL   XMEM_CS_OUT |= \
	(1 << XMEM_CS_0_PIN) | \
	(1 << XMEM_CS_1_PIN) | \
	(1 << XMEM_CS_2_PIN)

/* KBD */

/* --- CLOCK: Arduino pin 2 --- */
/** Clock pin - Data direction register */
#define PS2_CLOCK_PORT_DIR  DDRB

/** Clock pin - Input register */
#define PS2_CLOCK_PORT_IN   PINB

/** Clock pin - Output register */
#define PS2_CLOCK_PORT_OUT  PORTB

/** Clock pin */
#define PS2_CLOCK_PIN       2

/** Clock pin - External interrupt */
#define PS2_EXT_INTERRUPT   INT0

/* --- DATA: Arduino pin 3 --- */
/** Data pin - Data direction register */
#define PS2_DATA_PORT_DIR   DDRB

/** Data pin - Input register */
#define PS2_DATA_PORT_IN    PINB

/** Data pin - Output register */
#define PS2_DATA_PORT_OUT   PORTB

/** Data pin */
#define PS2_DATA_PIN        3

/**
 * @brief Configure all GPIO pins
 *
 * This function is neccessary because multiple peripherals are connected to the
 * SPI bus, so all CS lines need to be driven LOW before SPI initialization.
 */
void gpio_configure(void);

#endif /* __GPIO_H__ */
