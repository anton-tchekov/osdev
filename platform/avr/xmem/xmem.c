/**
 * @file    xmem.c
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    01.05.2023
 */

#include <xmem.h>

#define SRAM_COMMAND_WRITE    2
#define SRAM_COMMAND_READ     3
#define CHUNK_SIZE          256

static uint8_t xram_memtest(void)
{
	uint8_t w, v;
	uint32_t i;

	stream_fputs(0, "Writing...\n");

	/* Write */
	SPI_OUT &= ~(1 << SPI_CS_0);
	spi_xchg(SRAM_COMMAND_WRITE);
	spi_xchg(0);
	spi_xchg(0);
	spi_xchg(0);

	v = 0xAA;
	for(i = 0; i < 0x1FFFFUL; ++i)
	{
		spi_xchg(v);
		v = ~v;
	}

	SPI_OUT |= (1 << SPI_CS_0);

	stream_fputs(0, "Reading...\n");

	/* Read */
	SPI_OUT &= ~(1 << SPI_CS_0);
	spi_xchg(SRAM_COMMAND_READ);
	spi_xchg(0);
	spi_xchg(0);
	spi_xchg(0);
	v = 0xAA;
	for(i = 0; i < 0x1FFFFUL; ++i)
	{
		w = spi_xchg(0xFF);
		if(v != w)
		{
			SPI_OUT |= (1 << SPI_CS_0);
			return 1;
		}

		v = ~v;
	}

	SPI_OUT |= (1 << SPI_CS_0);
	return 0;
}


static u8 _bank_to_pin(u8 bank)
{
	static const u8 _bank_pins[] = { SPI_CS_0 };
	return _bank_pins[bank >> 1];
}

static void _xmem_select(u8 bank)
{
	SPI_OUT &= ~(1 << _bank_to_pin(bank));
}

static void _xmem_deselect(u8 bank)
{
	SPI_OUT |= (1 << _bank_to_pin(bank));
}

static void _xmem_addr(u8 bank, u16 addr)
{
	spi_xchg(bank & 1);
	spi_xchg(addr >> 8);
	spi_xchg(addr & 0xFF);
}

void xmem_init(void)
{
	SPI_DIR = (1 << SPI_MOSI) | (1 << SPI_SCK) |
			(1 << SPI_CS_0);
	SPCR = (1 << SPE) | (1 << MSTR);
	SPSR = (1 << SPI2X);
}

void xmem_read(u32 addr, void *data, u16 size)
{
	u16 i;
	u8 *data8;
	_xmem_select(bank);
	data8 = (u8 *)data;
	spi_xchg(SRAM_COMMAND_READ);
	_xmem_addr(bank, addr);
	for(i = 0; i < size; ++i)
	{
		data8[i] = spi_xchg(0xFF);
	}

	_xmem_deselect(bank);
}

void xmem_write(u32 addr, const void *data, u16 size)
{
	u16 i;
	u8 *data8;
	_xmem_select(bank);
	data8 = (u8 *)data;
	spi_xchg(SRAM_COMMAND_WRITE);
	_xmem_addr(bank, addr);
	for(i = 0; i < size; ++i)
	{
		spi_xchg(data8[i]);
	}

	_xmem_deselect(bank);
}

void xmem_set(u32 addr, u8 value, u16 size)
{
	u16 i;
	_xmem_select(bank);
	spi_xchg(SRAM_COMMAND_WRITE);
	_xmem_addr(bank, addr);
	for(i = 0; i < size; ++i)
	{
		spi_xchg(value);
	}

	_xmem_deselect(bank);
}

void xmem_cpy(u32 dest, u32 src, u32 count)
{
	/* Handle overlap (like memmove), if src == dest, do nothing */
	if(dest < src)
	{
		/* Copy direction: Low to high */
		u16 cur;
		u8 chunk[CHUNK_SIZE];

		while(count)
		{
			cur = count > CHUNK_SIZE ? CHUNK_SIZE : count;
			xmem_read(bank, src, chunk, cur);
			xmem_write(bank, dest, chunk, cur);
			src += cur;
			dest += cur;
			count -= cur;
		}
	}
	else if(dest > src)
	{
		/* Copy direction: High to low addresses */
		u16 cur;
		u8 chunk[CHUNK_SIZE];

		src += count;
		dest += count;
		while(count)
		{
			cur = count > CHUNK_SIZE ? CHUNK_SIZE : count;
			src -= cur;
			dest -= cur;
			xmem_read(bank, src, chunk, cur);
			xmem_write(bank, dest, chunk, cur);
			count -= cur;
		}
	}
}

i8 memory_cmp(u8 bank, u16 a, u16 b, u16 count)
{
	i8 ret;
	u16 cur;
	u8 chunk_a[CHUNK_SIZE],
		chunk_b[CHUNK_SIZE];

	while(count)
	{
		cur = count > CHUNK_SIZE ? CHUNK_SIZE : count;
		xmem_read(bank, a, chunk_a, cur);
		xmem_read(bank, b, chunk_b, cur);
		if((ret = memcmp(chunk_a, chunk_b, cur)))
		{
			return ret;
		}

		count -= cur;
		a += cur;
		b += cur;
	}

	return 0;
}

u16 memory_chr(u8 bank, u16 addr, u8 value, u16 count)
{
	u16 cur, ret;
	u8 chunk[CHUNK_SIZE];

	while(count)
	{
		cur = count > CHUNK_SIZE ? CHUNK_SIZE : count;
		xmem_read(bank, addr, chunk, cur);
		if((ret = ((u8 *)memchr(chunk, value, cur) - chunk)))
		{
			return ret;
		}

		count -= cur;
		addr += cur;
	}

	return 0;
}
