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

#endif /* __FS_H__ */