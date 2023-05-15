#ifndef __EDITOR_H__
#define __EDITOR_H__

#include <types.h>
#include <vector.h>

#ifdef EDITORIISSAD

typedef struct
{
	char *Screen;
	Vector Lines;

	i32 CursorX, CursorY;
	i32 PageW, PageH, PageX, PageY;
	i32 SelectionX, SelectionY;
	i32 LineNumberDigits;
	i32 TabSize;
} Editor;

#endif

typedef struct
{
	char *Screen;
	Vector Text;
	i32 Cursor, Selection;
	i32 LineNumberDigits, TabSize;
	i32 PageW, PageH;
} Editor;

void editor_init(Editor *ed);
void editor_render(Editor *ed);

void editor_char(Editor *ed, char c);
void editor_backspace(Editor *ed);
void editor_delete(Editor *ed);
void editor_new_line(Editor *ed);
void editor_home(Editor *ed);
void editor_end(Editor *ed);
void editor_page_up(Editor *ed);
void editor_page_down(Editor *ed);
void editor_left(Editor *ed);
void editor_right(Editor *ed);
void editor_up(Editor *ed);
void editor_down(Editor *ed);

#endif /* __EDITOR_H__ */
