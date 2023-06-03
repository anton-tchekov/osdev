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

/** File Handle */
typedef struct
{
	/** Starting block of the file */
	u32 StartBlock;

	/** Size of the file in blocks */
	u32 SizeBlocks;
} File;

/** Directory Handle */
typedef struct
{
	/** Number of entries in the directory */
	u32 NumEntries;

	/** Every directory is a file */
	File InternalFile;
} Dir;

/**
 * Directory entry: 64 bytes
 * - 4 byte address
 * - 4 byte size
 * - 54 byte file name
 * - 1 byte null terminator
 * - 1 byte type (directory = 0, text file = non-zero)
 */
typedef struct
{
	/** Starting block of the file */
	u32 StartBlock;

	/** Size of the file in blocks */
	u32 SizeBlocks;

	/** Name of the file */
	char Name[ATFS_MAX_FILE_NAME_LENGTH + 1];

	/** Directory/File type */
	u8 Type;
} DirEntry;

/**
 * @brief Join two paths by copying the second path to the end of the first
 *        path and adding a dot in between. The first string must have enough
 *        space to contain the whole path.
 *        The validity of the path is not checked.
 *
 * @param path First part of the path
 * @param append Path that should be added to the end of the first path
 * @return path
 */
char *path_join(char *path, const char *append);

/**
 * @brief Remove the last component of the path in place.
 *        The validity of the path is not checked.
 *
 * @param path The path to remove the last part of
 * @return path
 */
char *path_parent(char *path);

/**
 * @brief Check if a path is valid.
 *
 * @param path The path to check
 * @return true if valid, else false
 */
bool path_valid(const char *path);

/**
 * @brief Create a file. If the file already exists, it is resized to
 *        the specified size. When a file is resized, all file handles to it
 *        are invalidated and need to be reopened.
 *
 * @param path Path to file
 * @param size Capacity of the file in blocks
 * @return Status
 */
Status fs_fcreate(const char *path, u32 size);

/**
 * @brief Open a file to get a file handle. It is not neccessary to close
 *        a file, therefore there is no `fs_fclose` function.
 *
 * @param path Path to file
 * @param file Output parameter file handle
 * @return Status
 */
Status fs_fopen(const char *path, File *file);

/**
 * @brief Reads one block from the file.
 *
 * @param file File handle
 * @param block The block to read
 * @param buf Buffer for read data (512 bytes)
 * @return Status
 */
Status fs_fread(File *file, u32 block, void *buf);

/**
 * @brief Write one block to the file
 *
 * @param file File handle
 * @param block The block to write
 * @param buf Buffer with data to write (512 bytes)
 * @return Status
 */
Status fs_fwrite(File *file, u32 block, const void *buf);

/**
 * @brief Delete a file. This function does NOT work for directories.
 *
 * @param path Path to file
 * @return Status
 */
Status fs_fdelete(const char *path);

/**
 * @brief Move or rename a file or a directory.
 *
 * @param dest Destination path
 * @param src Source path
 * @return Status
 */
Status fs_frename(const char *dest, const char *src);

/**
 * @brief Open a directory
 *
 * @param path Path to directory
 * @param dir Output parameter directory handle
 * @return Status
 */
Status fs_dopen(const char *path, Dir *dir);

/**
 * @brief Read a directory entry
 *
 * @param dir Directory handle
 * @param index File index
 * @param entry Output parameter directory entry
 * @return Status
 */
Status fs_dread(Dir *dir, u32 index, DirEntry *entry);

#endif /* __FS_H__ */
