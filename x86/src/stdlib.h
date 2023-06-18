#ifndef __STDLIB_H__
#define __STDLIB_H__

#include "types.h"

u32 strlen(const char *str);

void memset16(void *ptr, u16 value, u32 count);

void *memmove(void *dest, const void *src, u32 len);

void strrev(char *str);

int itoa(int num, char *str, int base);

#endif /* __STDLIB_H__ */
