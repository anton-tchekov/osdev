#include "types.h"
#include "terminal.h"
#include "isr.h"
#include "gdt.h"
#include "timer.h"
#include "keyboard.h"
#include "mouse.h"
#include "stdlib.h"
#include "graphics.h"
#include "noto.h"

void kernel_main(void *info)
{
	Framebuffer test;

	graphics_init(info);
	gdt_init();
	isr_init();
	timer_init(100);
	keyboard_init();
	mouse_init();

	framebuffer_init(&test, (u32 *)0x2000000, 200, 100);
	framebuffer_circle(&test, 50, 50, 40, 0x0000FF);
	font_string(&test, 10, 10, "Hello World", font_noto, 0xFFFFFFFF);

	graphics_blit_framebuffer(&test, 200, 200);

	/* Enable Interrupts */
	__asm__ __volatile__("sti");
	for(;;)
	{
		/* Idle until interrupt */
		__asm__ __volatile__("hlt");
	}
}
