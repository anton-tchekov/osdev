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
	serial_tx('0');
	serial_tx('x');
	serial_tx(byte_to_hex((byte >> 4) & 0x0F));
	serial_tx(byte_to_hex(byte & 0x0F));
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

int main(void)
{
	/* --- BOOT SEQUENCE --- */

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

	/* Initialize ADC */
	adc_init();

	/* Initialize RNG */
	random_init();

	/* Initialize keyboard */
	ps2_init();

	/* Initialize RTC driver */
	rtc_init();

	/* Initialize LCD driver */
	lcd_init();

	/* Initialize SD card driver */
	sd_init();

	serial_tx_str_P(PSTR("\nREADY.\n\n"));

	/* --- READY --- */

	/* Infinite loop */
	for(;;)
	{
	}

	/* Unreachable */
	return 0;
}

