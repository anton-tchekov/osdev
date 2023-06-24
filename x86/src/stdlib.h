#ifndef __STDLIB_H__
#define __STDLIB_H__

#include "types.h"
#include <stdarg.h>

u32 strlen(const char *str);

void assert(bool cond);

void memset8(u8 *ptr, u8 value, u32 count);
void memset16(u16 *ptr, u16 value, u32 count);
void memset32(u32 *ptr, u32 value, u32 count);

void *memcpy(void *dest, const void *src, u32 count);

void *memmove(void *dest, const void *src, u32 len);

void strrev(char *str);

int itoa(int num, char *str, int base);

void *bsearch(const void *key, const void *base, u32 nitems, u32 size,
	i32 (*compare)(const void *, const void *));

i32 codepoint_utf8(i32 codepoint, char *out);
const char *utf8_codepoint(const char *s, i32 *out);
u32 utf8_length(const char *s);

u32 rand(void);

i32 sprintf(char *str, const char *format, ...);
i32 snprintf(char *str, u32 size, const char *format, ...);
i32 vsnprintf(char *str, u32 size, const char *format, va_list arg);

#endif /* __STDLIB_H__ */
