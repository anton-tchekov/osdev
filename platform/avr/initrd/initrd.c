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
#include <avr/pgmspace.h>

/* TODO: Load STDLIB as shared library */

/** RAM Offset of STDLIB */
#define RAM_OFFSET_STDLIB      0

/** RAM Offset of INIT Program */
#define RAM_OFFSET_INIT        0

static const u8 _signature[] = { 'A', 'T', 'F', 'S' };

void initrd_load(void)
{
	u8 buf[BLOCK_SIZE];
	u16 block, num_sectors;
	u32 addr, revision, size_stdlib, size_init;

	log_boot_P(LOG_INIT, PSTR("Loading Init Binary"));
	sd_read(ATFS_SECTOR_BOOT, buf);

	/* Check signature */
	if(memcmp(buf + ATFS_OFFSET_SIGNATURE, _signature, sizeof(_signature)))
	{
		panic(PSTR("Wrong FS signature"));
	}

	/* Check revision */
	revision = load_le_32(buf + ATFS_OFFSET_REVISION);
	if(revision != ATFS_REVISION)
	{
		panic(PSTR("Unsupported ATFS revision"));
	}

	/* Get STDLIB Size */
	size_stdlib = load_le_32(buf + ATFS_OFFSET_STDLIB_SIZE);

	/* Get INIT Size */
	size_init = load_le_32(buf + ATFS_OFFSET_INIT_SIZE);

	/* Load STDLIB */
	/* _load_xmem(RAM_OFFSET_STDLIB, SECTOR_STDLIB, size_stdlib >> BLOCK_SIZE_POT); */

	log_boot_P(LOG_EXT, PSTR("INIT Size: %"PRIu32), size_init);
	log_boot_P(LOG_EXT, PSTR("STDLIB Size: %"PRIu32), size_stdlib);

	num_sectors = (size_init + BLOCK_SIZE - 1) >> BLOCK_SIZE_POT;
	log_boot_P(LOG_EXT, PSTR("INIT Sectors: %d"), num_sectors);

	/* Load INIT */
	for(addr = 0, block = ATFS_SECTOR_INIT;
		block < ATFS_SECTOR_INIT + num_sectors;
		++block, addr += BLOCK_SIZE)
	{
		if(sd_read(block, buf))
		{
			panic(PSTR("Loading InitRD failed"));
		}

		xmem_write(addr, buf, BLOCK_SIZE);
	}
}
