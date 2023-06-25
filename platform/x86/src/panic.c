#include "panic.h"
#include "graphics.h"
#include "framebuffer.h"
#include "font_noto.h"
#include "sprintf.h"

void _panic(const char *msg, const char *file, u32 line)
{
	char location_str[64];
	Framebuffer fe;
	framebuffer_init(&fe, (u32 *)0x4000000, graphics_width(), graphics_height());
	framebuffer_rect(&fe, 0, 0, graphics_width(), graphics_height(), 0x00000000);

	sprintf(location_str, "You fucked up in %s:%d", file, line);

	font_string(&fe, 20,  20, "<<< KERNEL PANIC >>>", font_noto, 0xFF0080FF);
	font_string(&fe, 20,  60, msg, font_noto, 0xFF0080FF);
	font_string(&fe, 20, 100, location_str, font_noto, 0xFF0080FF);

	graphics_blit_framebuffer(&fe, 0, 0);

	__asm__ __volatile__("cli");
	__asm__ __volatile__("hlt");
}
