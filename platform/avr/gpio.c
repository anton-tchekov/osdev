/**
 * @file    gpio.c
 * @author  Anton Tchekov
 * @version 0.1
 * @date    20.05.2023
 * @brief   GPIO Config
 */

/* --- SPI --- */

/** SPI direction port */
#define SPI_DIR             DDRB

/** SPI MOSI pin number */
#define SPI_MOSI           3

/** SPI MISO pin number */
#define SPI_MISO           4

/** SPI SCK pin number */
#define SPI_SCK            5

/* --- LCD --- */

/** LCD backlight pin direction port */
#define LCD_BL_DIR          DDRD

/** LCD backlight pin number */
#define LCD_BL_PIN         3

/** LCD reset pin direction port */
#define LCD_RST_DIR         DDRD

/** LCD reset pin output port */
#define LCD_RST_OUT         PORTD

/** LCD reset pin number */
#define LCD_RST_PIN        7

/** LCD chip select pin direction port */
#define LCD_CS_DIR          DDRC

/** LCD chip select pin output port */
#define LCD_CS_OUT          PORTC

/** LCD chip select pin number */
#define LCD_CS_PIN         0

/** LCD data/command pin direction port */
#define LCD_DC_DIR          DDRC

/** LCD data/command pin output port */
#define LCD_DC_OUT          PORTC

/** LCD data/command pin number */
#define LCD_DC_PIN         1

/* --- SD CARD --- */

/** Set LCD CS LOW */
#define LCD_SELECT          LCD_CS_OUT &= ~(1 << LCD_CS_PIN)

/** Set LCD CS HIGH */
#define LCD_DESELECT        LCD_CS_OUT |= (1 << LCD_CS_PIN)

/** Set LCD DC LOW */
#define LCD_DC_0            LCD_DC_OUT &= ~(1 << LCD_DC_PIN)

/** Set LCD DC HIGH */
#define LCD_DC_1            LCD_DC_OUT |= (1 << LCD_DC_PIN)

/** Set LCD RST LOW */
#define LCD_RST_0           LCD_RST_OUT &= ~(1 << LCD_RST_PIN)

/** Set LCD RST HIGH */
#define LCD_RST_1           LCD_RST_OUT |= (1 << LCD_RST_PIN)

/** SD card CS direction port */
#define SD_CS_DIR           DDRD

/** SD card CS output port */
#define SD_CS_OUT           PORTD

/** SD card CS pin number */
#define SD_CS_PIN          4

/** Select SD card */
#define SD_SELECT           SD_CS_OUT &= ~(1 << SD_CS_PIN)

/** Deselect SD card */
#define SD_DESELECT         SD_CS_OUT |= (1 << SD_CS_PIN)

/* --- XMEM --- */

/** XMEM CS pins direction register */
#define XMEM_CS_DIR         DDRB

/** XMEM CS pins output register */
#define XMEM_CS_OUT         PORTB

/** XMEM bank 0 CS pin */
#define XMEM_CS_0_PIN      0

/** XMEM bank 1 CS pin */
#define XMEM_CS_1_PIN      1

/** XMEM bank 2 CS pin */
#define XMEM_CS_2_PIN      2

/** Macro to select XMEM bank */
#define XMEM_SELECT(BANK)   XMEM_CS_OUT &= ~(1 << (BANK))

/** Macro to deselect XMEM bank */
#define XMEM_DESELECT(BANK) XMEM_CS_OUT |= (1 << (BANK))

/** Deselect all XMEM banks */
#define XMEM_DESELECT_ALL   XMEM_CS_OUT |= \
	(1 << XMEM_CS_0_PIN) | \
	(1 << XMEM_CS_1_PIN) | \
	(1 << XMEM_CS_2_PIN)

/* --- KEYBOARD --- */

/* --- CLOCK: Arduino pin 2 --- */
/** Clock pin - Data direction register */
#define PS2_CLOCK_PORT_DIR  DDRB

/** Clock pin - Input register */
#define PS2_CLOCK_PORT_IN   PINB

/** Clock pin - Output register */
#define PS2_CLOCK_PORT_OUT  PORTB

/** Clock pin */
#define PS2_CLOCK_PIN      2

/** Clock pin - External interrupt */
#define PS2_EXT_INTERRUPT   INT0

/* --- DATA: Arduino pin A3 --- */
/** Data pin - Data direction register */
#define PS2_DATA_PORT_DIR   DDRC

/** Data pin - Input register */
#define PS2_DATA_PORT_IN    PINC

/** Data pin - Output register */
#define PS2_DATA_PORT_OUT   PORTC

/** Data pin */
#define PS2_DATA_PIN       3

/**
 * @brief Configure all GPIO pins
 *
 * This function is neccessary because multiple peripherals are connected to the
 * SPI bus, so all CS lines need to be driven LOW before SPI initialization.
 */
static void gpio_configure(void)
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

/**
 * @brief Deselect all SPI peripherals
 */
static void deselect_all(void)
{
	SD_DESELECT;
	XMEM_DESELECT_ALL;
	LCD_DESELECT;
}
