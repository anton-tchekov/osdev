/**
 * @file    xmem.c
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    01.05.2023
 * @brief   External memory driver (23LC1024)
 */

/** The size of each memory bank in bytes */
#define XMEM_CHIP_SIZE                 0x20000UL

/** Chip address mask */
#define XMEM_CHIP_MASK                 0x1FFFFUL

/** How many memory chips there are */
#define XMEM_CHIP_COUNT               3

/** How many memory banks there are */
#define XMEM_BANK_COUNT               6

/** Do a simple checkerboard memory test. Comment this out to do a more
    thorough memory test using random numbers */
#define XMEM_TEST_SIMPLE

/** Get chip number for bank number */
#define BANK_TO_CHIP(BANK)             ((BANK) >> 1)

/** Write block command */
#define XMEM_COMMAND_WRITE            2

/** Read block command */
#define XMEM_COMMAND_READ             3

/** The interval in bytes at which to do progress updates in a memory test */
#define XMEM_TEST_OUTPUT_INTERVAL      0x4000UL

/** Random seed for extended memory test */
#define XMEM_TEST_SEED               42

/* --- PRIVATE --- */

/**
 * @brief Start a transmission to the external memory
 *
 * @param command Read/Write command
 * @param bank Memory chip number
 * @param addr Start address
 */
static void _xmem_start_bank(u8 command, u8 bank, u16 addr)
{
	XMEM_SELECT(bank >> 1);
	spi_xchg_fast(command);
	spi_xchg_fast(bank & 1);
	spi_xchg_fast(addr >> 8);
	spi_xchg_fast(addr);
}

/**
 * @brief Start a transmission to the external memory
 *
 * @param command Read/Write command
 * @param bank Memory chip number
 * @param addr Start address
 */
static void _xmem_end_bank(u8 bank)
{
	XMEM_DESELECT(bank >> 1);
}

/**
 * @brief Start a transmission to the external memory
 *
 * @param command Read/Write command
 * @param bank Memory chip number
 * @param addr Start address
 */
static void _xmem_start_chip(u8 command, u8 chip, u32 addr)
{
	XMEM_SELECT(chip);
	spi_xchg_fast(command);
	spi_xchg_fast(addr >> 16);
	spi_xchg_fast(addr >> 8);
	spi_xchg_fast(addr);
}

/**
 * @brief Perform a memory test on the external memoryy
 */
static void xmem_test(void)
{
	u8 chip, w, v;
	u32 i;

	/* Run checkerboard memory test */
	log_boot_P(LOG_INIT, PSTR("Starting complete memory test"));

	spi_fast();
	for(chip = 0; chip < XMEM_CHIP_COUNT; ++chip)
	{
		log_boot_P(LOG_INIT, PSTR("Testing memory chip [%02d]"), chip + 1);

		/* Write */
		log_boot_P(LOG_INIT, PSTR("Writing pattern"));
		_xmem_start_chip(XMEM_COMMAND_WRITE, chip, 0);
#ifdef XMEM_TEST_SIMPLE
		v = 0xAA;
#else
		srand(XMEM_TEST_SEED);
#endif
		for(i = 0; ; ++i)
		{
#ifdef XMEM_TEST_SIMPLE
			v = ~v;
#else
			v = rand();
#endif
			if(i % XMEM_TEST_OUTPUT_INTERVAL == 0)
			{
				log_boot_P(LOG_EXT, PSTR("0x%06lX"), i);
			}

			if(i >= XMEM_CHIP_SIZE)
			{
				break;
			}

			spi_xchg(v);
		}

		XMEM_DESELECT(chip);

		/* Read */
		log_boot_P(LOG_INIT, PSTR("Verifying pattern"));
		_xmem_start_chip(XMEM_COMMAND_READ, chip, 0);
#ifdef XMEM_TEST_SIMPLE
		v = 0xAA;
#else
		srand(XMEM_TEST_SEED);
#endif
		for(i = 0; ; ++i)
		{
#ifdef XMEM_TEST_SIMPLE
			v = ~v;
#else
			v = rand();
#endif
			if(i % XMEM_TEST_OUTPUT_INTERVAL == 0)
			{
				log_boot_P(LOG_EXT, PSTR("0x%06lX"), i);
			}

			if(i >= XMEM_CHIP_SIZE)
			{
				break;
			}

			w = spi_xchg(SPI_DUMMY);
			if(w != v)
			{
				XMEM_DESELECT(chip);
				panic(PSTR(
					"Memory test failed at address 0x%06lX\n"
					"         [0x%02X != 0x%02X]"),
					i, w, v);
			}
		}

		XMEM_DESELECT(chip);
	}
}

/**
 * @brief Initialize external memory and run memory test
 */
static void xmem_init(void)
{
	log_boot_P(LOG_INIT, PSTR("External memory driver initialized"));
	xmem_test();
}

/**
 * @brief Read data from a memory bank
 *
 * @param bank Bank number
 * @param addr Start address
 * @param data Buffer for the read data
 * @param size Number of bytes to read
 */
static void xmem_read(u8 bank, u16 addr, void *data, u16 size)
{
	u16 i;
	u8 *data8;

	data8 = data;
	_xmem_start_bank(XMEM_COMMAND_READ, bank, addr);
	for(i = 0; i < size; ++i)
	{
		data8[i] = spi_xchg_fast(SPI_DUMMY);
	}

	_xmem_end_bank(bank);
}

/**
 * @brief Write data to a memory bank
 *
 * @param bank Bank number
 * @param addr Start address
 * @param data Buffer with the data to write
 * @param size Number of bytes to write
 */
static void xmem_write(u8 bank, u16 addr, const void *data, u16 size)
{
	u16 i;
	const u8 *data8;

	data8 = data;
	_xmem_start_bank(XMEM_COMMAND_WRITE, bank, addr);
	for(i = 0; i < size; ++i)
	{
		spi_xchg_fast(data8[i]);
	}

	_xmem_end_bank(bank);
}

/**
 * @brief Set an area within a memory bank to 0
 *
 * @param bank Bank number
 * @param addr Start address
 * @param value Byte to write
 * @param size Number of bytes to write
 */
static void xmem_set(u8 bank, u16 addr, u8 value, u16 size)
{
	u16 i;
	_xmem_start_bank(XMEM_COMMAND_WRITE, bank, addr);
	for(i = 0; i < size; ++i)
	{
		spi_xchg_fast(value);
	}

	_xmem_end_bank(bank);
}


/**
 * @brief Read data from a memory chip
 *
 * @param chip Chip number
 * @param addr Start address
 * @param data Buffer for the read data
 * @param size Number of bytes to read
 */
static void xmem_read_chip(u8 chip, u32 addr, void *data, u16 size)
{
	u16 i;
	u8 *data8;

	data8 = data;
	_xmem_start_chip(XMEM_COMMAND_READ, chip, addr);
	for(i = 0; i < size; ++i)
	{
		data8[i] = spi_xchg_fast(SPI_DUMMY);
	}

	XMEM_DESELECT(chip);
}

/**
 * @brief Write data to a memory chip
 *
 * @param chip Chip number
 * @param addr Start address
 * @param data Buffer with the data to write
 * @param size Number of bytes to write
 */
static void xmem_write_chip(u8 chip, u32 addr, const void *data, u16 size)
{
	u16 i;
	const u8 *data8;

	data8 = data;
	_xmem_start_chip(XMEM_COMMAND_WRITE, chip, addr);
	for(i = 0; i < size; ++i)
	{
		spi_xchg_fast(data8[i]);
	}

	XMEM_DESELECT(chip);
}