/**
 * @file    initrd.c
 * @author  Anton Tchekov
 * @version 0.1
 * @date    21.05.2023
 */

#include <atfs.h>
#include <initrd.h>
#include <types.h>
#include <sd.h>
#include <xmem.h>
#include <logger.h>
#include <string.h>
#include <util.h>
#include <avr/pgmspace.h>

/** RAM Offset of INIT Program */
#define RAM_OFFSET_INIT        0

void initrd_load(void)
{
	u8 buf[BLOCK_SIZE];
	u16 num_sectors;
	u32 addr, block, revision, start_init, size_init;

	log_boot_P(LOG_INIT, PSTR("Loading INIT Binary"));
	sd_read(ATFS_SECTOR_BOOT, buf);

	/* Check signature */
	if(memcmp(buf + ATFS_OFFSET_SIGNATURE,
		_atfs_signature, sizeof(_atfs_signature)))
	{
		panic(PSTR("Wrong FS signature"));
	}

	/* Check revision */
	revision = load_le_32(buf + ATFS_OFFSET_REVISION);
	if(revision != ATFS_REVISION)
	{
		panic(PSTR("Unsupported ATFS revision"));
	}

	/* Get INIT Start and Size */
	start_init = load_le_32(buf + ATFS_OFFSET_INIT_BLOCK);
	size_init = load_le_32(buf + ATFS_OFFSET_INIT_SIZE);

	log_boot_P(LOG_EXT, PSTR("INIT Start: %"PRIu32), start_init);
	log_boot_P(LOG_EXT, PSTR("INIT Size: %"PRIu32), size_init);

	num_sectors = (size_init + BLOCK_SIZE - 1) >> BLOCK_SIZE_POT;
	log_boot_P(LOG_EXT, PSTR("INIT Sectors: %" PRIu16), num_sectors);

	/* Load INIT */
	for(addr = RAM_OFFSET_INIT, block = start_init;
		block < start_init + num_sectors;
		++block, addr += BLOCK_SIZE)
	{
		if(sd_read(block, buf))
		{
			panic(PSTR("Loading INIT failed"));
		}

		xmem_write(addr, buf, BLOCK_SIZE);
	}
}
