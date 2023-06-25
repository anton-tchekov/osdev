#include "types.h"
#include "terminal.h"
#include "isr.h"
#include "gdt.h"
#include "timer.h"
#include "keyboard.h"
#include "mouse.h"
#include "stdlib.h"
#include "graphics.h"
#include "searchbar.h"
#include "font_noto.h"

/*static Framebuffer searchBar;

void key_event_main(Key key, i32 codepoint, KeyState state)
{
	static int x = 0;
	char s[2] = {codepoint, '\0'};
	font_string(&searchBar, 10, 10 + x, s, font_noto, 0xFFFFFFFF);
	x+=10;
}*/

void kernel_main(void *info)
{
	Framebuffer searchBar;

	graphics_init(info);
	gdt_init();
	isr_init();
	timer_init(100);
	keyboard_init();
	//keyboard_event_register(key_event_main);
	mouse_init();

	framebuffer_init(&searchBar, (u32 *)0x2000000, 500, 300);
	searchbar_render(&searchBar);
	graphics_blit_framebuffer(&searchBar, (graphics_width() / 2) - 250, (graphics_height() / 2) - 150);

	/* Enable Interrupts */
	__asm__ __volatile__("sti");
	for(;;)
	{
		/* Idle until interrupt */
		__asm__ __volatile__("hlt");
	}
}
