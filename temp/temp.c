/* TEMP FILE CONTAINS CODE THAT IS NO LONGER USED BUT MIGHT GET USEFUL IN THE FUTURE*/

/* --- optimization of string.h by forwarding to kernel --- */

SYSCALL_MEMCPY,
SYSCALL_MEMMOVE,
SYSCALL_MEMCMP,
SYSCALL_MEMCHR,
SYSCALL_MEMSET,

SYSCALL_STRCPY,
SYSCALL_STRNCPY,
SYSCALL_STRLEN,
SYSCALL_STRCMP,
SYSCALL_STRNCMP,
SYSCALL_STRCHR,

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

syscall_memcpy,
syscall_memmove,
syscall_memcmp,
syscall_memchr,
syscall_memset,

syscall_strcpy,
syscall_strncpy,
syscall_strlen,
syscall_strcmp,
syscall_strncmp,
syscall_strchr,

u32 syscall_memcpy(u32 *args);
u32 syscall_memmove(u32 *args);
u32 syscall_memcmp(u32 *args);
u32 syscall_memchr(u32 *args);
u32 syscall_memset(u32 *args);

u32 syscall_strcpy(u32 *args);
u32 syscall_strncpy(u32 *args);
u32 syscall_strlen(u32 *args);
u32 syscall_strcmp(u32 *args);
u32 syscall_strncmp(u32 *args);
u32 syscall_strchr(u32 *args);

u32 syscall_memcpy(u32 *args)
{
	return (u8 *)memcpy(_memory + args[0],
		_memory + args[1], args[2]) - _memory;
}

u32 syscall_memmove(u32 *args)
{
	return (u8 *)memmove(_memory + args[0],
		_memory + args[1], args[2]) - _memory;
}

u32 syscall_memcmp(u32 *args)
{
	return memcmp(_memory + args[0], _memory + args[1], args[2]);
}

u32 syscall_memchr(u32 *args)
{
	return (u8 *)memchr(_memory + args[0], args[1], args[2]) - _memory;
}

u32 syscall_memset(u32 *args)
{
	return (u8 *)memset(_memory + args[0], args[1], args[2]) - _memory;
}

u32 syscall_strcpy(u32 *args)
{
	return strcpy((char *)(_memory + args[0]),
		(char *)(_memory + args[1])) - (char *)_memory;
}

u32 syscall_strncpy(u32 *args)
{
	return strncpy((char *)(_memory + args[0]),
		(char *)(_memory + args[1]), args[2]) - (char *)_memory;
}

u32 syscall_strlen(u32 *args)
{
	return strlen((char *)(_memory + args[0]));
}

u32 syscall_strcmp(u32 *args)
{
	return strcmp((char *)(_memory + args[0]), (char *)(_memory + args[1]));
}

u32 syscall_strncmp(u32 *args)
{
	return strncmp((char *)(_memory + args[0]),
		(char *)(_memory + args[1]), args[2]);
}

u32 syscall_strchr(u32 *args)
{
	return strchr((char *)(_memory + args[0]), args[1]) - (char *)_memory;
}
