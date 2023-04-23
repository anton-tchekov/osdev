#include "ramdisk.h"
#include "types.h"
#include "status.h"
#include <stdlib.h>

/* --- PRIVATE --- */
#define BLOCK_SIZE   512
#define BLOCK_COUNT 1024

static u8 _disk[BLOCK_COUNT * BLOCK_SIZE];

static StatusCode _ramdisk_read(u32 offset, u32 count, u8 *buffer)
{
	memcpy(buffer, _disk + offset, count);
}

static StatusCode _ramdisk_write(u32 offset, u32 count, u8 *buffer)
{
	memcpy(_disk + offset, buffer, count);
}

static BlockDevice _ramdisk =
{
	.BlockSize = BLOCK_SIZE,
	.BlockCount = BLOCK_COUNT,
	.Read = _ramdisk_read,
	.Write = _ramdisk_write
};

/* --- PUBLIC --- */
void ramdisk_init(void)
{
	dev_attach(&_ramdisk);
}

