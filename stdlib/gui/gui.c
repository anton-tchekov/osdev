/**
 * @file    gui.c
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    23.04.2023
 */

#include <gui.h>
#include <gfx.h>
#include <font.h>
#include <colors.h>
#include <font_default.h>
#include <ctype.h>
#include <utf8.h>

/** Height of the title bar in pixels */
#define TITLE_BAR_HEIGHT 20

#define TITLE_OFFSET_X    4
#define TITLE_OFFSET_Y    4

#define BORDER_SIZE       1
#define BORDER_SIZE_SEL   2

/** Input */
#define INPUT_HEIGHT     24

#define INPUT_PADDING_X   6
#define INPUT_PADDING_Y   7

#define CURSOR_WIDTH      1
#define CURSOR_HEIGHT    14
#define CURSOR_OFFSET    -1

#define CURSOR_OFFSET_Y   5

#define INPUT_CAPACITY   16

/** Pointer to the currently open window */
static Window *_current_window;

/** Generic element */
typedef struct
{
	/** Element type */
	ElementType Type;
} Element;

/* LABEL */

/**
 * @brief Render a label on the screen
 *
 * @param l The label
 */
static void label_render(Label *l)
{
	u32 align = l->Flags & LABEL_ALIGN_MASK;
	if(align == FLAG_ALIGN_CENTER)
	{
		font_string(
			l->X - font_string_width(l->Text, font_default) / 2,
			TITLE_BAR_HEIGHT + l->Y,
			l->Text,
			font_default,
			_current_window->ColorFG,
			_current_window->ColorBG);
	}
	else if(align == FLAG_ALIGN_RIGHT)
	{
		font_string(
			l->X - font_string_width(l->Text, font_default),
			TITLE_BAR_HEIGHT + l->Y,
			l->Text,
			font_default,
			_current_window->ColorFG,
			_current_window->ColorBG);
	}
	else if(align == FLAG_ALIGN_LEFT)
	{
		font_string(
			l->X,
			TITLE_BAR_HEIGHT + l->Y,
			l->Text,
			font_default,
			_current_window->ColorFG,
			_current_window->ColorBG);
	}
}

/* BUTTON */

/**
 * @brief Renders an button on the screen.
 *
 * @param b Pointer to the button structure
 * @param sel A boolean value indicating if the input field is selected or not
 */
static void button_render(Button *b, bool sel)
{
	Color inner_color;
	i32 x, y;

	y = TITLE_BAR_HEIGHT + b->Y;

	inner_color = sel ? _current_window->ElementSelBG :
		_current_window->ElementBG;

	gfx_rect(b->X, y, b->W, b->H, inner_color);

	if(sel)
	{
		gfx_rect_border(b->X, y, b->W, b->H, BORDER_SIZE_SEL,
			_current_window->ColorBorderSel);
	}
	else
	{
		gfx_rect_border(b->X, y, b->W, b->H, BORDER_SIZE,
			_current_window->ColorBorder);
	}

	x = b->X + b->W / 2 - font_string_width(b->Text, font_default) / 2;
	font_string(x, y + b->H / 2 - 5,
		b->Text, font_default,
		_current_window->ColorFG,
		inner_color);
}

/* INPUT */

/**
 * @brief Renders an input field on the screen.
 *
 * @param i Pointer to the input structure
 * @param sel A boolean value indicating if the input field is selected or not
 */
static void input_render(Input *i, bool sel)
{
	Color inner_color;
	i32 y = TITLE_BAR_HEIGHT + i->Y;

	const char *text = vector_data(&i->Text);

	inner_color = sel ? _current_window->ElementSelBG : _current_window->ElementBG;
	gfx_rect(i->X + BORDER_SIZE,
		TITLE_BAR_HEIGHT + i->Y + BORDER_SIZE,
		i->W - 2 * BORDER_SIZE,
		INPUT_HEIGHT - 2 + BORDER_SIZE,
		inner_color);

	if(sel)
	{
		gfx_rect_border(i->X, TITLE_BAR_HEIGHT + i->Y,
			i->W, INPUT_HEIGHT, BORDER_SIZE_SEL,
			_current_window->ColorBorderSel);
	}
	else
	{
		gfx_rect_border(i->X, y, i->W, INPUT_HEIGHT, BORDER_SIZE,
			_current_window->ColorBorder);
	}

	if(i->Selection == i->Position)
	{
		font_string_len(i->X + INPUT_PADDING_X, y + INPUT_PADDING_Y,
			text, vector_len(&i->Text),
			font_default,
			_current_window->ColorFG,
			inner_color);
	}
	else
	{
		i32 sel_start, sel_len, sel_x;

		sel_start = i32_min(i->Selection, i->Position);
		sel_len = i32_max(i->Selection, i->Position) - sel_start;

		/* Before selection */
		font_string_len(i->X + INPUT_PADDING_X,
			y + INPUT_PADDING_Y,
			text, sel_start,
			font_default,
			_current_window->ColorFG,
			inner_color);

		/* Selection */
		sel_x = font_string_width_len(text, sel_start, font_default);

		gfx_rect(i->X + INPUT_PADDING_X + sel_x, y + CURSOR_OFFSET_Y,
			font_string_width_len(text + sel_start, sel_len, font_default),
			CURSOR_HEIGHT,
			_current_window->ColorFG);

		font_string_len(i->X + INPUT_PADDING_X + sel_x, y + INPUT_PADDING_Y,
			text + sel_start, sel_len,
			font_default,
			_current_window->ColorTextSelFG,
			inner_color);

		/* After selection */
		font_string_len(i->X + INPUT_PADDING_X +
			font_string_width_len(text, sel_start + sel_len, font_default),
			y + INPUT_PADDING_Y,
			text + sel_start + sel_len,
			vector_len(&i->Text) - sel_start - sel_len,
			font_default,
			_current_window->ColorFG,
			inner_color);
	}

	if(sel)
	{
		/* Cursor */
		gfx_rect(i->X + INPUT_PADDING_X +
			font_string_width_len(text, i->Position, font_default) +
			CURSOR_OFFSET, TITLE_BAR_HEIGHT + i->Y + CURSOR_OFFSET_Y,
			CURSOR_WIDTH,
			CURSOR_HEIGHT,
			_current_window->ColorCursor);
	}
}

void input_clear(Input *i)
{
	vector_clear(&i->Text);
	input_render(i, true);
}

/**
 * @brief Inserts a string into an input field's buffer at the current
 *        cursor position, replacing the current selection.
 *
 * @param i Pointer to the Input structure
 * @param str The string to insert into the input buffer
 * @param len The length of the string to be inserted
 */
static void input_selection_replace(Input *i, const char *str, i32 len)
{
	i32 sel_start, sel_len;

	/* TODO: This is broken */
	if(font_string_width_len(vector_data(&i->Text),
		vector_len(&i->Text), font_default) +
		font_string_width_len(str, len, font_default)
		>= i->W - 2 * INPUT_PADDING_X)
	{
		return;
	}

	sel_start = i32_min(i->Selection, i->Position);
	sel_len = i32_max(i->Selection, i->Position) - sel_start;
	vector_replace(&i->Text, sel_start, sel_len, str, len);
	i->Position = sel_start + len;
	i->Selection = i->Position;
	input_render(i, true);
}


/**
 * @brief Move input cursor one position to the left and render input.
 *
 * @param i Pointer to the input structure
 */
static void input_left(Input *i)
{
	if(i->Selection != i->Position)
	{
		i->Selection = i->Position;
		input_render(i, true);
	}
	else if(i->Position > 0)
	{
		--i->Position;
		i->Selection = i->Position;
		input_render(i, true);
	}
}

static void input_select_left(Input *i)
{
	if(i->Position > 0)
	{
		--i->Position;
		input_render(i, true);
	}
}

/**
 * @brief Move input cursor one position to the right and render input.
 *
 * @param i Pointer to the input structure
 */
static void input_right(Input *i)
{
	if(i->Selection != i->Position)
	{
		i->Selection = i->Position;
		input_render(i, true);
	}
	else if(i->Position < (i32)vector_len(&i->Text))
	{
		++i->Position;
		i->Selection = i->Position;
		input_render(i, true);
	}
}

static void input_select_right(Input *i)
{
	if(i->Position < (i32)vector_len(&i->Text))
	{
		++i->Position;
		input_render(i, true);
	}
}

static void input_backspace(Input *i)
{
	if(i->Selection != i->Position)
	{
		input_selection_replace(i, NULL, 0);
	}
	else if(i->Position > 0)
	{
		vector_remove(&i->Text, --i->Position);
		input_render(i, true);
	}
}

static void input_delete(Input *i)
{
	if(i->Selection != i->Position)
	{
		input_selection_replace(i, NULL, 0);
	}
	else if(i->Position < (i32)vector_len(&i->Text))
	{
		vector_remove(&i->Text, i->Position);
		input_render(i, true);
	}
}

static void input_home(Input *i)
{
	i->Selection = 0;
	i->Position = 0;
	input_render(i, true);
}

static void input_select_home(Input *i)
{
	i->Position = 0;
	input_render(i, true);
}

static void input_end(Input *i)
{
	i->Position = vector_len(&i->Text);
	i->Selection = i->Position;
	input_render(i, true);
}

static void input_select_end(Input *i)
{
	i->Position = vector_len(&i->Text);
	input_render(i, true);
}

static void input_char(Input *i, i32 chr)
{
	i32 cnt;
	char ins[4];
	cnt = codepoint_utf8(chr, ins);
	input_selection_replace(i, ins, cnt);
}

static void input_select_all(Input *i)
{
	i->Selection = 0;
	i->Position = vector_len(&i->Text);
	input_render(i, true);
}

static void input_copy(Input *i)
{
}

static void input_cut(Input *i)
{
}

static void input_paste(Input *i)
{
}

/**
 * @brief Forward a key event to an input field
 *
 * @param i The input
 * @param c The character
 */
static void input_event_key(Input *i, Key key, i32 chr)
{
	if(key == KEY_HOME)
	{
		input_home(i);
	}
	else if(key == (KEY_HOME | MOD_SHIFT))
	{
		input_select_home(i);
	}
	else if(key == KEY_END)
	{
		input_end(i);
	}
	else if(key == (KEY_END | MOD_SHIFT))
	{
		input_select_end(i);
	}
	else if(key == KEY_LEFT)
	{
		input_left(i);
	}
	else if(key == (KEY_LEFT | MOD_SHIFT))
	{
		input_select_left(i);
	}
	else if(key == KEY_RIGHT)
	{
		input_right(i);
	}
	else if(key == (KEY_RIGHT | MOD_SHIFT))
	{
		input_select_right(i);
	}
	else if(key == KEY_BACKSPACE)
	{
		input_backspace(i);
	}
	else if(key == KEY_DELETE)
	{
		input_delete(i);
	}
	else if(key == (KEY_A | MOD_CTRL))
	{
		input_select_all(i);
	}
	else if(key == (KEY_C | MOD_CTRL))
	{
		input_copy(i);
	}
	else if(key == (KEY_X | MOD_CTRL))
	{
		input_cut(i);
	}
	else if(key == (KEY_V | MOD_CTRL))
	{
		input_paste(i);
	}
	else if(chr)
	{
		input_char(i, chr);
	}
}

/* ELEMENT */

/**
 * @brief Render an element based on its type and selection status.
 *
 * @param e Pointer to the Element
 * @param sel Selection status of the element.
 */
static void element_render_sel(Element *e, u32 sel)
{
	switch(e->Type)
	{
		case ELEMENT_TYPE_LABEL:
			label_render((Label *)e);
			break;

		case ELEMENT_TYPE_BUTTON:
			button_render((Button *)e, sel);
			break;

		case ELEMENT_TYPE_INPUT:
			input_render((Input *)e, sel);
			break;
	}
}

void element_render(void *e)
{
	element_render_sel(e, _current_window->Selected >= 0 &&
		_current_window->Elements[_current_window->Selected] == e);
}

/**
 * @brief Checks if an element can be selected
 *
 * @param index Element index
 * @return Boolean true if the element is selectable
 */
static bool _selectable(i32 index)
{
	ElementType type = ((Element *)_current_window->Elements[index])->Type;
	return type == ELEMENT_TYPE_BUTTON ||
			type == ELEMENT_TYPE_INPUT;
}

/**
 * @brief Selects the element
 *
 * @param index Element index
 */
static void _select(i32 index)
{
	void **elems = _current_window->Elements;
	element_render_sel(elems[_current_window->Selected], false);
	_current_window->Selected = index;
	element_render_sel(elems[_current_window->Selected], true);
}

/**
 * @brief Select the first element
 */
static void element_first(void)
{
	i32 i, count = _current_window->Count;
	for(i = 0; i < count; ++i)
	{
		if(_selectable(i))
		{
			_current_window->Selected = i;
			break;
		}
	}
}

/**
 * @brief Select the next element
 */
static void element_next(void)
{
	i32 i, count = _current_window->Count;
	for(i = _current_window->Selected + 1; i < count; ++i)
	{
		if(_selectable(i))
		{
			_select(i);
			return;
		}
	}

	/* Wrap around */
	for(i = 0; i < count; ++i)
	{
		if(_selectable(i))
		{
			_select(i);
			return;
		}
	}
}

/**
 * @brief Select the previous element
 */
static void element_prev(void)
{
	i32 i;
	for(i = _current_window->Selected - 1; i >= 0; --i)
	{
		if(_selectable(i))
		{
			_select(i);
			return;
		}
	}

	/* Wrap around */
	for(i = _current_window->Count - 1; i >= 0; --i)
	{
		if(_selectable(i))
		{
			_select(i);
			return;
		}
	}
}

/* WINDOW */

/**
 * @brief Draw a window
 *
 * @param window The window
 */
static void window_render(Window *window)
{
	i32 i;

	/* Title Bar */
	gfx_rect(0, 0, GFX_WIDTH, TITLE_BAR_HEIGHT - 1,
		_current_window->ColorTitleBar);

	gfx_rect(0, TITLE_BAR_HEIGHT - 1, GFX_WIDTH, 1,
		_current_window->ColorFG);

	/* Background */
	gfx_rect(0, TITLE_BAR_HEIGHT, GFX_WIDTH, GFX_HEIGHT - TITLE_BAR_HEIGHT,
		_current_window->ColorBG);

	/* Title */
	font_string(TITLE_OFFSET_X, TITLE_OFFSET_Y, window->Title,
		font_default, _current_window->ColorFG,
		_current_window->ColorTitleBar);

	for(i = 0; i < window->Count; ++i)
	{
		element_render(window->Elements[i]);
	}
}

void window_open(Window *window)
{
	_current_window = window;
	if(_current_window->Selected < 0)
	{
		element_first();
	}

	window_render(window);
}

void window_event_key(Key key, i32 chr, KeyState state)
{
	void *ce;
	if(!_current_window)
	{
		return;
	}

	if(_current_window->OnKey)
	{
		_current_window->OnKey(key, chr, state);
	}

	if(_current_window->Selected < 0)
	{
		return;
	}

	if(state == KEYSTATE_RELEASED)
	{
		return;
	}

	ce = _current_window->Elements[_current_window->Selected];
	if(key == KEY_TAB)
	{
		element_next();
	}
	else if(key == (KEY_TAB | MOD_SHIFT))
	{
		element_prev();
	}
	else
	{
		ElementType type = ((Element *)ce)->Type;
		if(type == ELEMENT_TYPE_BUTTON)
		{
			if(key == KEY_RETURN)
			{
				Button *b = ce;
				if(b->Click)
				{
					b->Click();
				}
			}
		}
		else if(type == ELEMENT_TYPE_INPUT)
		{
			input_event_key((Input *)ce, key, chr);
		}
	}
}

#define THEME_BLACK  0x0F0000FF
#define THEME_DARK   0x2F0000FF
#define THEME_MIDDLE 0x4F0000FF
#define THEME_LIGHT  0xFF8000FF

void window_init(Window *window, char *title, void *elems, i32 count,
	KeyEvent onkey)
{
	window->ColorFG = THEME_LIGHT;
	window->ColorBG = THEME_BLACK;
	window->ColorTextSelBG = THEME_LIGHT;
	window->ColorTextSelFG = THEME_DARK;
	window->ColorBorder = THEME_MIDDLE;
	window->ColorBorderSel = THEME_LIGHT;
	window->ElementBG = THEME_DARK;
	window->ElementSelBG = THEME_MIDDLE;
	window->ColorCursor = THEME_LIGHT;
	window->ColorTitleBar = THEME_DARK;

	window->Title = title;
	window->Elements = elems;
	window->Count = count;
	window->OnKey = onkey;
}

void label_init(Label *label, i32 x, i32 y, u32 flags, char *text)
{
	label->Type = ELEMENT_TYPE_LABEL;
	label->X = x;
	label->Y = y;
	label->Flags = flags;
	label->Text = text;
}

void button_init(Button *button, i32 x, i32 y, i32 w, i32 h, char *text,
	void (*onclick)(void))
{
	button->Type = ELEMENT_TYPE_BUTTON;
	button->X = x;
	button->Y = y;
	button->W = w;
	button->H = h;
	button->Text = text;
	button->Click = onclick;
}

void input_init(Input *input, i32 x, i32 y, i32 w)
{
	input->Type = ELEMENT_TYPE_INPUT;
	input->X = x;
	input->Y = y;
	input->W = w;
	input->Position = 0;
	input->Selection = 0;
	vector_init(&input->Text, sizeof(char), INPUT_CAPACITY);
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
