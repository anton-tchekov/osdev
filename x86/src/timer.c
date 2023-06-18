#include "timer.h"
#include "isr.h"
#include "io.h"
#include "terminal.h"
#include "stdlib.h"

static u32 _ticks;

static void timer_callback(registers_t regs)
{
	++_ticks;
	(void)regs;
}

void timer_init(u32 freq)
{
	u32 divisor = 1193180 / freq;
	isr_register(IRQ0, timer_callback);
	outb(0x43, 0x36);
	outb(0x40, (u8)(divisor & 0xFF));
	outb(0x40, (u8)((divisor >> 8) & 0xFF));
}
