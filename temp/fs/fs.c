/**
 * @file    fs.c
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    03.06.2023
 */

#include "fs.h"
#include <ctype.h>
#include <string.h>

/** For testing */
static u8 _ramdisk[ATFS_BLOCK_SIZE * 128];

/* --- PRIVATE --- */
/**
 * @brief Get the size of the disk
 *
 * @param size Output parameter for size in blocks
 * @return Status
 */
static Status disk_size(u32 *size)
{
	*size = sizeof(_ramdisk) / ATFS_BLOCK_SIZE;
	return STATUS_OK;
}

/**
 * @brief Read one block from the disk
 *
 * @param block Block number to read from
 * @param buf Buffer for read data
 * @return Status
 */
static Status disk_read(u32 block, void *buf)
{
	/* TODO: Use system call */
	memcpy(buf, _ramdisk + block * ATFS_BLOCK_SIZE, ATFS_BLOCK_SIZE);
	return STATUS_OK;
}

/**
 * @brief Write one block to the disk
 *
 * @param block Block number to write to
 * @param buf Buffer with data to write
 * @return Status
 */
static Status disk_write(u32 block, const void *buf)
{
	/* TODO: Use system call */
	memcpy(_ramdisk + block * ATFS_BLOCK_SIZE, buf, ATFS_BLOCK_SIZE);
	return STATUS_OK;
}

/**
 * @brief Read directory entry in a endianness-independant way
 *
 * @param buf Data buffer
 * @param entry Directory entry output
 */
static void dir_entry_get(void *buf, DirEntry *entry)
{
	u8 *buf8 = buf;
	entry->StartBlock = load_le_32(buf8 + ATFS_DIR_ENTRY_OFFSET_START);
	entry->SizeBlocks = load_le_32(buf8 + ATFS_DIR_ENTRY_OFFSET_SIZE);
	memcpy(entry->Name, buf8 + ATFS_DIR_ENTRY_OFFSET_NAME,
		ATFS_MAX_FILE_NAME_LENGTH + 1);
	entry->Type = buf8[ATFS_DIR_ENTRY_OFFSET_TYPE];
}

/**
 * @brief Find a directory entry in a directory
 *
 * @param name
 * @param name_len
 * @param block
 * @param entry
 * @return Status
 */
static Status _find_entry(
	const char *name, u32 name_len, u32 block, DirEntry *entry)
{
	u8 buf[ATFS_BLOCK_SIZE];
	u32 i, num_entries, offset;

	RETURN_IF(disk_read(block, buf));
	num_entries = load_le_32(buf + ATFS_DIR_OFFSET_NUM_ENTRIES);
	offset = ATFS_DIR_ENTRY_SIZE;
	for(i = 0; i < num_entries; ++i)
	{
		if(offset == ATFS_BLOCK_SIZE)
		{
			offset = 0;
			++block;
			RETURN_IF(disk_read(block, buf));
		}

		dir_entry_get(buf + offset, entry);

		/* Compare name with each directory entry */
		if(strlen(entry->Name) == name_len &&
			!strncmp(name, entry->Name, name_len))
		{
			return STATUS_OK;
		}

		offset += ATFS_DIR_ENTRY_SIZE;
	}

	return STATUS_NO_SUCH_FILE_OR_DIR;
}

/**
 * @brief Allocate `size` contiguous blocks on disk
 *
 * @param size Number of blocks to allocate
 * @param start Output parameter starting block
 * @return Status
 */
static Status fs_alloc(u32 size, u32 *start)
{
	u8 buf[ATFS_BLOCK_SIZE];
	u32 cur, cur_size, prev, next;

	/* Find first area of sufficient size */

	// THIS HAS A BUG !!!
	cur = ATFS_SECTOR_BOOT;
	cur_size = 0;
	next = 1;
	while(next && cur_size < size)
	{
		prev = cur;
		RETURN_IF(disk_read(cur, buf));
		cur = load_le_32(buf + ATFS_FREE_OFFSET_NEXT);
		cur_size = load_le_32(buf + ATFS_FREE_OFFSET_SIZE);
	}
	// --------------------

	if(cur_size < size)
	{
		/* We have searched the entire chain */
		return STATUS_NO_SPACE_LEFT_ON_DEVICE;
	}
	else if(cur_size == size)
	{
		/* Perfect Fit: Make previous area point to next area */
		RETURN_IF(disk_read(prev, buf));
		write_le_32(buf + ATFS_FREE_OFFSET_NEXT, next);
		RETURN_IF(disk_write(prev, buf));
	}
	else /* cur_size > size */
	{
		/* Split area */
		u32 new_start, new_size;

		new_start = cur + size;
		new_size = cur_size - size;

		RETURN_IF(disk_read(prev, buf));
		write_le_32(buf + ATFS_FREE_OFFSET_NEXT, new_start);
		RETURN_IF(disk_write(prev, buf));

		write_le_32(buf + ATFS_FREE_OFFSET_SIZE, new_size);
		write_le_32(buf + ATFS_FREE_OFFSET_NEXT, next);
		RETURN_IF(disk_write(new_start, buf));
	}

	*start = cur;
	return STATUS_OK;
}

/**
 * @brief Free `count` blocks starting at `block`
 *
 * @param block Start block
 * @param count Number of blocks to free
 */
static void fs_free(u32 block, u32 count)
{
	/* TODO: Implement this */
	(void)block, (void)count;
}

/* --- PUBLIC --- */
char *path_join(char *path, const char *append)
{
	u32 len = strlen(path);
	path[len] = ATFS_DIR_SEPARATOR;
	strcpy(path + len + 1, append);
	return path;
}

char *path_parent(char *path)
{
	char c, *s, *last_sep;

	last_sep = path;
	for(s = path; (c = *s); ++s)
	{
		if(c == ATFS_DIR_SEPARATOR)
		{
			last_sep = s;
		}
	}

	*last_sep = '\0';
	return path;
}

bool path_valid(const char *path)
{
	bool was_sep;
	char c;
	const char *s;

	/* Initialize with true because path may not start with separator */
	was_sep = true;
	for(s = path; (c = *s); ++s)
	{
		if(c == ATFS_DIR_SEPARATOR)
		{
			if(was_sep)
			{
				/* Two separators in a row are not allowed */
				return false;
			}

			was_sep = true;
		}
		else if(isdigit(c))
		{
			if(was_sep)
			{
				/* Path component may not start with a digit */
				return false;
			}

			was_sep = false;
		}
		else if(c == '_' || islower(c))
		{
			was_sep = false;
		}
		else
		{
			/* Invalid character */
			return false;
		}
	}

	/* Path may not end with separator, but allow empty path */
	return !was_sep || *path == '\0';
}

Status fs_fcreate(const char *path, u32 size)
{
	#ifdef AAA
	Dir dir;
	DirEntry entry;
	if(_traverse(path, &dir))
	{
		u32 start;

		/* New file */
		RETURN_IF(fs_alloc(size, &start));
	}
	else if(file.SizeBlocks != size)
	{
		/* TODO: Resize file */
	}
	#endif

	return STATUS_OK;
}

bool fs_exists(const char *path)
{
	File f;
	return fs_fopen(path, &f);
}

Status fs_format(void)
{
	u32 size;
	u8 buf[ATFS_BLOCK_SIZE];

	RETURN_IF(disk_size(&size));

	/* Boot Sector */
	memcpy(buf + ATFS_OFFSET_SIGNATURE, _atfs_signature,
		sizeof(_atfs_signature));

	write_le_32(buf + ATFS_OFFSET_REVISION, ATFS_REVISION);
	write_le_32(buf + ATFS_OFFSET_INIT_BLOCK, 0);
	write_le_32(buf + ATFS_OFFSET_INIT_SIZE, 0);
	write_le_32(buf + ATFS_OFFSET_ROOT_BLOCK, 1);
	write_le_32(buf + ATFS_OFFSET_ROOT_SIZE, 1);

	write_le_32(buf + ATFS_FREE_OFFSET_NEXT, 2);
	write_le_32(buf + ATFS_FREE_OFFSET_SIZE, 0);

	RETURN_IF(disk_write(ATFS_SECTOR_BOOT, buf));

	/* Root Dir */
	memset(buf, 0, ATFS_BLOCK_SIZE);
	RETURN_IF(disk_write(1, buf));

	/* Free List */
	write_le_32(buf + ATFS_FREE_OFFSET_NEXT, 0);
	write_le_32(buf + ATFS_FREE_OFFSET_SIZE, size - 2);
	RETURN_IF(disk_write(2, buf));
	return STATUS_OK;
}

Status fs_fopen(const char *path, File *file)
{
	u8 buf[ATFS_BLOCK_SIZE];
	DirEntry entry;
	const char *s, *name;
	char c;
	u32 name_len;

	/* Check path format beforehand for simplicity */
	if(!path_valid(path))
	{
		return STATUS_PATH_FORMAT_INVALID;
	}

	/* Traverse directory structure starting in root directory */
	RETURN_IF(disk_read(ATFS_SECTOR_BOOT, buf));
	entry.StartBlock = load_le_32(buf + ATFS_OFFSET_ROOT_BLOCK);
	entry.SizeBlocks = load_le_32(buf + ATFS_OFFSET_ROOT_SIZE);
	s = path;
	while(*s)
	{
		/* Get next part of path string */
		name = s;
		for(; (c = *s) && c != ATFS_DIR_SEPARATOR; ++s) {}
		name_len = s - name;

		/* Search current directory */
		RETURN_IF(_find_entry(name, name_len, entry.StartBlock, &entry));

		/* If in the middle of a path */
		if(!*s)
		{
			/* Skip directory separator */
			++s;

			/* Found entry must be a directory */
			if(entry.Type)
			{
				return STATUS_NO_SUCH_FILE_OR_DIR;
			}
		}
	}

	/* Entry was found */
	file->StartBlock = entry.StartBlock;
	file->SizeBlocks = entry.SizeBlocks;
	return STATUS_OK;
}

Status fs_fread(File *file, u32 block, void *buf)
{
	if(block >= file->SizeBlocks)
	{
		return STATUS_OUT_OF_BOUNDS;
	}

	return disk_read(file->StartBlock + block, buf);
}

Status fs_fwrite(File *file, u32 block, const void *buf)
{
	if(block >= file->SizeBlocks)
	{
		return STATUS_OUT_OF_BOUNDS;
	}

	return disk_write(file->StartBlock + block, buf);
}

/* TODO */
Status fs_delete(const char *path)
{
	return STATUS_OK;
	(void)path;
}

/* TODO */
Status fs_rename(const char *dest, const char *src)
{
	/* Create new directory entries */

	/* Delete old directory entries */

	return STATUS_OK;
	(void)dest, (void)src;
}

/* TODO */
Status fs_copy(const char *dest, const char *src)
{
	return STATUS_OK;
	(void)dest, (void)src;
}

Status fs_dopen(const char *path, Dir *dir)
{
	u8 buf[ATFS_BLOCK_SIZE];

	RETURN_IF(fs_fopen(path, &dir->InternalFile));
	RETURN_IF(fs_fread(&dir->InternalFile, 0, buf));
	dir->NumEntries = load_le_32(buf);
	return STATUS_OK;
}

Status fs_dread(Dir *dir, u32 index, DirEntry *entry)
{
	u8 buf[ATFS_BLOCK_SIZE];
	u32 offset, block;

	if(index >= dir->NumEntries)
	{
		return STATUS_OUT_OF_BOUNDS;
	}

	/* Add one because the first directory entry is reserved for metadata */
	offset = (index + 1) * ATFS_DIR_ENTRY_SIZE;
	block = offset / ATFS_BLOCK_SIZE;
	offset -= block * ATFS_BLOCK_SIZE;
	RETURN_IF(fs_fread(&dir->InternalFile, block, buf));
	dir_entry_get(buf + offset, entry);
	return STATUS_OK;
}
