/**
 * @file    main.c
 * @author  Anton Tchekov
 * @version 0.1
 * @date    21.05.2023
 * @brief   Boot image creator
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <types.h>

/** Disk sector size */
#define BLOCK_SIZE           512

/* 512 bytes */

/** Boot sector offset in blocks */
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

/* FS Signature (bytes 0-4) */
static const u8 _signature[] = { 'A', 'T', 'F', 'S' };

/**
 * @brief Write a 32-bit little endian value to a buffer
 *
 * @param buf Input
 * @return Result
 */
static void _write_le_32(u8 *buf, u32 val)
{
	buf[0] = val & 0xFF;
	buf[1] = (val >> 8) & 0xFF;
	buf[2] = (val >> 16) & 0xFF;
	buf[3] = (val >> 24) & 0xFF;
}

int main(int argc, char **argv)
{
	u32 i, size;
	u8 buf[BLOCK_SIZE];
	FILE *in, *out;
	char *in_filename, *out_filename;

	if(argc != 3)
	{
		fprintf(stderr, "Usage: ./mkinitrd input-binary output-disk-image\n");
		return 1;
	}

	in_filename = argv[1];
	out_filename = argv[2];

	if(!(in = fopen(in_filename, "rb")))
	{
		fprintf(stderr, "Failed to open input file \"%s\"\n", in_filename);
		return 1;
	}

	if(!(out = fopen(out_filename, "wb")))
	{
		fprintf(stderr, "Failed to open output file \"%s\"\n", out_filename);
		return 1;
	}

	fseek(in, 0, SEEK_END);
	size = ftell(in);
	fseek(in, 0, SEEK_SET);

	memset(buf, 0, BLOCK_SIZE);

	memcpy(buf + OFFSET_SIGNATURE, _signature, sizeof(_signature));
	_write_le_32(buf + OFFSET_REVISION, REVISION);
	_write_le_32(buf + OFFSET_STDLIB_SIZE, 0);
	_write_le_32(buf + OFFSET_INIT_SIZE, size);

	fwrite(buf, 1, BLOCK_SIZE, out);

	/* Write 64 empty sectors */
	memset(buf, 0, BLOCK_SIZE);
	for(i = 0; i < SIZE_STDLIB; ++i)
	{
		fwrite(buf, 1, BLOCK_SIZE, out);
	}

	/* Write init program */
	for(i = 0; i < SIZE_INIT; ++i)
	{
		memset(buf, 0, BLOCK_SIZE);
		fread(buf, 1, BLOCK_SIZE, in);
		fwrite(buf, 1, BLOCK_SIZE, out);
	}

	/* TEMPORARY: End marker 0xFF root dir sector */
	memset(buf, 0xFF, BLOCK_SIZE);
	fwrite(buf, 1, BLOCK_SIZE, out);
	return 0;
}

