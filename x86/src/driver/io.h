#ifndef __IO_H__
#define __IO_H__

#include "types.h"

static inline void outb(u16 port, u8 val)
{
	asm volatile("outb %0, %1"
				 :
				 : "a"(val), "Nd"(port)
				 : "memory");
}

static inline u8 inb(u16 port)
{
	u8 ret;
	asm volatile("inb %1, %0"
				 : "=a"(ret)
				 : "Nd"(port)
				 : "memory");
	return ret;
}

static inline void io_wait(void)
{
	outb(0x80, 0);
}

#endif /* __IO_H__ */
