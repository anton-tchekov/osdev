#ifndef __STD_H__
#define __STD_H__

#include "types.h"

int sprintf(char *str, const char *format, ...);
int snprintf(char *str, u32 n, const char *format, ...);

char *strcpy(char *dest, const char *src);
char *strncpy(char *dest, const char *src, u32 num);
u32 strlen(const char *str);
int strcmp(const char *str1, const char *str2);
int strncmp(const char *str1, const char *str2, u32 num);

void *memcpy(void *dest, const void *src, u32 count);
void *memmove(void *dest, const void *src, u32 num);
int memcmp(const void *ptr1, const void *ptr2, u32 num);
const void *memchr(const void *ptr, int value, u32 num);
void *memset(void *ptr, int value, u32 num);

void print(const char *str);

void exit(int status);

#endif /* __STD_H__ */
