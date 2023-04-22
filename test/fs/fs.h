#ifndef __FS_H__
#define __FS_H__

#include <status.h>
#include <types.h>
#include <syscall.h>

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
 * @brief Read Data from File
 *
 * @param file File handle
 * @param start Index of first Byte
 * @param len Number of bytes to read
 * @param buf Pointer to buffer for read data
 * @return Status
 */
Status file_read(File file, u32 start, u32 len, void *buf);

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
Status fs_close(File file);

/**
 * @brief Gets the size of a file in bytes
 *
 * @param file File handle
 * @param size Output parameter size
 * @return Status
 */
Status file_size(File file, u32* size);

#endif /* __FS_H__ */