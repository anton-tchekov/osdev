#include "ps2.h"
#include "serial.h"
#include "adc.h"
#include "random.h"
#include "timer.h"
#include "rtc.h"
#include "xmem.h"
#include "logger.h"
#include "spi.h"
#include <avr/pgmspace.h>

static const char _msg_launch[] PROGMEM =
	"RISC-V VirtualOS Version 0.1 Booting ...";

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

int main(void)
{
	/* --- BOOT SEQUENCE --- */

	/* Initialize serial port */
	serial_init();

	/* Enable interrupts */
	sei();

	/* Print boot message */
	log_boot(_msg_launch);

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

	/* Initialize RTC */
	rtc_init();

	/* --- READY --- */

	/* Infinite loop */
	for(;;)
	{
	}

	/* Unreachable */
	return 0;
}

