#include <std.h>
#include <syscall.h>

/* --- STR --- */
char *strcpy(char *dest, const char *src)
{
	return (char *)syscall2(SYSCALL_STRCPY, (u32)dest, (u32)src);
}

char *strncpy(char *dest, const char *src, u32 num)
{
	return (char *)syscall3(SYSCALL_STRNCPY, (u32)dest, (u32)src, num);
}

u32 strlen(const char *str)
{
	return syscall1(SYSCALL_STRLEN, (u32)str);
}

int strcmp(const char *str1, const char *str2)
{
	return syscall2(SYSCALL_STRCMP, (u32)str1, (u32)str2);
}

int strncmp(const char *str1, const char *str2, u32 num)
{
	return syscall3(SYSCALL_STRNCMP, (u32)str1, (u32)str2, num);
}

/* --- MEM --- */
void *memcpy(void *dest, const void *src, u32 count)
{
	return (void *)syscall3(SYSCALL_MEMCPY, (u32)dest, (u32)src, count);
}

void *memmove(void *dest, const void *src, u32 num)
{
	return (void *)syscall3(SYSCALL_MEMMOVE, (u32)dest, (u32)src, num);
}

int memcmp(const void *ptr1, const void *ptr2, u32 num)
{
	return syscall3(SYSCALL_MEMCMP, (u32)ptr1, (u32)ptr2, num);
}

const void *memchr(const void *ptr, int value, u32 num)
{
	return (void *)syscall3(SYSCALL_MEMCHR, (u32)ptr, (u32)value, num);
}

void *memset(void *ptr, int value, u32 num)
{
	return (void *)syscall3(SYSCALL_MEMSET, (u32)ptr, (u32)value, num);
}

/* --- IO --- */
void print(const char *s)
{
	syscall1(SYSCALL_PRINT, (u32)s);
}

void exit(int status)
{
	syscall1(SYSCALL_EXIT, status);
}

int main(void);

void start(void) __attribute__((section(".start")));
void start(void)
{
	exit(main());
}
