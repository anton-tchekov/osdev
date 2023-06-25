#ifndef __MEMSET_H__
#define __MEMSET_H__

#include "types.h"

void memset8(u8 *ptr, u8 value, u32 count);
void memset16(u16 *ptr, u16 value, u32 count);
void memset32(u32 *ptr, u32 value, u32 count);

#endif /* __MEMSET_H__ */