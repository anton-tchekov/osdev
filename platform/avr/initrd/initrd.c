/**
 * @file    initrd.c
 * @author  Anton Tchekov
 * @version 0.1
 * @date    21.05.2023
 */

#include <initrd.h>
#include <types.h>
#include <sd.h>
#include <xmem.h>
#include <logger.h>
#include <string.h>
#include <avr/pgmspace.h>

/* 512 bytes */
#define SECTOR_BOOT            0
#define SIZE_BOOT              1

/* 64 KiB */
#define SECTOR_STDLIB           SIZE_BOOT
#define SIZE_STDLIB          128

/* 64 KiB */
#define SECTOR_INIT             (SIZE_BOOT + SIZE_STDLIB)
#define SIZE_INIT            128

/* Root Dir */
#define SECTOR_ROOT             (SIZE_BOOT + SIZE_STDLIB + SIZE_INIT)

/* Current FS Revision */
#define REVISION               0

/* Boot sector offsets */
#define OFFSET_SIGNATURE       0
#define OFFSET_REVISION        4
#define OFFSET_STDLIB_SIZE     8
#define OFFSET_INIT_SIZE      12

/* TODO: STDLIB as shared library */
/* RAM offsets */
#define RAM_OFFSET_STDLIB       TODO
#define RAM_OFFSET_INIT        0

/* FS Signature (bytes 0-4) */
static const u8 _signature[] = { 'A', 'T', 'F', 'S' };

/**
 * @brief Load a 32-bit little endian value from a buffer
 *
 * @param buf Input
 * @return Result
 */
static u32 _load_le_32(u8 *buf)
{
	return (u32)(buf[0]) |
		((u32)(buf[1]) << 8UL) |
		((u32)(buf[2]) << 16UL) |
		((u32)(buf[3]) << 24UL);
}

void initrd_load(void)
{
	u8 buf[BLOCK_SIZE];
	u16 block;
	u32 addr, revision, size_stdlib, size_init;

	log_boot_P(LOG_INIT, PSTR("Loading Init Binary"));

	sd_read(SECTOR_BOOT, buf);

	memory_dump(0, buf, BLOCK_SIZE);

	/* Check signature */
	if(memcmp(buf + OFFSET_SIGNATURE, _signature, sizeof(_signature)))
	{
		panic(PSTR("Wrong FS signature"));
	}

	/* Check revision */
	revision = _load_le_32(buf + OFFSET_REVISION);
	if(revision != REVISION)
	{
		panic(PSTR("Unsupported ATFS revision"));
	}

	/* Get STDLIB Size */
	size_stdlib = _load_le_32(buf + OFFSET_STDLIB_SIZE);

	/* Get INIT Size */
	size_init = _load_le_32(buf + OFFSET_INIT_SIZE);

	/* Load STDLIB */
	/* _load_xmem(RAM_OFFSET_STDLIB, SECTOR_STDLIB, size_stdlib >> BLOCK_SIZE_POT); */

	log_boot_P(LOG_EXT, PSTR("INIT Size: %"PRIu32), size_init);
	log_boot_P(LOG_EXT, PSTR("STDLIB Size: %"PRIu32), size_stdlib);

	u16 num_sectors = (size_init + BLOCK_SIZE - 1) >> BLOCK_SIZE_POT;

	log_boot_P(LOG_EXT, PSTR("INIT Sectors: %d"), num_sectors);

	/* Load INIT */
	for(addr = 0, block = SECTOR_INIT;
		block < SECTOR_INIT + num_sectors;
		++block, addr += BLOCK_SIZE)
	{
		if(sd_read(block, buf))
		{
			panic(PSTR("Loading InitRD failed"));
		}

		xmem_write(addr, buf, BLOCK_SIZE);
	}
}
