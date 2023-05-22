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
