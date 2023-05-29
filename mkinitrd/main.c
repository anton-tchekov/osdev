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
#include <atfs.h>

/**
 * @brief MkInitRD Main
 *
 * @param argc Number of command line parameters
 * @param argv Array of command line parameters
 * @return Exit code
 */
int main(int argc, char **argv)
{
	u32 i, size;
	u8 buf[ATFS_BLOCK_SIZE];
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

	memset(buf, 0, ATFS_BLOCK_SIZE);

	memcpy(buf + ATFS_OFFSET_SIGNATURE, _atfs_signature, sizeof(_atfs_signature));
	write_le_32(buf + ATFS_OFFSET_REVISION, ATFS_REVISION);
	write_le_32(buf + ATFS_OFFSET_STDLIB_SIZE, 0);
	write_le_32(buf + ATFS_OFFSET_INIT_SIZE, size);

	fwrite(buf, 1, ATFS_BLOCK_SIZE, out);

	/* Temporarily write empty sectors instead of stdlib */
	memset(buf, 0, ATFS_BLOCK_SIZE);
	for(i = 0; i < ATFS_SIZE_STDLIB; ++i)
	{
		fwrite(buf, 1, ATFS_BLOCK_SIZE, out);
	}

	/* Write init program */
	for(i = 0; i < ATFS_SIZE_INIT; ++i)
	{
		memset(buf, 0, ATFS_BLOCK_SIZE);
		fread(buf, 1, ATFS_BLOCK_SIZE, in);
		fwrite(buf, 1, ATFS_BLOCK_SIZE, out);
	}

	/* Temporary end marker 0xFF in root dir sector */
	memset(buf, 0xFF, ATFS_BLOCK_SIZE);
	fwrite(buf, 1, ATFS_BLOCK_SIZE, out);
	return 0;
}
