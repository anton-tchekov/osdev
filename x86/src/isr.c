#include "isr.h"
#include "idt.h"
#include "terminal.h"
#include "stdlib.h"
#include "io.h"

void isr0(void);
void isr1(void);
void isr2(void);
void isr3(void);
void isr4(void);
void isr5(void);
void isr6(void);
void isr7(void);
void isr8(void);
void isr9(void);
void isr10(void);
void isr11(void);
void isr12(void);
void isr13(void);
void isr14(void);
void isr15(void);
void isr16(void);
void isr17(void);
void isr18(void);
void isr19(void);
void isr20(void);
void isr21(void);
void isr22(void);
void isr23(void);
void isr24(void);
void isr25(void);
void isr26(void);
void isr27(void);
void isr28(void);
void isr29(void);
void isr30(void);
void isr31(void);

void irq0(void);
void irq1(void);
void irq2(void);
void irq3(void);
void irq4(void);
void irq5(void);
void irq6(void);
void irq7(void);
void irq8(void);
void irq9(void);
void irq10(void);
void irq11(void);
void irq12(void);
void irq13(void);
void irq14(void);
void irq15(void);

isr_t interrupt_handlers[256];

void isr_init(void)
{
	idt_set_gate(0, (u32)isr0);
	idt_set_gate(1, (u32)isr1);
	idt_set_gate(2, (u32)isr2);
	idt_set_gate(3, (u32)isr3);
	idt_set_gate(4, (u32)isr4);
	idt_set_gate(5, (u32)isr5);
	idt_set_gate(6, (u32)isr6);
	idt_set_gate(7, (u32)isr7);
	idt_set_gate(8, (u32)isr8);
	idt_set_gate(9, (u32)isr9);
	idt_set_gate(10, (u32)isr10);
	idt_set_gate(11, (u32)isr11);
	idt_set_gate(12, (u32)isr12);
	idt_set_gate(13, (u32)isr13);
	idt_set_gate(14, (u32)isr14);
	idt_set_gate(15, (u32)isr15);
	idt_set_gate(16, (u32)isr16);
	idt_set_gate(17, (u32)isr17);
	idt_set_gate(18, (u32)isr18);
	idt_set_gate(19, (u32)isr19);
	idt_set_gate(20, (u32)isr20);
	idt_set_gate(21, (u32)isr21);
	idt_set_gate(22, (u32)isr22);
	idt_set_gate(23, (u32)isr23);
	idt_set_gate(24, (u32)isr24);
	idt_set_gate(25, (u32)isr25);
	idt_set_gate(26, (u32)isr26);
	idt_set_gate(27, (u32)isr27);
	idt_set_gate(28, (u32)isr28);
	idt_set_gate(29, (u32)isr29);
	idt_set_gate(30, (u32)isr30);
	idt_set_gate(31, (u32)isr31);

	outb(0x20, 0x11);
	outb(0xA0, 0x11);
	outb(0x21, 0x20);
	outb(0xA1, 0x28);
	outb(0x21, 0x04);
	outb(0xA1, 0x02);
	outb(0x21, 0x01);
	outb(0xA1, 0x01);
	outb(0x21, 0x0);
	outb(0xA1, 0x0);

	idt_set_gate(32, (u32)irq0);
	idt_set_gate(33, (u32)irq1);
	idt_set_gate(34, (u32)irq2);
	idt_set_gate(35, (u32)irq3);
	idt_set_gate(36, (u32)irq4);
	idt_set_gate(37, (u32)irq5);
	idt_set_gate(38, (u32)irq6);
	idt_set_gate(39, (u32)irq7);
	idt_set_gate(40, (u32)irq8);
	idt_set_gate(41, (u32)irq9);
	idt_set_gate(42, (u32)irq10);
	idt_set_gate(43, (u32)irq11);
	idt_set_gate(44, (u32)irq12);
	idt_set_gate(45, (u32)irq13);
	idt_set_gate(46, (u32)irq14);
	idt_set_gate(47, (u32)irq15);

	idt_set();
}

static const char *exception_messages[] =
{
	"Division By Zero",
	"Debug",
	"Non Maskable Interrupt",
	"Breakpoint",
	"Into Detected Overflow",
	"Out of Bounds",
	"Invalid Opcode",
	"No Coprocessor",
	"Double Fault",
	"Coprocessor Segment Overrun",
	"Bad TSS",
	"Segment Not Present",
	"Stack Fault",
	"General Protection Fault",
	"Page Fault",
	"Unknown Interrupt",
	"Coprocessor Fault",
	"Alignment Check",
	"Machine Check",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved"
};

void isr_handler(registers_t r)
{
	char s[3];
	terminal_string("Interrupt ");
	itoa(r.int_no, s, 10);
	terminal_string(s);
	terminal_string(" - ");
	terminal_string(exception_messages[r.int_no]);
	terminal_string("\n");
}

void isr_register(u32 n, isr_t handler)
{
	interrupt_handlers[n] = handler;
}

void irq_handler(registers_t r)
{
	if(r.int_no >= 40)
	{
		outb(0xA0, 0x20);
	}

	outb(0x20, 0x20);
	if(interrupt_handlers[r.int_no] != 0)
	{
		interrupt_handlers[r.int_no](r);
	}
}
