#include "timer.h"
#include "isr.h"
#include "io.h"
#include "terminal.h"
#include "stdlib.h"

static u32 _ticks;

static void timer_callback(registers_t regs)
{
	char tick_ascii[256];

	++_ticks;

	terminal_string("Tick: ");
	itoa(_ticks, tick_ascii, 10);
	terminal_string(tick_ascii);
	terminal_string("\n");

	(void)regs;
}

void timer_init(u32 freq)
{
	u32 divisor = 1193180 / freq;
	register_interrupt_handler(IRQ0, timer_callback);
	outb(0x43, 0x36);
	outb(0x40, (u8)(divisor & 0xFF));
	outb(0x40, (u8)((divisor >> 8) & 0xFF));
}
