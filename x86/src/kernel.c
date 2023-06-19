#include "types.h"
#include "terminal.h"
#include "isr.h"
#include "gdt.h"
#include "timer.h"
#include "keyboard.h"
#include "mouse.h"
#include "stdlib.h"
#include "graphics.h"

void kernel_main(void *info)
{
	graphics_init(info);
	/* terminal_init(); */
	gdt_init();
	isr_init();
	timer_init(100);
	keyboard_init();
	mouse_init();

	graphics_rect(10, 10, 100, 100, graphics_color(255, 0, 0));
	graphics_rect(30, 30, 100, 100, graphics_color(0, 255, 0));
	graphics_rect(50, 50, 100, 100, graphics_color(0, 0, 255));

	/* Enable Interrupts */
	__asm__ __volatile__("sti");

	for(;;)
	{
		/* Idle until interrupt */
		__asm__ __volatile__("hlt");
	}
}
