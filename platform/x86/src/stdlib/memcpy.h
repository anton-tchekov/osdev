#ifndef __MEMCPY_H__
#define __MEMCPY_H__

#include "types.h"

void *memcpy(void *dest, const void *src, u32 count);
void *memmove(void *dest, const void *src, u32 len);

#endif /* __MEMCPY_H__ */
