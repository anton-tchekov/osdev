/* TODO: Documentation */

#include <spi.h>

u8 spi_xchg(u8 byte)
{
	SPDR = byte;
	while(!(SPSR & (1 << SPIF))) {}
	return SPDR;
}
