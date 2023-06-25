#ifndef __SEARCHBAR_H__
#define __SEARCHBAR_H__

#include "framebuffer.h"
#include "types.h"
#include "colortheme.h"
#include "font_noto.h"
#include "str_contains.h"

/* Define Struct for a Program (only text by now but add icon later?) */
typedef struct Program
{
	char *title;
} Program;

void searchbar_render(Framebuffer *fb);

static void render_searchresult(Framebuffer *fb, Program res, i32 x, i32 y);

#endif /* __SEARCHBAR_H__ */
