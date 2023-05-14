#include "editor.h"
#include <std.h>

/* --- PUBLIC --- */
void editor_init(Editor *ed)
{
	ed->TabSize = 4;
	ed->LineNumberDigits = 4;

	vector_init(&ed->Lines, sizeof(Vector), 128);

	//vector_push(&ed->Lines, ector_get(&ed->Lines, 0), sizeof(char), 8);

	ed->CursorX = 0;
	ed->CursorY = 0;

	ed->PageW = 30;
	ed->PageH = 50;
	ed->PageY = 0;
	ed->PageX = 0;

	ed->Screen = memalloc((ed->PageW + 1 + ed->LineNumberDigits) * ed->PageH);
}

/* --- EDITING --- */
void editor_char(Editor *ed, char c)
{
	vector_insert(vector_get(&ed->Lines, ed->CursorY), ed->CursorX, &c);
}

void editor_backspace(Editor *ed)
{
	Vector *line = vector_get(&ed->Lines, ed->CursorY);
	if(ed->CursorX == 0)
	{
		if(ed->CursorY > 0)
		{
			/* merge with previous line */
			Vector *prev = vector_get(&ed->Lines, --ed->CursorY);
			ed->CursorX = vector_len(prev);
			vector_push_range(prev, vector_len(line), line->Data);
			vector_destroy(line);
			vector_remove(&ed->Lines, ed->CursorY + 1);
		}
	}
	else
	{
		/* delete prev char */
		vector_remove(line, --ed->CursorX);
	}
}

void editor_delete(Editor *ed)
{
	Vector *line = vector_get(&ed->Lines, ed->CursorY);
	i32 line_len = (i32)vector_len(line);
	if(ed->CursorX >= line_len)
	{
		i32 num_lines = (i32)vector_len(&ed->Lines);
		if(ed->CursorY < num_lines)
		{
			i32 next_idx = ed->CursorY + 1;
			Vector *next = vector_get(&ed->Lines, next_idx);

			/* merge with next line */
			vector_push_range(line, vector_len(next), next->Data);
			vector_destroy(next);
			vector_remove(&ed->Lines, next_idx);
		}
	}
	else
	{
		/* delete next char */
		vector_remove(line, ed->CursorX);
	}
}

void editor_new_line(Editor *ed)
{
	Vector *cur = vector_get(&ed->Lines, ed->CursorY);
	char *str = (char *)cur->Data + ed->CursorX;
	i32 len = vector_len(cur) - ed->CursorX;
	Vector new;

	/* Copy characters after cursor on current line to new line */
	vector_init(&new, sizeof(char), len);
	vector_push_range(&new, len, str);

	/* Insert new line */
	vector_insert(&ed->Lines, ed->CursorY + 1, &new);

	/* Remove characters after cursor on current line */
	vector_remove_range(cur, ed->CursorX, len);

	++ed->CursorY;
	ed->CursorX = 0;
}

/* --- CURSOR MOVEMENT --- */
void editor_home(Editor *ed)
{
	i32 i = 0;
	if(ed->CursorX == 0)
	{
		Vector *line = vector_get(&ed->Lines, ed->CursorY);
		char *buf = line->Data;
		while(isspace(buf[i]))
		{
			++i;
		}
	}

	ed->CursorX = i;
}

void editor_end(Editor *ed)
{
	ed->CursorX = vector_len(vector_get(&ed->Lines, ed->CursorY));
}

void editor_page_up(Editor *ed)
{
	ed->CursorY -= ed->PageH;
	if(ed->CursorY < 0)
	{
		ed->CursorY = 0;
		ed->CursorX = 0;
	}
}

void editor_page_down(Editor *ed)
{
	i32 num_lines = (i32)vector_len(&ed->Lines);
	ed->CursorY += ed->PageY;
	if(ed->CursorY >= num_lines)
	{
		ed->CursorY = num_lines - 1;
		ed->CursorX = vector_len(vector_get(&ed->Lines, ed->CursorY));
	}
}

void editor_left(Editor *ed)
{
	if(ed->CursorX == 0)
	{
		ed->CursorX = vector_len(vector_get(&ed->Lines, --ed->CursorY));
	}
	else
	{
		--ed->CursorX;
	}
}

void editor_right(Editor *ed)
{
	if(ed->CursorX == (i32)vector_len(vector_get(&ed->Lines, ed->CursorY)))
	{
		++ed->CursorY;
		ed->CursorX = 0;
	}
	else
	{
		++ed->CursorX;
	}
}

void editor_up(Editor *ed)
{
	if(ed->CursorY == 0)
	{
		ed->CursorX = 0;
	}
	else
	{
		--ed->CursorY;
		if(ed->CursorY < ed->PageY)
		{
			ed->PageY = ed->CursorY;
		}
	}
}

void editor_down(Editor *ed)
{
	i32 max = (i32)vector_len(&ed->Lines) - 1;
	if(ed->CursorY >= max)
	{
		ed->CursorX = (i32)vector_len(vector_get(&ed->Lines, max));
	}
	else
	{
		++ed->CursorY;
		if(ed->CursorY >= ed->PageY + ed->PageH)
		{
			ed->PageY = ed->CursorY - ed->PageH;
		}
	}
}
