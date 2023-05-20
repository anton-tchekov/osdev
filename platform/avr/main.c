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
#include <avr/pgmspace.h>

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


int main(void)
{
	/* --- BOOT SEQUENCE --- */

	/* Configure GPIO */
	gpio_configure();

	/* Initialize serial port */
	serial_init();

	/* Enable interrupts */
	sei();

	/* Print boot message */
	serial_tx_str_P(PSTR("\nImaginaryOS Version 0.1 Booting ...\n\n"));

	/* Initialize timer */
	timer_init();

	/* Initialize SPI */
	spi_init();

	/* Initialize external memory */
	xmem_init();

	/* Initialize LCD driver */
	lcd_init(0xFF, COLOR_BLACK);
	lcd_rect(10, 10, 200, 200, COLOR_RED);

	/* Initialize ADC */
	adc_init();

	/* Initialize RNG */
	random_init();

	/* Initialize keyboard */
	ps2_init();

	/* Initialize RTC driver */
	rtc_init();

	/* Initialize SD card driver */
	sd_init();

	serial_tx_str_P(PSTR("\nREADY.\n\n"));

	/* --- READY --- */

	{
		u8 buf[512], i;
		Status ret;

		/* Test */
		log_boot_P(PSTR("Printing sectors\n"));

		for(i = 32; i < 42; ++i)
		{
			if((ret = sd_read(i, &buf)))
			{
				log_boot_P(PSTR("error code = %d\n"), ret);
			}
			memory_dump(i << 9, buf, sizeof(buf));
		}
	}

	serial_tx_str_P(PSTR("\nStarting RISC-V Emulator Kernel ...\n\n"));

	/* Infinite loop */
	for(;;)
	{
	}

	/* Unreachable */
	return 0;
}

