#include "emulator/emulator.c"
#include <stdio.h>
#include <assert.h>

#define READ_SIZE 1024

int main(int argc, char **argv)
{
	int i;
	size_t len, offset;
	FILE *fp;
	Emulator emu;

	if(argc != 2)
	{
		fprintf(stderr, "Usage: ./emu rv32i-binary\n");
		return 1;
	}

	/* Load binary into memory */
	if(!(fp = fopen(argv[1], "rb")))
	{
		fprintf(stderr, "Failed to open file \"%s\"\n", argv[1]);
		return 1;
	}

	offset = 0;
	do
	{
		len = fread(_memory + offset, 1, READ_SIZE, fp);
		offset += len;
	}
	while(len == READ_SIZE);

	/* Program Counter starts at bottom */
	emu.PC = 0;

	/* Zero all Registers */
	for(i = 0; i < 32; ++i)
	{
		emu.Registers[i] = 0;
	}

	/* Initialize Stack Pointer at end of memory */
	emu.Registers[2] = 1024;
	while(!emulator_next(&emu))
	{
		registers_dump(&emu);
		memory_dump(0, 1024);
	}

	// run_tests();
	return 0;
}

