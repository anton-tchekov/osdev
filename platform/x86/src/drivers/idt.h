#ifndef __IDT_H__
#define __IDT_H__

#include "types.h"

typedef struct
{
	u16 low_offset;
	u16 sel;
	u8 always0;
	u8 flags;
	u16 high_offset;
} __attribute__((packed)) idt_gate_t;

typedef struct
{
	u16 limit;
	u32 base;
} __attribute__((packed)) idt_register_t;

void idt_set_gate(u32 n, u32 handler);
void idt_set(void);

#endif /* __IDT_H__ */
