#ifndef __SYSCALL_H__
#define __SYSCALL_H__

#include "types.h"

static inline u32 syscall0(u32 id)
{
	register u32 ra0 asm("a0");
	register u32 ra7 asm("a7") = id;

	asm volatile("ecall"
		: "=r"(ra0)
		: "r"(ra7));

	return ra0;
}

static inline u32 syscall1(u32 id, u32 a0)
{
	register u32 ra0 asm("a0") = a0;
	register u32 ra7 asm("a7") = id;

	asm volatile("ecall"
		: "+r"(ra0)
		: "r"(ra7));

	return ra0;
}

static inline u32 syscall2(u32 id, u32 a0, u32 a1)
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

static inline u32 syscall3(u32 id, u32 a0, u32 a1, u32 a2)
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

static inline u32 syscall4(u32 id, u32 a0, u32 a1, u32 a2, u32 a3)
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

static inline u32 syscall5(u32 id, u32 a0, u32 a1, u32 a2, u32 a3, u32 a4)
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

static inline u32 syscall6(u32 id, u32 a0, u32 a1, u32 a2, u32 a3, u32 a4, u32 a5)
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
