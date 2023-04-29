#include "spi.c"
#include "uart.c"

#include <assert.h>
#include <string.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#define SRAM_COMMAND_WRITE   2
#define SRAM_COMMAND_READ    3

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

/* PRIVATE */
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

/* PUBLIC */
static void xmem_init(void)
{
	SPI_DIR = (1 << SPI_MOSI) | (1 << SPI_SCK) |
			(1 << SPI_CS_0);
	SPCR = (1 << SPE) | (1 << MSTR);
	SPSR = (1 << SPI2X);
}

static void xmem_read(u8 bank, u16 addr, void *data, u16 size)
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

static void xmem_write(u8 bank, u16 addr, void *data, u16 size)
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

static void xmem_set(u8 bank, u16 addr, u8 value, u16 size)
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

/* Checkerboard Test */
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

/* 128 KiB, split in 2 banks of 64 KiB */
static u8 _output[2 * 64 * 1024];

/* PRIVATE */
static u8 *_bank_ptr(u8 bank)
{
	return _output + 0x10000 * bank;
}

static u8 *_calculate_ptr(u8 bank, u16 addr)
{
	return _bank_ptr(bank) + addr;
}

/* PUBLIC */
void xmem_read(u8 bank, u16 addr, void *data, u16 size)
{
	memcpy(data, _calculate_ptr(bank, addr), size);
}

void xmem_write(u8 bank, u16 addr, void *data, u16 size)
{
	memcpy(_calculate_ptr(bank, addr), data, size);
}

/* READ ADDRESS */
u8 memory_r8(u8 bank, u16 addr)
{
	u8 ret;
	xmem_read(bank, addr, &ret, 1);
	return ret;
}

u16 memory_r16(u8 bank, u16 addr)
{
	u16 ret;
	xmem_read(bank, addr, &ret, 2);
	return ret;
}

u32 memory_r32(u8 bank, u16 addr)
{
	u32 ret;
	xmem_read(bank, addr, &ret, 4);
	return ret;
}

/* WRITE ADDRESS */
void memory_w8(u8 bank, u16 addr, u8 val)
{
	xmem_write(bank, addr, &val, 1);
}

void memory_w16(u8 bank, u16 addr, u16 val)
{
	xmem_write(bank, addr, &val, 2);
}

void memory_w32(u8 bank, u16 addr, u32 val)
{
	xmem_write(bank, addr, &val, 4);
}

/* BULK */
#define CHUNK_SIZE 256

void memory_cpy(u8 bank, u16 dest, u16 src, u16 count)
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

void memory_set(u8 bank, u16 addr, u8 value, u16 size)
{
	memset(_calculate_ptr(bank, addr), value, size);
}

static void timer_init(void)
{
	/* TODO */
}

static u32 millis(void)
{
	/* TODO */
	return 0;
}

#define UART_BAUD        9600
#define _BAUD                (((F_CPU / (UART_BAUD * 16UL))) - 1)

static void uart_init(void)
{
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
	UBRR0 = _BAUD;
}

static void uart_tx(char c)
{
	while(!(UCSR0A & (1 << UDRE0))) {}
	UDR0 = c;
}



