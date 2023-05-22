/**
 * @file    atfs.h
 * @author  Anton Tchekov
 * @version 0.1
 * @date    22.05.2023
 *
 * @brief ATFS File System
 */

#ifndef __ATFS_H__
#define __ATFS_H__

#include <types.h>

/** Disk sector size */
#define ATFS_BLOCK_SIZE           512

/* 512 bytes */

/** Boot sector offset in blocks */
#define ATFS_SECTOR_BOOT            0
#define ATFS_SIZE_BOOT              1

/* 64 KiB */
#define ATFS_SECTOR_STDLIB           ATFS_SIZE_BOOT
#define ATFS_SIZE_STDLIB          128

/* 64 KiB */
#define ATFS_SECTOR_INIT             (ATFS_SIZE_BOOT + ATFS_SIZE_STDLIB)
#define ATFS_SIZE_INIT            128

/* Root Dir */
#define ATFS_SECTOR_ROOT             (ATFS_SIZE_BOOT + ATFS_SIZE_STDLIB + ATFS_SIZE_INIT)

/* Current FS Revision */
#define ATFS_REVISION               0

/* Boot sector offsets */
#define ATFS_OFFSET_SIGNATURE       0
#define ATFS_OFFSET_REVISION        4
#define ATFS_OFFSET_STDLIB_SIZE     8
#define ATFS_OFFSET_INIT_SIZE      12

/* FS Signature (bytes 0-4) */
static const u8 _atfs_signature[] = { 'A', 'T', 'F', 'S' };

/**
 * @brief Write a 32-bit little endian value to a buffer
 *
 * @param buf Input
 * @return Result
 */
static void write_le_32(u8 *buf, u32 val)
{
	buf[0] = val & 0xFF;
	buf[1] = (val >> 8) & 0xFF;
	buf[2] = (val >> 16) & 0xFF;
	buf[3] = (val >> 24) & 0xFF;
}

/**
 * @brief Load a 32-bit little endian value from a buffer
 *
 * @param buf Input
 * @return Result
 */
static u32 load_le_32(u8 *buf)
{
	return (u32)(buf[0]) |
		((u32)(buf[1]) << 8UL) |
		((u32)(buf[2]) << 16UL) |
		((u32)(buf[3]) << 24UL);
}

#endif /* __ATFS_H__ */
