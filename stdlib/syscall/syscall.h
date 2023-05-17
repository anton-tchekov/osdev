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

	SYSCALL_KEYBOARD_IS_KEY_PRESSED,

	SYSCALL_SERIAL_WRITE,

	SYSCALL_DATETIME_NOW,
	SYSCALL_MILLIS,
} SyscallID;

/**
 * @brief System call with zero parameters
 *
 * @param id System call ID. See SyscallID enum above
 * @return Return value
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
 * @brief System call with one parameter
 *
 * @param id System call ID. See SyscallID enum above
 * @param a0 First parameter
 * @return Return value
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
 * @brief System call with two parameters
 *
 * @param id System call ID. See SyscallID enum above
 * @param a0 First parameter
 * @param a1 Second parameter
 * @return Return value
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
 * @brief System call with three parameters
 *
 * @param id System call ID. See SyscallID enum above
 * @param a0 First parameter
 * @param a1 Second parameter
 * @param a2 Third parameter
 * @return Return value
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
 * @brief System call with four parameters
 *
 * @param id System call ID. See SyscallID enum above
 * @param a0 First parameter
 * @param a1 Second parameter
 * @param a2 Third parameter
 * @param a3 Fourth parameter
 * @return Return value
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
 * @brief System call with five parameters
 *
 * @param id System call ID. See SyscallID enum above
 * @param a0 First parameter
 * @param a1 Second parameter
 * @param a2 Third parameter
 * @param a3 Fourth parameter
 * @param a4 Fifth parameter
 * @return Return value
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
 * @brief System call with six parameters
 *
 * @param id System call ID. See SyscallID enum above
 * @param a0 First parameter
 * @param a1 Second parameter
 * @param a2 Third parameter
 * @param a3 Fourth parameter
 * @param a4 Fifth parameter
 * @param a5 Sixth parameter
 * @return Return value
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

/**
 * @brief End program
 *
 * @param status Exit status
 */
static inline void exit(i32 status)
{
	syscall1(SYSCALL_EXIT, status);
}

#endif /* __SYSCALL_H__ */
