/**
 * @file    string.c
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    23.04.2023
 */

#include <string.h>
#include <syscall.h>

char *strcpy(char *dest, const char *src)
{
	return (char *)syscall2(SYSCALL_STRCPY, (u32)dest, (u32)src);
}

char *strncpy(char *dest, const char *src, u32 count)
{
	return (char *)syscall3(SYSCALL_STRNCPY, (u32)dest, (u32)src, count);
}

u32 strlen(const char *str)
{
	return syscall1(SYSCALL_STRLEN, (u32)str);
}

i32 strcmp(const char *str1, const char *str2)
{
	return syscall2(SYSCALL_STRCMP, (u32)str1, (u32)str2);
}

i32 strncmp(const char *str1, const char *str2, u32 count)
{
	return syscall3(SYSCALL_STRNCMP, (u32)str1, (u32)str2, count);
}

const char *strchr(const char *str, i32 c)
{
	return (const char *)syscall2(SYSCALL_STRCHR, (u32)str, c);
}

void *memcpy(void *dest, const void *src, u32 count)
{
	return (void *)syscall3(SYSCALL_MEMCPY, (u32)dest, (u32)src, count);
}

void *memmove(void *dest, const void *src, u32 count)
{
	return (void *)syscall3(SYSCALL_MEMMOVE, (u32)dest, (u32)src, count);
}

i32 memcmp(const void *ptr1, const void *ptr2, u32 count)
{
	return syscall3(SYSCALL_MEMCMP, (u32)ptr1, (u32)ptr2, count);
}

const void *memchr(const void *ptr, i32 value, u32 count)
{
	return (void *)syscall3(SYSCALL_MEMCHR, (u32)ptr, (u32)value, count);
}

void *memset(void *ptr, i32 value, u32 count)
{
	return (void *)syscall3(SYSCALL_MEMSET, (u32)ptr, (u32)value, count);
}
