/**
 * @file    main.c
 * @author  Anton Tchekov
 * @version 0.1
 * @date    25.05.2023
 * @brief   AVR Main
 */

#include <ps2.h>
#include <serial.h>
#include <adc.h>
#include <random.h>
#include <timer.h>
#include <xmem.h>
#include <logger.h>
#include <spi.h>
#include <lcd.h>
#include <gpio.h>
#include <sd.h>
#include <initrd.h>
#include <avr/pgmspace.h>
#include <emulator.h>

/**
 * @brief AVR Platform main function, loads drivers and starts kernel
 *
 * @return Exit code
 */
int main(void)
{
	/* --- BOOT SEQUENCE --- */

	/* Enable interrupts */
	sei();

	/* First, configure GPIO, because all modules that do I/O depend on it */
	gpio_configure();

	/* Second, initialize serial port so that we can print boot messages */
	serial_init();

	/* Print startup message */
	log_boot_P(LOG_NONE, PSTR("\nImaginaryOS Version 0.1 Booting ...\n"));

	/* Initialize millisecond timer */
	timer_init();

	/* Initialize SPI (Needed for SD Card, External Memory and LCD) */
	spi_init();

	/* Initialize LCD driver early for boot image and messages */
	lcd_init(0xFF, COLOR_BLACK);

	/* Initialize LCD Logger */
	logger_lcd_init();

	/* Initialize keyboard (fast) */
	ps2_init();

	/* Initialize ADC (fast) */
	adc_init();

	/* Initialize RNG (fast) */
	random_init();

	/* Initialize external memory late because of slow memory test */
	xmem_init();

	/* Initialize SD card driver */
	sd_init();

	/* --- READY --- */

	/* Load initial ram disk */
	initrd_load();

	log_boot_P(LOG_NONE, PSTR("\nStarting RISC-V Emulator Kernel ...\n\n"));

	kernel_init();
	log_boot_P(LOG_EXT, PSTR("Setup done!"));

	/* Infinite loop */
	for(;;)
	{
	}

	/* Unreachable */
	return 0;
}

/* --- KEYBOARD --- */

/**
 * @brief Convert 4-bit hex value to ASCII
 *
 * @param byte Value from 0-15
 * @return '0'-'9' / 'A'-'F'
 */
static char byte_to_hex(u8 byte)
{
	if(byte < 10)
	{
		return byte + '0';
	}

	return byte - 10 + 'A';
}
