/**
 * @file    syscall.h
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    23.04.2023
 * @brief   System call interface to communicate with the kernel
 */

#ifndef __SYSCALL_H__
#define __SYSCALL_H__

#include <types.h>

/** List of all Syscalls */
typedef enum
{
	SYSCALL_EXIT,
	SYSCALL_FINISH,

	SYSCALL_EVENT_REGISTER,

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

	SYSCALL_RAND,

	SYSCALL_GFX_RECT,
	SYSCALL_GFX_IMAGE_RGBA,
	SYSCALL_GFX_IMAGE_RGB,
	SYSCALL_GFX_IMAGE_RGB565,
	SYSCALL_GFX_IMAGE_GRAYSCALE,
	SYSCALL_GFX_IMAGE_1BIT,

	SYSCALL_FILE_OPEN,
	SYSCALL_FILE_READ,
	SYSCALL_FILE_WRITE,
	SYSCALL_FILE_CLOSE,
	SYSCALL_FILE_SIZE,

	SYSCALL_KEYBOARD_IS_KEY_PRESSED,

	SYSCALL_SERIAL_WRITE,
	SYSCALL_SERIAL_READ,

	SYSCALL_DATETIME_NOW,
	SYSCALL_MILLIS,
} SyscallID;

/**
 * @brief TODO
 *
 * @param id TODO
 * @return TODO
 */
static inline u32 syscall0(SyscallID id)
{
	register u32 ra0 asm("a0");
	register u32 ra7 asm("a7") = id;

	asm volatile("ecall"
		: "=r"(ra0)
		: "r"(ra7));

	return ra0;
}

/**
 * @brief TODO
 *
 * @param id TODO
 * @param a0 TODO
 * @return TODO
 */
static inline u32 syscall1(SyscallID id, u32 a0)
{
	register u32 ra0 asm("a0") = a0;
	register u32 ra7 asm("a7") = id;

	asm volatile("ecall"
		: "+r"(ra0)
		: "r"(ra7));

	return ra0;
}

/**
 * @brief TODO
 *
 * @param id TODO
 * @param a0 TODO
 * @param a1 TODO
 * @return TODO
 */
static inline u32 syscall2(SyscallID id, u32 a0, u32 a1)
{
	register u32 ra0 asm("a0") = a0;
	register u32 ra1 asm("a1") = a1;
	register u32 ra7 asm("a7") = id;

	asm volatile("ecall"
		: "+r"(ra0)
		: "r"(ra1),
		"r"(ra7));

	return ra0;
}

/**
 * @brief TODO
 *
 * @param id TODO
 * @param a0 TODO
 * @param a1 TODO
 * @param a2 TODO
 * @return TODO
 */
static inline u32 syscall3(SyscallID id, u32 a0, u32 a1, u32 a2)
{
	register u32 ra0 asm("a0") = a0;
	register u32 ra1 asm("a1") = a1;
	register u32 ra2 asm("a2") = a2;
	register u32 ra7 asm("a7") = id;

	asm volatile("ecall"
		: "+r"(ra0)
		: "r"(ra1), "r"(ra2),
		"r"(ra7));

	return ra0;
}

/**
 * @brief TODO
 *
 * @param id TODO
 * @param a0 TODO
 * @param a1 TODO
 * @param a2 TODO
 * @param a3 TODO
 * @return TODO
 */
static inline u32 syscall4(SyscallID id, u32 a0, u32 a1, u32 a2, u32 a3)
{
	register u32 ra0 asm("a0") = a0;
	register u32 ra1 asm("a1") = a1;
	register u32 ra2 asm("a2") = a2;
	register u32 ra3 asm("a3") = a3;
	register u32 ra7 asm("a7") = id;

	asm volatile("ecall"
		: "+r"(ra0)
		: "r"(ra1), "r"(ra2), "r"(ra3),
		"r"(ra7));

	return ra0;
}

/**
 * @brief TODO
 *
 * @param id TODO
 * @param a0 TODO
 * @param a1 TODO
 * @param a2 TODO
 * @param a3 TODO
 * @param a4 TODO
 * @return TODO
 */
static inline u32 syscall5(SyscallID id, u32 a0, u32 a1, u32 a2, u32 a3, u32 a4)
{
	register u32 ra0 asm("a0") = a0;
	register u32 ra1 asm("a1") = a1;
	register u32 ra2 asm("a2") = a2;
	register u32 ra3 asm("a3") = a3;
	register u32 ra4 asm("a4") = a4;
	register u32 ra7 asm("a7") = id;

	asm volatile("ecall"
		: "+r"(ra0)
		: "r"(ra1), "r"(ra2), "r"(ra3), "r"(ra4),
		"r"(ra7));

	return ra0;
}

/**
 * @brief TODO
 *
 * @param id TODO
 * @param a0 TODO
 * @param a1 TODO
 * @param a2 TODO
 * @param a3 TODO
 * @param a4 TODO
 * @param a5 TODO
 * @return TODO
 */
static inline u32 syscall6(
	SyscallID id, u32 a0, u32 a1, u32 a2, u32 a3, u32 a4, u32 a5)
{
	register u32 ra0 asm("a0") = a0;
	register u32 ra1 asm("a1") = a1;
	register u32 ra2 asm("a2") = a2;
	register u32 ra3 asm("a3") = a3;
	register u32 ra4 asm("a4") = a4;
	register u32 ra5 asm("a5") = a5;
	register u32 ra7 asm("a7") = id;

	asm volatile("ecall"
		: "+r"(ra0)
		: "r"(ra1), "r"(ra2), "r"(ra3), "r"(ra4), "r"(ra5),
		"r"(ra7));

	return ra0;
}

#endif /* __SYSCALL_H__ */
