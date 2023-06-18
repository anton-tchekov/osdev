#include "idt.h"

#define IDT_ENTRIES 256

idt_gate_t idt[IDT_ENTRIES];
idt_register_t idt_reg;

void idt_set_gate(u32 n, u32 handler)
{
	idt[n].low_offset = (u16)((handler) & 0xFFFF);
	idt[n].sel = 0x08;
	idt[n].always0 = 0;
	idt[n].flags = 0x8E;
	idt[n].high_offset = (u16)(((handler) >> 16) & 0xFFFF);
}

void idt_set(void)
{
	idt_reg.base = (u32)&idt;
	idt_reg.limit = IDT_ENTRIES * sizeof(idt_gate_t) - 1;
	__asm__ __volatile__("lidtl (%0)" : : "r"(&idt_reg));
}
