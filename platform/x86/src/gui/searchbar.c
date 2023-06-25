#include "searchbar.h"

#define SEARCH_HEIGHT 40

char* searchTerm = "";

ColorTheme nordPalette = {
	.primary = 0xFF40342E,
	.secondary = 0xFF52423B,
	.background = 0xFF5E4C43,
	.text = 0xFFF4EFEC};

#define PROGRAM_COUNT 4
/* Create Array of Programms */
Program programs[PROGRAM_COUNT] = {
	{"Firefox"}, {"VScode"}, {"Files"}, {"Tetris"}
};

void searchbar_render(Framebuffer *fb)
{
	/* Draw background */
	framebuffer_round_rect_outline(fb, 0, 0, fb->Width, fb->Height, 10, 5, nordPalette.primary, nordPalette.background);

	/* Divider - underline of searchbar */
	framebuffer_rect(fb, 0, SEARCH_HEIGHT, fb->Width, 5, nordPalette.primary);

	/* Search title */
	if(searchTerm == "") {
		font_string(fb, 15, 10, "Type to search...", font_noto, nordPalette.text);
	} else {
		font_string(fb, 15, 10, searchTerm, font_noto, nordPalette.text);
	}

	/* Create Results array and count value */
	Program results[PROGRAM_COUNT] = {};
	i32 resultsCount = 0;

	for (i32 i = 0; i < PROGRAM_COUNT; i++)
	{
		/* check for every element if it contains the search term */
		if(str_contains(programs[i].title, searchTerm)) {
			/* Add it to the results array and increment the Counter */
			results[resultsCount] = programs[i];
			resultsCount++;
		}
	}
	/* Render all the results to list */
	for (i32 i = 0; i < resultsCount; i++)
	{
		render_searchresult(fb, results[i], 15, SEARCH_HEIGHT * (i+1));
	}
	
}

static void render_searchresult(Framebuffer *fb, Program res, i32 x, i32 y) {

	/* "Icon" */
	framebuffer_circle(fb, x + 10, y + (SEARCH_HEIGHT / 2) + 2, 5, nordPalette.text);
	/* Name */
	font_string(fb, x + 30, y + (SEARCH_HEIGHT - font_noto->Size) / 2, res.title, font_noto, nordPalette.text);
	/* Divider to next */
	framebuffer_rect(fb, SEARCH_HEIGHT, y + SEARCH_HEIGHT, fb->Width - 2*SEARCH_HEIGHT, 1, nordPalette.primary);
}