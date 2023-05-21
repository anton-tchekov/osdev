#include <ps2.h>
#include <serial.h>
#include <adc.h>
#include <random.h>
#include <timer.h>
#include <rtc.h>
#include <xmem.h>
#include <logger.h>
#include <spi.h>
#include <lcd.h>
#include <gpio.h>
#include <sd.h>
#include <initrd.h>
#include <avr/pgmspace.h>
#include "logo/logo_tiny.c"
#include "logo/logo_text.c"

char byte_to_hex(u8 byte)
{
	if(byte < 10)
	{
		return byte + '0';
	}

	return byte - 10 + 'A';
}

void ps2_event(u8 byte)
{
	/* Called when a byte is received from the keyboard */
	/* serial_tx('0'); */
	/* serial_tx('x'); */
	/* serial_tx(byte_to_hex((byte >> 4) & 0x0F)); */
	/* serial_tx(byte_to_hex(byte & 0x0F)); */
}

void serial_write(const void *data, u32 len)
{
	u32 i;
	const char *data8 = data;
	for(i = 0; i < len; ++i)
	{
		serial_tx(data8[i]);
	}
}

u32 memory_size(void)
{
	return 3UL * 128UL * 1024UL;
}

void memory_read(u32 addr, void *data, u32 size)
{
	xmem_read(addr, data, size);
}

void memory_write(u32 addr, const void *data, u32 size)
{
	xmem_write(addr, data, size);
}

void gfx_rect(i32 x, i32 y, i32 w, i32 h, u32 color){}
u32 syscall_gfx_image_rgba(u32 *args){}
u32 syscall_gfx_image_rgb(u32 *args){}
u32 syscall_gfx_image_rgb565(u32 *args){}
u32 syscall_gfx_image_grayscale(u32 *args){}
u32 syscall_gfx_image_1bit(u32 *args){}


#define LOGO_X_OFFSET      30
#define LOGO_TEXT_X_OFFSET 56
#define LOGO_Y_OFFSET      50

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
	lcd_logo_P(
		LOGO_X_OFFSET,
		LOGO_Y_OFFSET,
		LOGO_TINY_WIDTH,
		LOGO_TINY_HEIGHT,
		logo_tiny);

	lcd_logo_P(
		LOGO_X_OFFSET + LOGO_TEXT_X_OFFSET,
		LOGO_Y_OFFSET + LOGO_TINY_HEIGHT / 2 - LOGO_TEXT_HEIGHT / 2,
		LOGO_TEXT_WIDTH,
		LOGO_TEXT_HEIGHT,
		logo_text);

	/* Initialize keyboard (fast) */
	ps2_init();

	/* Initialize ADC (fast) */
	adc_init();

	/* Initialize RNG (fast) */
	random_init();

	/* Initialize RTC driver (fast) */
	rtc_init();

	/* Initialize external memory late because of slow memory test */
	xmem_init();

	/* Initialize SD card driver */
	sd_init();

	/* --- READY --- */

	/* Load initial ram disk */
	initrd_load();

	log_boot_P(LOG_NONE, PSTR("\nStarting RISC-V Emulator Kernel ...\n\n"));

	/* Infinite loop */
	for(;;)
	{
	}

	/* Unreachable */
	return 0;
}

