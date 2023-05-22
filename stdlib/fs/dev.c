#include "dev.h"

#define MAX_DEVICES 26

static BlockDevice *_devices[MAX_DEVICES];

char dev_attach(BlockDevice *dev)
{
	u8 i;
	for(i = 0; i < MAX_DEVICES; ++i)
	{
		if(!_devices[i])
		{
			_devices[i] = dev;
			return i + 'A';
		}
	}

	return 0;
}

void dev_detach(char letter)
{
	_devices[letter - 'A'] = NULL;
}

StatusCode dev_read(char dev, u32 offset, u32 count, u8 *buffer)
{
	u8 i = dev - 'A';
	BlockDevice *dev = _devices[i];
	if(!dev)
	{
		return ERROR_NO_DEVICE;
	}

	return dev->Read(offset, count, buffer);
}

StatusCode dev_write(char dev, u32 offset, u32 count, u8 *buffer)
{
	u8 i = dev - 'A';
	BlockDevice *dev = _devices[i];
	if(!dev)
	{
		return ERROR_NO_DEVICE;
	}

	return dev->Write(offset, count, buffer);
}

StatusCode dev_block_size(char dev, u32 *size)
{
	u8 i = dev - 'A';
	BlockDevice *dev = _devices[i];
	if(!dev)
	{
		return ERROR_NO_DEVICE;
	}

	*size = dev->BlockSize;
	return STATUS_SUCCESS;
}

StatusCode dev_block_count(char dev, u32 *count)
{
	u8 i = dev - 'A';
	BlockDevice *dev = _devices[i];
	if(!dev)
	{
		return ERROR_NO_DEVICE;
	}

	*count = dev->BlockCount;
	return STATUS_SUCCESS;
}
