/**
 * @file    fs.c
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    23.04.2023
 */

#include "fs.h"

//! @cond Doxygen_Suppress

/* Allocate `size` contiguous blocks on device `dev` */
static StatusCode fs_alloc(u8 dev, u32 size, u32 *start)
{
	u8 buf[8];
	u32 prev, block, next, avail;

	/* Get first free block */
	RETURN_IF(dev_read(dev, 0, 0, 4, buf));
	block = _read_32(buf);

	/* Find first area of sufficient size */
	prev = 0;
	while(block && avail < size)
	{
		prev = block;
		block = next;
		RETURN_IF(dev_read(dev, block, 0, 8, buf));
		next = _read_32(buf);
		avail = _read_32(buf + 4);
	}

	if(avail < size)
	{
		return ERROR_FS_FULL;
	}

	if(avail == size)
	{
		/* Link previous and next area */

	}
	else
	{
		/* Split area */
		u32 new_start, new_size, next_start;

		new_start = block + size;
		new_size = avail - size;

		/* Make previous block point to new block */
		_write_32(buf, new_start);
		RETURN_IF(dev_write(dev, prev_start, 0, 4, buf));

		/* Make new block point to next block */
		_write_32(buf, next_start);
		_write_32(buf + 4, new_size);
		RETURN_IF(dev_write(dev, new_start, 0, 8, buf));
	}

	*start = block;
	return SUCCESS;
}

/* Frees `count` blocks starting at `block` */
static void fs_free(u8 dev, u32 block, u32 count)
{

}

static StatusCode _file_bounds_check(File *file, u32 offset, u32 size)
{
	u8 block_size = dev_block_size(file->Device);
	if((offset < (file->StartBlock << block_size)) ||
		(offset + size >= ((file->StartBlock + file->BlockSize) << block_size)))
	{
		return ERROR_FS_OUT_OF_BOUNDS;
	}

	return SUCCESS;
}

/* --- PUBLIC --- */
StatusCode fs_format(char dev)
{
	u8 buf[8];
	u32 start, next, blocks, size;

	blocks = dev_block_count(dev);

	/* First free block */
	start = 1;
	_write_32(buf, start);

	/* Write block 0 */
	dev_write(dev, 0, 0, 4, buf);

	/* No next block */
	next = 0;
	_write_32(buf, next);

	/* Area size */
	size = blocks - 1;
	_write_32(buf + 4, size);

	/* Write block 1 */
	dev_write(dev, 0, 0, 8, buf);
}

StatusCode fs_fcreate(const char *path, u32 size)
{
	RETURN_IF(fs_alloc(dev, size, ));
}

StatusCode fs_fdelete(const char *path)
{
	RETURN_IF(fs_free(path[0], block, count));
}

StatusCode fs_fopen(const char *path, File *file)
{
	file->Device = path[0];
}

StatusCode fs_fread(File *file, u32 offset, u32 size, void *buffer)
{
	u8 block_size = dev_block_size(file->Device);
	RETURN_IF(_file_bounds_check(file, offset, size));
	return dev_read(file->Device, (file->StartBlock << block_size) + offset, size, buffer);
}

StatusCode fs_fwrite(File *file, u32 offset, u32 size, const void *buffer)
{
	u8 block_size = dev_block_size(file->Device);
	RETURN_IF(_file_bounds_check(file, offset, size));
	return dev_write(file->Device, (file->StartBlock << block_size) + offset, size, buffer);
}

Status file_open(const char *name, File *file)
{
	return syscall2(SYSCALL_FILE_OPEN, (u32)name, (u32)file);
}

Status file_write(File file, u32 start, u32 len, const void *buf)
{
	return syscall4(SYSCALL_FILE_WRITE, file, start, len, (u32)buf);
}

Status file_close(File file)
{
	return syscall1(SYSCALL_FILE_CLOSE, file);
}

Status file_size(File file, u32 *size)
{
	return syscall2(SYSCALL_FILE_SIZE, file, (u32)size);
}

//! @endcond
