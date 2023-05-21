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
#define SECTOR_INIT             (SECTOR_BOOT + SIZE_STDLIB)
#define SIZE_INIT            128

/* Root Dir */
#define SECTOR_ROOT             (SIZE_BOOT + SIZE_STDLIB + SIZE_INIT)

/* Current FS Revision */
#define REVISION               0

/* Boot sector offsets */
#define OFFSET_SIGNATURE       0
#define OFFSET_REVISION        4
#define OFFSET_STDLIB_SIZE    12
#define OFFSET_INIT_SIZE      16

/* TODO: STDLIB as shared library */
/* RAM offsets */
#define RAM_OFFSET_STDLIB       TODO
#define RAM_OFFSET_INIT        0

/* FS Signature (bytes 0-4) */
static const u8 _signature[] = { 'A', 'T', 'F', 'S' };

static void _load_xmem(u32 addr, u32 block, u32 count)
{
	u8 buf[BLOCK_SIZE];
	u32 i, end;

	end = block + count;
	for(i = block; i < end; ++i)
	{
		if(sd_read(i, buf))
		{
			panic(PSTR("Loading InitRD failed"));
		}

		xmem_write(addr, buf, BLOCK_SIZE);
	}
}

/**
 * @brief Load a 32-bit little endian value from a buffer
 *
 * @param buf Input
 * @return Result
 */
static u32 _load_le_32(u8 *buf)
{
	return (u32)(buf[0]) |
		((u32)(buf[1]) << 8) |
		((u32)(buf[2]) << 16) |
		((u32)(buf[3]) << 24);
}

void initrd_load(void)
{
	u32 size_stdlib, size_init;

	log_boot_P(LOG_INIT, PSTR("Loading Init Binary"));

	/* Conserve Stack */
	{
		u32 revision;
		u8 buf[BLOCK_SIZE];

		sd_read(SECTOR_BOOT, buf);

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
		size_init = _load_le_32(buf + OFFSET_STDLIB_SIZE);
	}

	/* Load STDLIB to address 0 */
	/* _load_xmem(RAM_OFFSET_STDLIB, SECTOR_STDLIB, size_stdlib >> BLOCK_SIZE_POT); */

	/* Load INIT */
	_load_xmem(RAM_OFFSET_INIT, SECTOR_INIT, size_init >> BLOCK_SIZE_POT);
}
