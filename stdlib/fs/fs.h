/**
 * @file    fs.h
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    23.04.2023
 * @brief   File system access
 */

#ifndef __FS_H__
#define __FS_H__

#include <status.h>
#include <types.h>
#include <syscall.h>

typedef enum
{
	FILETYPE_DIR = "DIR",
	FILETYPE_TEXT = "TXT"

} FileType;

typedef struct
{
	char Device;
	u32 StartBlock, SizeBlocks;
} File;

typedef struct
{
	char Device;
	u32 NumFiles;
	File InternalFile;
} Dir;

typedef struct
{
	u32 Size, Type;
	char Name[64], Type;
} DirEntry;


/** File handle */
typedef u32 File;

/**
 * @brief Open a file to get a file handle
 *
 * @param name File name
 * @param file Output parameter file handle
 * @return Status
 */
Status file_open(const char *name, File *file);

/**
 * @brief Write data into file
 *
 * @param file File handle
 * @param start Index of first byte to write
 * @param len Number of bytes to write
 * @param buf Pointer to buffer with data to write
 * @return Status
 */
Status file_write(File file, u32 start, u32 len, const void *buf);

/**
 * @brief Close an open file
 *
 * @param file File handle
 * @return Status
 */
Status file_close(File file);

/**
 * @brief Gets the size of a file in bytes
 *
 * @param file File handle
 * @param size Output parameter size
 * @return Status
 */
Status file_size(File file, u32* size);

/**
 * Format a drive.
 *
 * @param dev Drive Letter
 */
StatusCode fs_format(char dev);

/**
 * Create a file.
 *
 * @param path File path
 */
StatusCode fs_fcreate(const char *path, u32 size);

StatusCode fs_fdelete(const char *path);
StatusCode fs_fopen(const char *path, File *file);

StatusCode fs_dopen(const char *path, Dir *dir);
StatusCode fs_dread(Dir *dir, i32 index, DirEntry *entry);

StatusCode fs_fresize(File *file, u32 new_size);
StatusCode fs_fread(File *file, u32 offset, u32 size, void *buffer);
StatusCode fs_fwrite(File *file, u32 offset, u32 size, const void *buffer);

#endif /* __FS_H__ */
