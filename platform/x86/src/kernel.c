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
#include "statusbar.h"

static Framebuffer searchBar;

void key_event_main(Key key, i32 codepoint, KeyState state)
{
	static int x = 0;
	char s[2] = {codepoint, '\0'};

	graphics_rect(0, 0, 100, 100, 0xFFFF00FF);

	font_string(&searchBar, 10 + x, 200, s, font_noto, 0xFFFFFFFF);
	x += 20;
	graphics_blit_framebuffer(&searchBar, (graphics_width() / 2) - 250, (graphics_height() / 2) - 150);
}

void kernel_main(void *info)
{
	Framebuffer searchBar, statusBar;

	graphics_init(info);
	gdt_init();
	isr_init();
	timer_init(100);
	keyboard_init();
	keyboard_event_register(key_event_main);
	mouse_init();

	/* Init Framebuffers */
	framebuffer_init(&searchBar, (u32 *)0x2000000, 500, 300);
	framebuffer_init(&statusBar, (u32 *)0x3000000, graphics_width(), 40);

	/* render the components in their Framebuffers */
	searchbar_render(&searchBar);
	statusbar_render(&statusBar);
	
	/* Place them on the Screen */
	graphics_blit_framebuffer(&searchBar, (graphics_width() / 2) - 250, (graphics_height() / 2) - 150);
	graphics_blit_framebuffer(&statusBar, 0, graphics_height() - 40);

	/* Enable Interrupts */
	__asm__ __volatile__("sti");
	for(;;)
	{
		/* Idle until interrupt */
		__asm__ __volatile__("hlt");
	}
}
