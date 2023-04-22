#ifndef __STD_H__
#define __STD_H__

#include "types.h"

void main(void) __attribute__((section(".start")));

char *strcpy(char *dest, const char *src);
char *strncpy(char *dest, const char *src, size_t num);

size_t strlen(const char *str);

int strcmp(const char *str1, const char *str2);
int strncmp(const char *str1, const char *str2, size_t num);

void *memcpy(void *dest, const void *src, size_t count);
void *memmove(void *dest, const void *src, size_t num);
int memcmp(const void *ptr1, const void *ptr2, size_t num);
const void *memchr(const void *ptr, int value, size_t num);
void *memset(void *ptr, int value, size_t num);

void exit(int status);

#endif /* __STD_H__ */
