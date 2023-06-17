#include "types.h"
#include "terminal.h"
#include "isr.h"
#include "gdt.h"
#include "timer.h"

void kernel_main(void)
{
	terminal_init();
	gdt_init();
	isr_install();
	timer_init(10);

	/* Enable Interrupts */
	__asm__ __volatile__("sti");

	for(;;)
	{
		/* Idle until interrupt */
		__asm__ __volatile__("hlt");
	}
}
