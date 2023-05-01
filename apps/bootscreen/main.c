#include <std.h>
#include "assets/bootscreen.c"

#define BOOTSCREEN_DELAY 2000

static u32 _begin;

void setup(void)
{
	_begin = millis();

	gfx_rect(0, 0, GFX_WIDTH, GFX_HEIGHT, COLOR_BLACK);
	gfx_image_rgb(
		GFX_WIDTH / 2 - bootscreen.width / 2,
		GFX_HEIGHT / 2 - bootscreen.height / 2,
		bootscreen.width,
		bootscreen.height,
		bootscreen.pixel_data);
}

void loop(void)
{
	if(millis() - _begin >= BOOTSCREEN_DELAY)
	{
		exit(0);
	}
}
