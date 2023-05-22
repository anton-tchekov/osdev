/**
 * @file    xmem.c
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    01.05.2023
 */

#include <xmem.h>
#include <spi.h>
#include <logger.h>
#include <gpio.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdlib.h>

/** Do a simple checkerboard memory test. Comment this out to do a more
    thorough memory test using random numbers */
#define MEMTEST_SIMPLE

/** How many memory banks there are */
#define BANK_COUNT            3

/** The size of each memory bank as a power of two */
#define BANK_SIZE_POT        17

/** The size of each memory bank in bytes */
#define BANK_SIZE              0x20000UL

/** The interval in bytes at which to do progress updates in a memory test */
#define OUTPUT_INTERVAL        0x2000UL

/** Dummy SPI value when receiving */
#define DUMMY                  0xFF

/** Random seed for extended memory test */
#define SEED                 42

/** Write block command */
#define SRAM_COMMAND_WRITE    2

/** Read block command */
#define SRAM_COMMAND_READ     3

/* --- PRIVATE --- */

/**
 * @brief Start a transmission to the external memory
 *
 * @param bank Memory bank number
 * @param command Read/Write command
 * @param addr Start address
 */
static void _xmem_start(u8 bank, u8 command, u32 addr)
{
	XMEM_SELECT(bank);
	spi_xchg(command);
	spi_xchg(addr >> 16);
	spi_xchg(addr >> 8);
	spi_xchg(addr);
}

/**
 * @brief Perform a memory test on the external memoryy
 */
static void _memtest(void)
{
	u8 bank, w, v;
	u32 i;

	/* Run checkerboard memory test */
	log_boot_P(LOG_INIT, PSTR("Starting complete memory test"));

	spi_fast();
	for(bank = 0; bank < BANK_COUNT; ++bank)
	{
		log_boot_P(LOG_INIT, PSTR("Testing memory bank [%02d]"), bank + 1);

		/* Write */
		log_boot_P(LOG_INIT, PSTR("Writing pattern"));
		_xmem_start(bank, SRAM_COMMAND_WRITE, 0);
#ifdef MEMTEST_SIMPLE
		v = 0xAA;
#else
		srand(SEED);
#endif
		for(i = 0; ; ++i)
		{
#ifdef MEMTEST_SIMPLE
			v = ~v;
#else
			v = rand();
#endif
			if(i % OUTPUT_INTERVAL == 0)
			{
				log_boot_P(LOG_EXT, PSTR("0x%06lX"), i);
			}

			if(i >= BANK_SIZE)
			{
				break;
			}

			spi_xchg(v);
		}

		XMEM_DESELECT(bank);

		/* Read */
		log_boot_P(LOG_INIT, PSTR("Verifying pattern"));
		_xmem_start(bank, SRAM_COMMAND_READ, 0);
#ifdef MEMTEST_SIMPLE
		v = 0xAA;
#else
		srand(SEED);
#endif
		for(i = 0; ; ++i)
		{
#ifdef MEMTEST_SIMPLE
			v = ~v;
#else
			v = rand();
#endif
			if(i % OUTPUT_INTERVAL == 0)
			{
				log_boot_P(LOG_EXT, PSTR("0x%06lX"), i);
			}

			if(i >= BANK_SIZE)
			{
				break;
			}

			w = spi_xchg(DUMMY);
			if(w != v)
			{
				XMEM_DESELECT(bank);
				panic(PSTR(
					"Memory test failed at address 0x%06lX\n"
					"         [0x%02X != 0x%02X]"),
					i, w, v);
			}
		}

		XMEM_DESELECT(bank);
	}
}

/**
 * @brief Read data from a memory bank
 *
 * @param bank Bank number
 * @param addr Start address
 * @param data Buffer for the read data
 * @param size Number of bytes to read
 */
static void _xmem_read(u8 bank, u16 addr, void *data, u16 size)
{
	u16 i;
	u8 *data8 = data;
	_xmem_start(bank, SRAM_COMMAND_READ, addr);
	for(i = 0; i < size; ++i)
	{
		data8[i] = spi_xchg(DUMMY);
	}

	XMEM_DESELECT(bank);
}

/**
 * @brief Write data to a memory bank
 *
 * @param bank Bank number
 * @param addr Start address
 * @param data Buffer with the data to write
 * @param size Number of bytes to write
 */
static void _xmem_write(u8 bank, u16 addr, const void *data, u16 size)
{
	u16 i;
	const u8 *data8 = data;
	_xmem_start(bank, SRAM_COMMAND_WRITE, addr);
	for(i = 0; i < size; ++i)
	{
		spi_xchg(data8[i]);
	}

	XMEM_DESELECT(bank);
}

/**
 * @brief Set an area within a memory bank to 0
 *
 * @param bank Bank number
 * @param addr Start address
 * @param value Byte to write
 * @param size Number of bytes to write
 */
static void _xmem_set(u8 bank, u16 addr, u8 value, u16 size)
{
	u16 i;
	_xmem_start(bank, SRAM_COMMAND_WRITE, addr);
	for(i = 0; i < size; ++i)
	{
		spi_xchg(value);
	}

	XMEM_DESELECT(bank);
}

/**
 * @brief Calculate which bank an address falls onto
 *
 * @param addr Full memory address
 * @return Bank number
 */
static u8 _addr_to_bank(u32 addr)
{
	return addr >> BANK_SIZE_POT;
}

/**
 * @brief Calculate the offset from the start of a memory bank for an address
 *
 * @param addr Full memory address
 * @return Bank address
 */
static u16 _addr_bank_offset(u32 addr)
{
	return addr & (BANK_SIZE - 1);
}

/** Internal struct to handle reads and writes across bank boundaries */
typedef struct
{
	/** First bank to access */
	u8 BankFirst;

	/** Second bank to access */
	u8 BankSecond;

	/** Start address of the first part */
	u16 AddrFirst;

	/** Size of the first part */
	u16 SizeFirst;

	/** Size of the second part */
	u16 SizeSecond;
} AddrHelper;

/**
 * @brief Calculate which bank will be affected by a memory operation
 *
 * @param addr Start address
 * @param size Number of bytes affected
 * @param h Output information
 */
static void _xmem_overlap(u32 addr, u16 size, AddrHelper *h)
{
	u32 addr_end;

	h->AddrFirst = _addr_bank_offset(addr);
	addr_end = addr + size;

	h->BankFirst = _addr_to_bank(addr);
	h->BankSecond = _addr_to_bank(addr_end);
	if(h->BankFirst == h->BankSecond)
	{
		h->SizeFirst = size;
	}
	else
	{
		h->SizeSecond = _addr_bank_offset(addr_end);
		h->SizeFirst = size - h->SizeSecond;
	}
}

/* --- PUBLIC --- */
void xmem_init(void)
{
	/* Initialize XMEM */
	log_boot_P(LOG_INIT, PSTR("External memory driver initialized"));
	_memtest();
}

void xmem_read(u32 addr, void *data, u16 size)
{
	AddrHelper h;
	_xmem_overlap(addr, size, &h);
	_xmem_read(h.BankFirst, h.AddrFirst, data, h.SizeFirst);
	if(h.BankSecond != h.BankFirst)
	{
		_xmem_read(h.BankSecond, 0, (u8 *)data + h.SizeFirst, h.SizeSecond);
	}
}

void xmem_write(u32 addr, const void *data, u16 size)
{
	AddrHelper h;
	_xmem_overlap(addr, size, &h);
	_xmem_write(h.BankFirst, h.AddrFirst, data, h.SizeFirst);
	if(h.BankSecond != h.BankFirst)
	{
		_xmem_write(h.BankSecond, 0, (const u8 *)data + h.SizeFirst,
			h.SizeSecond);
	}
}

void xmem_set(u32 addr, u8 value, u16 size)
{
	AddrHelper h;
	_xmem_overlap(addr, size, &h);
	_xmem_set(h.BankFirst, h.AddrFirst, value, h.SizeFirst);
	if(h.BankSecond != h.BankFirst)
	{
		_xmem_set(h.BankSecond, 0, value, h.SizeSecond);
	}
}

u32 xmem_size(void)
{
	return BANK_COUNT * BANK_SIZE;
}
