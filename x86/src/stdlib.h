#ifndef __STDLIB_H__
#define __STDLIB_H__

#include "types.h"

u32 strlen(const char *str);

void memset8(u8 *ptr, u8 value, u32 count);
void memset16(u16 *ptr, u16 value, u32 count);
void memset32(u32 *ptr, u32 value, u32 count);

void *memcpy(void *dest, const void *src, u32 count);

void *memmove(void *dest, const void *src, u32 len);

void strrev(char *str);

int itoa(int num, char *str, int base);

#endif /* __STDLIB_H__ */
