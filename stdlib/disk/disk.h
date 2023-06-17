/**
 * @file    disk.h
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    07.06.2023
 * @brief   Disk I/O functions
 */

#ifndef __DISK_H__
#define __DISK_H__

#include <types.h>
#include <status.h>

/**
 * @brief Get the number of drives
 *
 * @return Number of drives
 */
u32 disk_count(void);

/**
 * @brief Get the size of the disk
 *
 * @param id Disk ID
 * @return Size in blocks
 */
u32 disk_size(u32 id);

/**
 * @brief Read one block from the disk
 *
 * @param id Disk ID
 * @param block Block number to read from
 * @param buf Buffer for read data
 * @return Status
 */
Status disk_read(u32 id, u32 block, void *buf);

/**
 * @brief Write one block to the disk
 *
 * @param id Disk ID
 * @param block Block number to write to
 * @param buf Buffer with data to write
 * @return Status
 */
Status disk_write(u32 id, u32 block, const void *buf);

#endif /* __DISK_H__ */
