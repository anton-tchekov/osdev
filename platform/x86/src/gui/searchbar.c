#include "searchbar.h"
#include "graphics.h"
#include "ctype.h"

#define SEARCH_HEIGHT   40
#define MAX_RESULTS      5

static u32 cursor;

static bool is_open;

static char searchTerm[256];

static Framebuffer searchBar;

static Program programs[] =
{
	{ "Firefox" },
	{ "VS Code" },
	{ "Files" },
	{ "Tetris" },
	{ "Shutdown" },
	{ "Restart" },
	{ "Settings" },
	{ "Snake" },
	{ "Terminal" },
};

void searchbar_key_event(Key key, i32 codepoint, KeyState state)
{
	if(state != KEYSTATE_PRESSED)
	{
		return;
	}

	if(key == (KEY_S | MOD_CTRL)) {
		is_open = true;
	}

	if(is_open) {
		if(key == KEY_BACKSPACE)
		{
			if(cursor > 0)
			{
				searchTerm[--cursor] = '\0';
			}
		}

		if(isprint(codepoint))
		{
			searchTerm[cursor++] = codepoint;
			searchTerm[cursor] = '\0';
		}

		searchbar_render(&searchBar);
		graphics_blit_framebuffer(&searchBar, (graphics_width() / 2) - 250,
			(graphics_height() / 2) - 150);
	}

}

static void render_searchresult(Framebuffer *fb, Program *res, i32 x, i32 y)
{
	/* Icon */
	framebuffer_circle(fb, x + 10, y + (SEARCH_HEIGHT / 2) + 2, 5,
		nordPalette.text);

	/* Name */
	font_string(fb, x + 30, y + (SEARCH_HEIGHT - font_noto->Size) / 2,
		res->title, font_noto, nordPalette.text);

	/* Divider to next */
	framebuffer_rect(fb, SEARCH_HEIGHT, y + SEARCH_HEIGHT,
		fb->Width - 2 * SEARCH_HEIGHT, 1, nordPalette.primary);
}

void searchbar_render(Framebuffer *fb)
{
	Program *results[MAX_RESULTS];
	i32 i, count;

	/* Draw background */
	framebuffer_round_rect_outline(fb, 0, 0, fb->Width, fb->Height, 10, 5,
		nordPalette.primary, nordPalette.background);

	/* Divider - underline of searchbar */
	framebuffer_rect(fb, 0, SEARCH_HEIGHT, fb->Width, 5, nordPalette.primary);

	/* Search title */
	if(searchTerm[0])
	{
		font_string(fb, 15, 10, searchTerm, font_noto, nordPalette.text);
	}
	else
	{
		font_string(fb, 15, 10, "Type to search ...", font_noto,
			nordPalette.text);
	}

	/* Search for Programms */
	for(i = 0, count = 0; i < (i32)ARRLEN(programs) && count < MAX_RESULTS; i++)
	{
		if(str_contains(programs[i].title, searchTerm))
		{
			results[count++] = &programs[i];
		}
	}

	/* render searchresults in list */
	for(i = 0; i < count; i++)
	{
		render_searchresult(fb, results[i], 15, SEARCH_HEIGHT * (i + 1));
	}
}

void searchbar_init(void)
{
	framebuffer_init(&searchBar, (u32 *)0x2000000, 500, 300);
}
