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

	framebuffer_init(&test, (u32 *)0x2000000, graphics_width(), graphics_height());
	framebuffer_round_rect_outline(&test, test.Width / 2 - 100, test.Height / 2 - 75, 200, 150, 50, 40, 0x001b1b1b, 0x003b3b3b);
	framebuffer_circle(&test, 50, 50, 50, 0x00ff0000);
	framebuffer_circle_outline(&test, 150, 50, 50, 40, 0x0000ff00, 0x00ff0000);

	font_string(&test, 100, 100, "Hello World", font_noto, 0xFFFFFFFF);

	graphics_blit_framebuffer(&test, 0, 0);

	/* Enable Interrupts */
	__asm__ __volatile__("sti");
	for(;;)
	{
		/* Idle until interrupt */
		__asm__ __volatile__("hlt");
	}
}
