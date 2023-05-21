/**
 * @file    sd.h
 * @author  Anton Tchekov
 * @version 0.1
 * @date    18.05.2023
 * @brief   SD card driver
 */

#ifndef __SD_H__
#define __SD_H__

#include <types.h>
#include <status.h>

#define BLOCK_SIZE             512
#define BLOCK_SIZE_POT           9

/**
 * @brief Initialize SD driver and try to detect card
 */
void sd_init(void);

/**
 * @brief Get SD card capacity
 *
 * @return Capacity in 512-byte blocks
 */
u32 sd_size(void);

/**
 * @brief Read a single 512-byte block from the SD card
 *
 * @param block SD card block id
 * @param data Data output
 * @return Read status
 */
Status sd_read(u32 block, void *data);

/**
 * @brief Write a single 512-byte block to the SD card
 *
 * @param block SD card block id
 * @param data Data input
 * @return Write status
 */
Status sd_write(u32 block, const void *data);

#endif /* __SD_H__ */
