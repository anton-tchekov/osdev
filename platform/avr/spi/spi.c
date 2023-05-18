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
	SPI_DIR |= (1 << SPI_MOSI) | (1 << SPI_SCK);
	SPCR = (1 << SPE) | (1 << MSTR);
	log_boot_P(PSTR("SPI initialized"));
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
