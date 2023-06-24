#include "types.h"
#include "terminal.h"
#include "isr.h"
#include "gdt.h"
#include "timer.h"
#include "keyboard.h"
#include "mouse.h"
#include "stdlib.h"
#include "graphics.h"

char hexdigit(u32 h)
{
	h &= 0x0F;
	return h < 10 ? h + '0' : h + 'A' - 10;
}

void memory_dump(u8 *buffer, u32 length)
{
	u32 x = 10, y = 10;
	u32 i;
	u8 *end = buffer + length;

	for(; buffer < end; buffer += 16)
	{
		for(i = 0; i < 16; ++i)
		{
			u8 b = buffer[i];

			x += graphics_char(x, y, hexdigit(b >> 4));
			x += graphics_char(x, y, hexdigit(b));
		}

		x += 50;
		for(i = 0; i < 16; ++i)
		{
			char c = buffer[i];
			x += graphics_char(x, y, (c < 32 || c > 126) ? '.' : c);
		}

		y += 12;
		x = 10;
	}
}

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
	
	graphics_blit_framebuffer(&test, 0, 0);

	graphics_string(600, 10, "Hello World!");

	/* Enable Interrupts */
	__asm__ __volatile__("sti");
	for(;;)
	{
		/* Idle until interrupt */
		__asm__ __volatile__("hlt");
	}
}
