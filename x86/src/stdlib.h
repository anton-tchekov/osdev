#ifndef __STDLIB_H__
#define __STDLIB_H__

#include "types.h"
#include <stdarg.h>

#define panic(msg) _panic((msg), __FILE__, __LINE__)

#define assert(cond) \
	do \
	{ \
		if(!(cond)) { \
			panic("Assertion failed: " #cond); \
		} \
	} while(0)

#define WHITESPACE " \n\t\v\f\r"

bool isalnum(i32 c);
bool isalpha(i32 c);
bool iscntrl(i32 c);
bool isdigit(i32 c);
bool isgraph(i32 c);
bool islower(i32 c);
bool isprint(i32 c);
bool ispunct(i32 c);
bool isspace(i32 c);
bool isupper(i32 c);
bool isxdigit(i32 c);
i32 tolower(i32 c);
i32 toupper(i32 c);
u32 strlen(const char *str);

char *strcpy(char *dest, const char *src);
char *strncpy(char *dest, const char *src, u32 count);
i32 strcmp(const char *str1, const char *str2);
i32 strncmp(const char *str1, const char *str2, u32 count);
const char *strchr(const char *str, i32 c);
void strrev(char *str);

i32 memcmp(const void *ptr1, const void *ptr2, u32 count);
const void *memchr(const void *ptr, i32 value, u32 count);
void *memset(void *ptr, i32 value, u32 count);
void memset8(u8 *ptr, u8 value, u32 count);
void memset16(u16 *ptr, u16 value, u32 count);
void memset32(u32 *ptr, u32 value, u32 count);
void *memcpy(void *dest, const void *src, u32 count);
void *memmove(void *dest, const void *src, u32 len);

void *bsearch(const void *key, const void *base, u32 nitems, u32 size,
	i32 (*compare)(const void *, const void *));

i32 codepoint_utf8(i32 codepoint, char *out);
const char *utf8_codepoint(const char *s, i32 *out);
u32 utf8_length(const char *s);

u32 rand(void);

i32 sprintf(char *str, const char *format, ...);
i32 snprintf(char *str, u32 size, const char *format, ...);
i32 vsnprintf(char *str, u32 size, const char *format, va_list arg);

void _panic(const char *msg, const char *file, u32 line);

#endif /* __STDLIB_H__ */
