#ifndef __EDITOR_H__
#define __EDITOR_H__

#include <types.h>
#include <vector.h>

typedef struct EDITOR
{
	char *Screen, *Copy;
	Vector Lines;

	i32 CursorX, CursorY;
	i32 PageW, PageH, PageX, PageY;
	i32 SelectionX, SelectionY;
	i32 LineNumberDigits;
	i32 TabSize;
} Editor;

void editor_init(Editor *ed);
void editor_render(Editor *ed);
void editor_action(Editor *ed, i32 action);
void editor_char(Editor *ed, char c);

#endif /* __EDITOR_H__ */
