#ifndef __SEARCHBAR_H__
#define __SEARCHBAR_H__

#include "framebuffer.h"
#include "types.h"
#include "colortheme.h"
#include "font_noto.h"
#include "str_contains.h"
#include "keyboard.h"

/* Define Struct for a Program (only text by now but add icon later?) */
typedef struct Program
{
	char *title;
} Program;

void searchbar_init(void);
void searchbar_render(Framebuffer *fb);
void searchbar_key_event(Key key, i32 codepoint, KeyState state);

#endif /* __SEARCHBAR_H__ */
