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
#include "ctype.h"

void key_event(Key key, i32 codepoint, KeyState state)
{
	searchbar_key_event(key, codepoint, state);
}

void kernel_main(void *info)
{
	Framebuffer statusBar;

	graphics_init(info);
	gdt_init();
	isr_init();
	timer_init(100);
	keyboard_init();
	mouse_init();

	searchbar_init();

	/* Init Framebuffers */
	framebuffer_init(&statusBar, (u32 *)0x3000000, graphics_width(), 40);

	/* render the components in their Framebuffers */
	statusbar_render(&statusBar);

	/* Place them on the Screen */
	graphics_blit_framebuffer(&statusBar, 0, graphics_height() - 40);

	/* Enable Interrupts */
	__asm__ __volatile__("sti");
	for(;;)
	{
		/* Idle until interrupt */
		__asm__ __volatile__("hlt");
	}
}
