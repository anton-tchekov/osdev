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

/**
 * @brief Initialize SD driver and try to detect card
 */
void sd_init(void);

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
