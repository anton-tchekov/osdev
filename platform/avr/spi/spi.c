/**
 * @file    spi.c
 * @author  Anton Tchekov
 * @version 0.1
 * @date    16.05.2023
 */

#include <spi.h>
#include <logger.h>
#include <avr/pgmspace.h>

void spi_init(void)
{
	SPCR = (1 << SPE) | (1 << MSTR);
	log_boot_P(LOG_INIT, PSTR("SPI initialized"));
}

void spi_fast(void)
{
	SPCR &= ~((1 << SPR1) | (1 << SPR0));
	SPSR |= (1 << SPI2X);
}

u8 spi_xchg(u8 byte)
{
	u16 cnt = 0;
	SPDR = byte;
	while(!(SPSR & (1 << SPIF)))
	{
		if(cnt++ == 0xFFFF)
		{
			panic(PSTR("SPI timeout"));
		}
	}

	return SPDR;
}

Status spi_xchg_try(u8 in, u8 *out)
{
	u32 cnt = 0;
	SPDR = in;
	while(!(SPSR & (1 << SPIF)))
	{
		if(cnt++ == 0xFFFF)
		{
			return STATUS_TIMEOUT;
		}
	}

	*out = SPDR;
	return STATUS_OK;
}

Status spi_tx_try(u8 in)
{
	u8 dummy;
	return spi_xchg_try(in, &dummy);
}

Status spi_rx_try(u8 *out)
{
	return spi_xchg_try(0xFF, out);
}
