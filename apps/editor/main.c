/**
 * @file    main.c
 * @author  Anton Tchekov
 * @version 0.1
 * @date    24.05.2023
 *
 * @brief   Text/Code Editor App
 */

#include <std.h>

//! @cond Doxygen_Suppress

typedef struct
{
	u32 ColorComment, ColorNumber, ColorString, ColorKeyword;
} SyntaxHighlighter;

void event_key(Key key, i32 chr, KeyState state)
{
}

void setup(void)
{
	keyboard_register_event(event_key);
}

void loop(void)
{
}

#ifdef EDITOR

#include <types.h>
#include <vector.h>

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

#include "editor.h"
#include <std.h>

void editor_init(Editor *ed)
{
	ed->TabSize = 4;
	ed->LineNumberDigits = 4;
	ed->Cursor = 0;

	vector_init(&ed->Text, sizeof(char), 128);

	ed->Screen = memalloc((ed->PageW + 1 + ed->LineNumberDigits) * ed->PageH);
}

void editor_render(Editor *ed)
{
	i32 x, y;
	i32 line;
	char c;
	const char *s, *text;

	line = 1;
	x = 0;
	y = 0;

	text = vector_data(&ed->Text);

	for(s = text; (c = *s); ++s)
	{
		if(c == '\n')
		{
			++line;
			++y;
			x = 0;

			/* Print line number */
			debug_print("\n%4d | ", line);
		}
		else
		{
			debug_print("%c", c);
		}
	}
}


void editor_char(Editor *ed, char c)
{

}

void editor_backspace(Editor *ed)
{

}

void editor_delete(Editor *ed)
{

}

void editor_new_line(Editor *ed)
{

}

void editor_home(Editor *ed)
{

}

void editor_end(Editor *ed)
{

}

void editor_page_up(Editor *ed)
{

}

void editor_page_down(Editor *ed)
{

}

void editor_left(Editor *ed)
{

}

void editor_right(Editor *ed)
{

}

void editor_up(Editor *ed)
{

}

void editor_down(Editor *ed)
{

}

/* --- PUBLIC --- */
void editor_init(Editor *ed)
{
	ed->TabSize = 4;
	ed->LineNumberDigits = 4;

	vector_init(&ed->Lines, sizeof(Vector), 128);
	vector_push(&ed->Lines, ector_get(&ed->Lines, 0), sizeof(char), 8);

	ed->CursorX = 0;
	ed->CursorY = 0;

	ed->PageW = 30;
	ed->PageH = 50;
	ed->PageY = 0;
	ed->PageX = 0;

	ed->Screen = memalloc((ed->PageW + 1 + ed->LineNumberDigits) * ed->PageH);
}


void editor_render(Editor *ed)
{
	/* keywords blue, strings/chars red, comments green, brackets yellow,
	numbers light green */
	i32 x, y, w, h;
	w = ed->PageW;
	h = ed->PageH;
	for(y = 0; y < h; ++y)
	{
		for(x = 0; x < w; ++x)
		{
			/* only update the parts that have changed */

			editor_render_char(x, y, bg_color, fg_color);
		}
	}

	/* draw cursor */
	editor_render_cursor();
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

static void _render_char(i32 x, i32 y, char c, Color fg, Color bg)
{

}

static void _render_line(Line *line, i32 line_number)
{
	/* syntax highlighting */
	static const char *_keywords[] =
	{
		"var",
		"return",
		"continue",
		"break",
		"if",
		"elif",
		"else",
		"for",
		"to",
		"loop",
		"do",
		"while",
		"switch",
		"case",
		"default"
	};

	char *p;
	i32 i, len;

	p = line->Buffer;
	len = line->Length;
	for(i = 0; i < len; ++i)
	{
		c = p[i];
		if(c == '/')
		{
			if(i + i < len && p[i + 1] == '/')
			{
				for(; i < len; ++i)
				{
					c = p[i];
					_render_char(x, y, c, COLOR_GREEN, COLOR_BLACK);
				}
			}
		}
		else if(c == '\"')
		{
			/* string start */
			while(c != '\"')
			{
				/* skip escape sequence */
				if(c == '\\')
				{
					++i;
				}

				_render_char(x, y, c, COLOR_RED, COLOR_BLACK);
			}
		}
		else if(c == '{' || c == '}' ||
			c == '[' || c == ']' ||
			c == '(' || c == ')')
		{
			_render_char(x, y, c, COLOR_YELLOW, COLOR_BLACK);
		}
		else if(is_identifer_start_char(c))
		{
			i32 id_start;

			id_start = i;
			while(is_identifer_char(c))
			{
			}

			if(is_identifier())
			{

			}
		}
	}
}

#endif /* EDITOR */

//! @endcond
