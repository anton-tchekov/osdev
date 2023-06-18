#include "gdt.h"
#include "timer.h"

typedef struct
{
	u16 limit_low;
	u16 base_low;
	u8 base_middle;
	u8 access;
	u8 granularity;
	u8 base_high;
} __attribute__((packed)) gdt_entry_t;

typedef struct
{
	u16 limit;
	u32 base;
} __attribute__((packed)) gdt_ptr_t;

gdt_entry_t gdt[3];
gdt_ptr_t gp;

void gdt_flush(void);

void gdt_set_gate(u32 num, u32 base, u32 limit, u8 access, u8 gran)
{
	gdt[num].base_low = (base & 0xFFFF);
	gdt[num].base_middle = (base >> 16) & 0xFF;
	gdt[num].base_high = (base >> 24) & 0xFF;
	gdt[num].limit_low = (limit & 0xFFFF);
	gdt[num].granularity = ((limit >> 16) & 0x0F);
	gdt[num].granularity |= (gran & 0xF0);
	gdt[num].access = access;
}

void gdt_init(void)
{
	gp.limit = 3 * sizeof(gdt_entry_t) - 1;
	gp.base = (u32)&gdt;
	gdt_set_gate(0, 0, 0, 0, 0);
	gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
	gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
	gdt_flush();
}
