/**
 * @file    fs.c
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    23.04.2023
 */

#include "fs.h"
#include <atfs.h>

char *path_join(char *path, const char *append)
{
	u32 len = strlen(path);
	path[len] = '.';
	strcpy(path + len + 1, append);
	return path;
}

char *path_parent(char *path)
{
	char c, *s, *last_dot;

	last_dot = path;
	for(s = path; (c = *s); ++s)
	{
		if(c == '.')
		{
			last_dot = s;
		}
	}

	*last_dot = '\0';
	return path;
}

bool path_valid(const char *path)
{
	bool was_dot;
	char c;
	const char *s;

	/* Initialize with true because path may not start with dot */
	was_dot = true;
	for(s = path; (c = *s); ++s)
	{
		if(c == '.')
		{
			if(was_dot)
			{
				/* Two dots in a row are not allowed */
				return false;
			}

			was_dot = true;
		}
		else if(isdigit(c))
		{
			if(was_dot)
			{
				/* Path component may not start with a digit */
				return false;
			}

			was_dot = false;
		}
		else if(c == '_' || islower(c))
		{
			was_dot = false;
		}
		else
		{
			/* Invalid character */
			return false;
		}
	}

	/* Path may not end with dot, but allow empty path */
	return !was_dot || *path == '\0';
}

static Status disk_read(u32 block, void *buf)
{

}

Status fs_fcreate(const char *path, u32 size)
{
	/* LATER */
}

Status _find_entry(const char *name, u32 block, )
{
	for(i = 0; i < dir->NumEntries; ++i)
	{
		/* Compare name with each directory entry */
		RETURN_IF(disk_read(&dir, i, &entry));

		if(path_second)
		{
			if(!strcmp(path_second, entry->Name))
			{
				/* Must be a directory */
				if(entry->Type)
				{
					return NO_SUCH_FILE_OR_DIR;
				}

				return _traverse(path_remain);
			}
		}
		else
		{
			/* File was found */
			if(!strcmp(path_second, entry->Name))
			{
				file->StartBlock = entry->StartBlock;
				file->SizeBlocks = entry->SizeBlocks;
				return STATUS_OK;
			}
		}
	}

	return NO_SUCH_FILE_OR_DIR;
}

Status _traverse(const char *path, u32 start_block, u32 size_blocks, File *out)
{
	DirEntry entry;
	char c;
	char path_first[ATFS_MAX_FILE_NAME_LENGTH + 1],
		path_second[ATFS_MAX_FILE_NAME_LENGTH + 1];
	const char *s, *path_remain;

	/* Traverse directory structure */
	for(s = path; (c = *s) && c != '.'; ++s) {}

	while(path_remain)
	{
		if(_find())
	}

	return NO_SUCH_FILE_OR_DIR;
}

bool fs_exists(const char *path)
{
	File f;
	return fs_fopen(path, &f);
}

Status fs_fopen(const char *path, File *file)
{
	return _traverse(path, _fs->RootBlock, _fs->RootSize, &file);
}

Status fs_fread(File *file, u32 block, void *buf)
{
	if(block >= file->SizeBlocks)
	{
		return STATUS_OUT_OF_BOUNDS;
	}

	return disk_read(block, buf);
}

Status fs_fwrite(File *file, u32 block, const void *buf)
{
	/* LATER */
}

Status fs_fdelete(const char *path)
{
	/* LATER */
}

Status fs_frename(const char *dest, const char *src)
{
	/* LATER */
}

Status fs_dopen(const char *path, Dir *dir)
{
	u8 buf[ATFS_BLOCK_SIZE];
	RETURN_IF(fs_fopen(path, &dir->InternalFile));
	RETURN_IF(fs_fread(&dir->InternalFile, 0, buf));
	dir->NumEntries = load_le_32(buf);
	return STATUS_OK;
}

void dir_entry_get(void *buf, u32 offset, DirEntry *entry)
{

}

Status fs_dread(Dir *dir, u32 index, DirEntry *entry)
{
	u8 buf[ATFS_BLOCK_SIZE];
	u32 offset, block;

	/* Add one because the first directory entry is reserved for metadata */
	offset = (index + 1) * ATFS_DIR_ENTRY_SIZE;
	block = offset / ATFS_BLOCK_SIZE;
	offset -= block * ATFS_BLOCK_SIZE;
	RETURN_IF(fs_fread(&dir->InternalFile, block, buf));
	/**TODO: NOT ENDIAN SAFE*/
	memcpy(entry, buf + offset, ATFS_DIR_ENTRY_SIZE);
	return STATUS_OK;
}

#ifdef AAA

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

#endif
