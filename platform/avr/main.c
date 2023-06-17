/**
 * @file    main.c
 * @author  Anton Tchekov
 * @version 0.1
 * @date    25.05.2023
 * @brief   AVR Main
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>

#include <types.h>
#include <status.h>
#include <atfs.h>
#include <event-types.h>
#include <keyboard-shared.h>

#include <german.c>

#include <util.h>
#include <logger.h>

#include <timer.c>
#include <gpio.c>
#include <serial.c>
#include <spi.c>
#include <lcd.c>
#include <logo/logo_text.c>
#include <logo/logo_tiny.c>
#include <logger.c>
#include <xmem.c>
#include <adc.c>
#include <ps2.c>
#include <random.c>
#include <sd.c>
#include <emulator.c>
#include <initrd.c>
#include <benchmark.c>

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
	lcd_init(128, COLOR_BLACK);

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

	/* Perform CPU and memory benchmark */
	/* benchmark(); */

	/* --- READY --- */

	/* Load init process form disk and start emulator */
	initrd_load();
	log_boot_P(LOG_NONE, PSTR("\nStarting Emulator Kernel ...\n\n"));

	/* Infinite loop */
	for(;;)
	{
		kernel_update();
	}

	/* Unreachable */
	return 0;
}
