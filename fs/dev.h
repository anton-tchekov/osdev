#ifndef __DEV_H__
#define __DEV_H__

#include "types.h"
#include "status.h"

typedef struct
{
	i32 BlockSize, BlockCount;
	StatusCode (*Read)(u32 offset, u32 count, u8 *buffer);
	StatusCode (*Write)(u32 offset, u32 count, u8 *buffer);
} BlockDevice;

char dev_attach(BlockDevice *dev);
void dev_detach(char letter);
StatusCode dev_read(char dev, u32 offset, u32 count, u8 *buffer);
StatusCode dev_write(char dev, u32 offset, u32 count, u8 *buffer);
StatusCode dev_block_size(char dev, u32 *size);
StatusCode dev_block_count(char dev, u32 *count);

#endif /* __DEV_H__ */
