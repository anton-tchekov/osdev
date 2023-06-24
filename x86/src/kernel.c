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

#include "searchbar.h"

void kernel_main(void *info)
{
	Framebuffer searchBar;

	graphics_init(info);
	gdt_init();
	isr_init();
	timer_init(100);
	keyboard_init();
	mouse_init();

	framebuffer_init(&searchBar, (u32 *)0x2000000, 500, 300);

	searchbar_render(&searchBar);
	
	// font_string(&searchBar, 100, 100, "Hello World", font_noto, 0xFFFFFFFF);

	graphics_blit_framebuffer(&searchBar, (graphics_width() / 2) - 250, (graphics_height() / 2) - 150);

	/* Enable Interrupts */
	__asm__ __volatile__("sti");
	for(;;)
	{
		/* Idle until interrupt */
		__asm__ __volatile__("hlt");
	}
}
