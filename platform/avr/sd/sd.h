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
 * @brief
 *
 */
void sd_init(void);

/**
 * @brief TODO
 *
 * @param block
 * @param data
 * @return Status
 */
Status sd_read(u32 block, void *data);

/**
 * @brief TODO
 *
 * @param block
 * @param data
 * @return Status
 */
Status sd_write(u32 block, const void *data);

#endif /* __SD_H__ */
