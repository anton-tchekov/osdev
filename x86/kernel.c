#include "types.h"
#include "terminal.h"
#include "isr.h"
#include "gdt.h"
#include "timer.h"

void kernel_main(void)
{
	terminal_init();
	gdt_install();
	isr_install();
	timer_init(10);
	asm volatile("sti");

	/* for (i = 0; i < 30; ++i)
	{
		terminal_string("Hello, kernel World! i = ");
		terminal_char((i / 10) + '0');
		terminal_char((i % 10) + '0');
		terminal_char('\n');
	}

	terminal_string("Why are you gae?");
	terminal_string("ABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZ\n");
	terminal_string("HALLO WELT\n\n\n");

	terminal_string("/home/files/ $");
	*/

	__asm__ __volatile__("int $2");
	while (true)
	{
	}
}
