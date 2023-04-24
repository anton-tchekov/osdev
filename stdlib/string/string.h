/**
 * @file    string.h
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    23.04.2023
 * @brief   TODO
 */

#ifndef __STRING_H__
#define __STRING_H__

#include <types.h>

char *strcpy(char *dest, const char *src);
char *strncpy(char *dest, const char *src, u32 num);
u32 strlen(const char *str);
i32 strcmp(const char *str1, const char *str2);
i32 strncmp(const char *str1, const char *str2, u32 num);
const char *strchr(const char *str, i32 c);

void *memcpy(void *dest, const void *src, u32 count);
void *memmove(void *dest, const void *src, u32 num);
i32 memcmp(const void *ptr1, const void *ptr2, u32 num);
const void *memchr(const void *ptr, i32 value, u32 num);
void *memset(void *ptr, i32 value, u32 num);

#endif /* __STRING_H__ */
