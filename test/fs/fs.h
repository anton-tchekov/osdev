#ifndef __FS_H__
#define __FS_H__

#include "status.h"
#include "types.h"
#include "syscall.h"

typedef u32 File;

/*
* @brief Open a file to get a fs_adress
* @param name Filename
* @param file fs_adress (pointer)
* @return Status
*/
Status fs_open(char* name, File *file);

/*
* @brief Read Data from File
* @param file fs_adress of file
* @param start Index of first Byte
* @param len Count of bytes to read
* @param buf Buffer with data (pointer)
* @return Status
*/
Status fs_read(File file, u32 start, u32 len, void* buf);

/*
* @brief Write data into file
* @param file fs_adress of file
* @param start Index of first byte to write
* @param len Count of bytes to write
* @param buf Buffer with data (pointer)
* @return Status
*/
Status fs_write(File file, u32 start, u32 len, const void* buf);

/*
* @brief Close an open file
* @param file fs_adress of file
* @return Status
*/
Status fs_close(File file);

/*
* @brief Gets the Size of a file.
* @param file fs_adress of file
* @param size pointer to the size
* @return Status
*/
Status fs_get_size(File file, u32* size);

#endif /* __FS_H__ */