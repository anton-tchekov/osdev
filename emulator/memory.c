#include "types.h"
#include <stdio.h>

static u8 _memory[1024];

/* Store */
static inline void memory_sb(u32 address, u32 value)
{
	i8 value8 = (i8)value;
	if(address >= sizeof(_memory))
	{
		return;
	}

	_memory[address] = value8;
}

static inline void memory_sh(u32 address, u32 value)
{
	i16 value16 = (i16)value;
	address >>= 1;
	if(address >= (sizeof(_memory) / 2))
	{
		return;
	}

	((i16 *)_memory)[address] = value16;
}

static inline void memory_sw(u32 address, u32 value)
{
	address >>= 2;
	if(address >= (sizeof(_memory) / 4))
	{
		return;
	}

	((u32 *)_memory)[address] = value;
}

/* Load */
static inline u32 memory_lb(u32 address)
{
	if(address >= sizeof(_memory))
	{
		return 0;
	}

	return (i32)(((i8 *)_memory)[address]);
}

static inline u32 memory_lh(u32 address)
{
	address >>= 1;
	if(address >= (sizeof(_memory) / 2))
	{
		return 0;
	}

	return ((i16 *)_memory)[address];
}

static inline u32 memory_lw(u32 address)
{
	address >>= 2;
	if(address >= (sizeof(_memory) / 4))
	{
		return 0;
	}

	return ((u32 *)_memory)[address];
}

static inline u32 memory_lbu(u32 address)
{
	return _memory[address];
}

static inline u32 memory_lhu(u32 address)
{
	address >>= 1;
	return ((u16 *)_memory)[address];
}

/* DUMP */
static void memory_dump(u32 address, u32 length)
{
	u32 start, end, i;
	start = (address >> 4) << 4;
	end = address + length;
	for(; start < end; start += 16)
	{
		printf("%08X | ", start);
		for(i = 0; i < 16; ++i)
		{
			printf("%02X ", _memory[start + i]);
		}

		printf("| ");
		for(i = 0; i < 16; ++i)
		{
			char c = _memory[start + i];
			printf("%c", (c < 32 || c > 126) ? '.' : c);
		}

		printf("\n");
	}

	printf("\n");
}
