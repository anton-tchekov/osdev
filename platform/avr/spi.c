#define SPI_DIR               DDRB
#define SPI_OUT               PORTB

#define SPI_CS_0             2
#define SPI_MOSI             3
#define SPI_MISO             4
#define SPI_SCK              5

static u8 spi_xchg(u8 data)
{
	SPDR = data;
	while(!(SPSR & (1 << SPIF))) {}
	return SPDR;
}
