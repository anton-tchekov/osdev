/**
 * @file    spi.c
 * @author  Anton Tchekov
 * @version 0.1
 * @date    16.05.2023
 * @brief   SPI interface driver
 */

/** Dummy SPI value for receiving or sending */
#define SPI_DUMMY              0xFF

/**
 * @brief Initialize the SPI interface
 */
static void spi_init(void)
{
	SPCR = (1 << SPE) | (1 << MSTR);
}

/**
 * @brief Enable fastest SPI mode
 */
static void spi_fast(void)
{
	SPCR &= ~((1 << SPR1) | (1 << SPR0));
	SPSR |= (1 << SPI2X);
}

static u8 spi_xchg_fast(u8 byte)
{
	u8 cnt = 0;
	SPDR = byte;
	while(!(SPSR & (1 << SPIF)))
	{
		if(++cnt == 0xFF)
		{
			panic(PSTR("SPI timeout"));
		}
	}

	return SPDR;
}

/**
 * @brief Send and receive one byte from the SPI interface
 *        Panics on timeout
 *
 * @param byte The byte to send
 * @return The byte that was received
 */
static u8 spi_xchg(u8 byte)
{
	u16 cnt = 0;
	SPDR = byte;
	while(!(SPSR & (1 << SPIF)))
	{
		if(++cnt == 0xFFFF)
		{
			panic(PSTR("SPI timeout"));
		}
	}

	return SPDR;
}

/**
 * @brief Try to send and receive one byte from SPI
 *
 * @param in Input byte
 * @param out Output byte
 * @return Status
 */
static Status spi_xchg_try(u8 in, u8 *out)
{
	u16 cnt = 0;
	SPDR = in;
	while(!(SPSR & (1 << SPIF)))
	{
		if(cnt++ == 0xFFFF)
		{
			SD_DESELECT;
			return STATUS_TIMEOUT;
		}
	}

	*out = SPDR;
	return STATUS_OK;
}

/**
 * @brief Try to send one byte to the SPI interface
 *
 * @param in The byte to send
 * @return Status
 */
static Status spi_tx_try(u8 in)
{
	u8 dummy;
	return spi_xchg_try(in, &dummy);
}

/**
 * @brief Try to receive one byte from the SPI interface
 *
 * @param out Output byte
 * @return Status
 */
static Status spi_rx_try(u8 *out)
{
	return spi_xchg_try(0xFF, out);
}
