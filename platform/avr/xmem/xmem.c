/**
 * @file    xmem.c
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    01.05.2023
 */

#include <xmem.h>
#include <spi.h>
#include <logger.h>
#include <avr/io.h>
#include <avr/pgmspace.h>

/** Write block command */
#define SRAM_COMMAND_WRITE    2

/** Read block command */
#define SRAM_COMMAND_READ     3

static const char _msg_xmem[] PROGMEM = "External memory initialized";

void xmem_init(void)
{
	/* TODO: Initialize XMEM */
	log_boot(_msg_xmem);
}

/* TODO: Reads and writes across bank boundaries */

#ifdef COMMENT

static u8 _bank_to_pin(u8 bank)
{
	static const u8 _bank_pins[] = { SPI_CS_0 };
	return _bank_pins[bank >> 1];
}

static void _xmem_select(u8 bank)
{
	/* SPI double speed */
	SPSR |= (1 << SPI2X);
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

#endif
