/**
 * @file    dev.h
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    01.05.2023
 * @brief   Block device interface
 */

#ifndef __DEV_H__
#define __DEV_H__

//! @cond Doxygen_Suppress

#include <types.h>
#include <status.h>

/** Block device structure */
typedef struct
{
	i32 BlockSize, BlockCount;
	StatusCode (*Read)(u32 offset, u32 count, u8 *buffer);
	StatusCode (*Write)(u32 offset, u32 count, u8 *buffer);
} BlockDevice;

/** TODO */
char dev_attach(BlockDevice *dev);

/** TODO */
void dev_detach(char letter);

/** TODO */
StatusCode dev_read(char dev, u32 offset, u32 count, u8 *buffer);

/** TODO */
StatusCode dev_write(char dev, u32 offset, u32 count, u8 *buffer);

/** TODO */
StatusCode dev_block_size(char dev, u32 *size);

/** TODO */
StatusCode dev_block_count(char dev, u32 *count);

//! @endcond

#endif /* __DEV_H__ */
