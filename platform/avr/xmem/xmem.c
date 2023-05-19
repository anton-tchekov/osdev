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
#include <stdlib.h>

#define XMEM_CS_DIR           DDRB
#define XMEM_CS_OUT           PORTB
#define XMEM_CS_0             0
#define XMEM_CS_1             1
#define XMEM_CS_2             2

#define BANK_COUNT            3
#define BANK_SIZE             0x20000UL
#define OUTPUT_INTERVAL       0x2000UL
#define DUMMY                 0xFF
#define SEED                  42

/** Write block command */
#define SRAM_COMMAND_WRITE    2

/** Read block command */
#define SRAM_COMMAND_READ     3

/* --- PRIVATE --- */
static void _xmem_select(u8 bank)
{
	XMEM_CS_OUT &= ~(1 << bank);
}

static void _configure_cs(void)
{
	/* Chip select pins output */
	XMEM_CS_DIR |= (1 << XMEM_CS_0) | (1 << XMEM_CS_1) | (1 << XMEM_CS_2);
}

static inline void _xmem_deselect_all(void)
{
	XMEM_CS_OUT |= (1 << XMEM_CS_0) | (1 << XMEM_CS_1) | (1 << XMEM_CS_2);
}

static inline void _xmem_deselect(u8 bank)
{
	XMEM_CS_OUT |= (1 << bank);
}

static void _xmem_start(u8 bank, u8 command, u32 addr)
{
	_xmem_select(bank);
	spi_xchg(command);
	spi_xchg(addr >> 16);
	spi_xchg(addr >> 8);
	spi_xchg(addr);
}

static void _memtest(void)
{
	u8 bank, w, v;
	u32 i;

	/* Run checkerboard memory test */
	log_boot_P(PSTR("Starting complete memory test"));

	spi_fast();
	for(bank = 0; bank < BANK_COUNT; ++bank)
	{
		log_boot_P(PSTR("Testing memory bank [%02d]"), bank + 1);

		/* Write */
		log_boot_P(PSTR("Writing pattern"));
		_xmem_start(bank, SRAM_COMMAND_WRITE, 0);
		/* srand(SEED); */
		v = 0xAA;
		for(i = 0; ; ++i)
		{
			v = ~v; /* rand(); */
			if(i % OUTPUT_INTERVAL == 0)
			{
				log_boot_P(PSTR("0x%06lX"), i);
			}

			if(i >= BANK_SIZE)
			{
				break;
			}

			spi_xchg(v);
		}

		_xmem_deselect(bank);

		/* Read */
		log_boot_P(PSTR("Verifying pattern"));
		_xmem_start(bank, SRAM_COMMAND_READ, 0);
		/* srand(SEED); */
		v = 0xAA;
		for(i = 0; ; ++i)
		{
			v = ~v; /* rand(); */
			if(i % OUTPUT_INTERVAL == 0)
			{
				log_boot_P(PSTR("0x%06lX"), i);
			}

			if(i >= BANK_SIZE)
			{
				break;
			}

			w = spi_xchg(DUMMY);
			if(w != v)
			{
				_xmem_deselect(bank);
				panic(PSTR(
					"Memory test failed at address 0x%06lX "
					"[0x%02X != 0x%02X]"),
					i, w, v);
			}
		}

		_xmem_deselect(bank);
	}
}

static void _xmem_read(u8 bank, u16 addr, void *data, u16 size)
{
	u16 i;
	u8 *data8 = data;
	_xmem_start(bank, SRAM_COMMAND_READ, addr);
	for(i = 0; i < size; ++i)
	{
		data8[i] = spi_xchg(DUMMY);
	}

	_xmem_deselect(bank);
}

static void _xmem_write(u8 bank, u16 addr, void *data, u16 size)
{
	u16 i;
	u8 *data8 = data;
	_xmem_start(bank, SRAM_COMMAND_WRITE, addr);
	for(i = 0; i < size; ++i)
	{
		spi_xchg(data8[i]);
	}

	_xmem_deselect(bank);
}

static void _xmem_set(u8 bank, u16 addr, u8 value, u16 size)
{
	u16 i;
	_xmem_start(bank, SRAM_COMMAND_WRITE, addr);
	for(i = 0; i < size; ++i)
	{
		spi_xchg(value);
	}

	_xmem_deselect(bank);
}

/* --- PUBLIC --- */
void xmem_init(void)
{
	/* Initialize XMEM */
	_configure_cs();
	_xmem_deselect_all();
	log_boot_P(PSTR("External memory driver initialized"));
	_memtest();
}

void xmem_read(u32 addr, void *data, u16 size)
{
}

void xmem_write(u32 addr, const void *data, u16 size)
{
}

void xmem_set(u32 addr, u8 value, u16 size)
{
}
