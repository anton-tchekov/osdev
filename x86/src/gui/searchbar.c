#include "searchbar.h"

#define SEARCH_HEIGHT 40

ColorTheme nordPalette = {
    .primary = 0xFF40342E,
    .secondary = 0xFF52423B,
    .background = 0xFF5E4C43,
    .text = 0xFFF4EFEC
};

void searchbar_render(Framebuffer *fb) {
	/* Draw background */
	framebuffer_round_rect_outline(fb, 0, 0, fb->Width, fb->Height, 10, 5, nordPalette.primary, nordPalette.background);

	/* Divider - underline of searchbar */
	framebuffer_rect(fb, 0, SEARCH_HEIGHT, fb->Width, 5, nordPalette.primary);

	/* Search title */
	font_string(fb, 15, 10, "Type to search...", font_noto, nordPalette.text);
}