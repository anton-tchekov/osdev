#ifndef __INT86_H__
#define __INT86_H__

#include "types.h"

typedef struct
{
	u32 edi, esi, ebp, esp;
	u32 ebx, edx, ecx, eax;
	u16 flags, es, ds, fs, gs;
} __attribute__((packed)) int86regs;

void int86(i32 inum, int86regs *regs);

#endif	/* __INT86_H__ */
