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
#include <font-default.h>

/** TODO */
typedef struct ELEMENT
{
	/** TODO */
	ElementType Type;
} Element;

/** TODO */
static Window *_current_window;

/** TODO */
static i32 _current_window->Selected;

/* LABEL */

/**
 * @brief TODO
 *
 * @param l TODO
 */
static void label_render(Label *l)
{
	font_string(l->X, 20 + l->Y, l->Text,
		font_default, COLOR_BLACK, COLOR_WHITE);
}

/* BUTTON */

/**
 * @brief TODO
 *
 * @param b TODO
 * @param sel TODO
 */
static void button_render(Button *b, bool sel)
{
	i32 x, y;

	y = 20 + b->Y;
	gfx_rect(b->X, y, b->W, b->H, COLOR_WHITE);
	if(sel)
	{
		gfx_rect_border(b->X, y, b->W, b->H, 2, COLOR_RED);
	}
	else
	{
		gfx_rect_border(b->X, y, b->W, b->H, 1, COLOR_BLACK);
	}

	x = b->X + b->W / 2 - font_string_width(b->Text, font_default) / 2;
	font_string(x, y, b->Text, font_default, COLOR_BLACK, COLOR_WHITE);
}

/* INPUT */

/**
 * @brief TODO
 *
 * @param i TODO
 * @param sel TODO
 */
static void input_render(Input *i, bool sel)
{
	gfx_rect(i->X + 1, 20 + i->Y + 1, i->W - 2, 20 - 2, COLOR_WHITE);
	if(sel)
	{
		gfx_rect_border(i->X, 20 + i->Y, i->W, 20, 2, COLOR_RED);

		/* Cursor */
		gfx_rect(i->X + 5 +
			font_string_width_len(i->Text, i->Position, font_default) - 1,
			20 + i->Y + 3, 1, 14, COLOR_BLACK);
	}
	else
	{
		gfx_rect_border(i->X, 20 + i->Y, i->W, 20, 1, COLOR_BLACK);
	}

	font_string(i->X + 5, 20 + i->Y + 5, i->Text,
		font_default, COLOR_BLACK, COLOR_WHITE);
}

/**
 * @brief TODO
 *
 * @param i TODO
 * @param n TODO
 */
static void input_grow(Input *i, i32 n)
{
	if(i->Text[i->Position])
	{
		i32 j;
		for(j = i->Length - 1; j >= i->Position; --j)
		{
			i->Text[j + n] = i->Text[j];
		}
	}

	i->Length += n;
	i->Text[i->Length] = '\0';
}

/**
 * @brief TODO
 *
 * @param i TODO
 * @param n TODO
 */
static void input_shrink(Input *i, i32 n)
{
	if(i->Text[i->Position])
	{
		i32 j = i->Position;
		for(; i->Text[j]; ++j)
		{
			i->Text[j - n] = i->Text[j];
		}
	}
}

/**
 * @brief TODO
 *
 * @param i TODO
 * @param c TODO
 */
static void input_insert(Input *i, i32 c)
{
	if(i->Length + 1 < i->Size)
	{
		input_grow(i, 1);
		i->Text[i->Position++] = c;
		input_render(i, 1);
	}
}

void input_clear(Input *i)
{
	i->Length = 0;
	i->Position = 0;
	i->Text[0] = '\0';
	input_render(i, 1);
}

/**
 * @brief TODO
 *
 * @param i TODO
 */
static void input_delete(Input *i)
{
	if(i->Position > 0)
	{
		input_shrink(i, 1);
		--i->Position;
		--i->Length;
		i->Text[i->Length] = '\0';
		input_render(i, 1);
	}
}

/**
 * @brief TODO
 *
 * @param i TODO
 */
static void input_left(Input *i)
{
	if(i->Position > 0)
	{
		--i->Position;
		input_render(i, 1);
	}
}

/**
 * @brief TODO
 *
 * @param i TODO
 */
static void input_right(Input *i)
{
	if(i->Position < i->Length)
	{
		++i->Position;
		input_render(i, 1);
	}
}

/**
 * @brief TODO
 *
 * @param i TODO
 * @param c TODO
 */
static void input_event_key(Input *i, i32 c)
{
	if(c >= 32 && c <= 126)
	{
		input_insert(i, c);
	}
	else if(c == '\b')
	{
		input_delete(i);
	}
}

/* ELEMENT */

/**
 * @brief TODO
 *
 * @param e TODO
 * @param sel TODO
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

/**
 * @brief TODO
 *
 * @param e TODO
 */
static void element_render(Element *e)
{
	element_render_sel(e, _current_window->Selected >= 0 &&
		_current_window->Elements[_current_window->Selected] == e);
}

/**
 * @brief TODO
 */
static void element_first(void)
{
	void **elems;
	i32 i, count;

	count = _current_window->Count;
	elems = _current_window->Elements;
	for(i = 0; i < count; ++i)
	{
		ElementType type = ((Element *)elems[i])->Type;
		if(type == ELEMENT_TYPE_BUTTON ||
			type == ELEMENT_TYPE_INPUT)
		{
			_current_window->Selected = i;
			break;
		}
	}
}

/**
 * @brief TODO
 */
static void element_next(void)
{
	void **elems;
	i32 i, count;

	count = _current_window->Count;
	elems = _current_window->Elements;
	for(i = _current_window->Selected + 1; i < count; ++i)
	{
		ElementType type = ((Element *)elems[i])->Type;
		if(type == ELEMENT_TYPE_BUTTON ||
			type == ELEMENT_TYPE_INPUT)
		{
			element_render_sel(elems[_current_window->Selected], false);
			_current_window->Selected = i;
			element_render_sel(elems[_current_window->Selected], true);
			break;
		}
	}
}

/**
 * @brief TODO
 */
static void element_prev(void)
{
	void **elems;
	i32 i;

	elems = _current_window->Elements;
	for(i = _current_window->Selected - 1; i >= 0; --i)
	{
		ElementType type = ((Element *)elems[i])->Type;
		if(type == ELEMENT_TYPE_BUTTON ||
			type == ELEMENT_TYPE_INPUT)
		{
			element_render_sel(elems[_current_window->Selected], false);
			_current_window->Selected = i;
			element_render_sel(elems[_current_window->Selected], true);
			break;
		}
	}
}

/* WINDOW */
void window_render(Window *window)
{
	i32 i;

	/* Title Bar */
	gfx_rect(0, 0, GFX_WIDTH, 20, COLOR_BLUE);

	/* Background */
	gfx_rect(0, 20, GFX_WIDTH, 220, COLOR_WHITE);

	/* Title */
	font_string(4, 4, window->Title,
		font_default, COLOR_WHITE, COLOR_BLUE);

	for(i = 0; i < window->Count; ++i)
	{
		element_render(window->Elements[i]);
	}
}

void window_open(Window *window)
{
	_current_window = window;
	_current_window->Selected = -1;
	element_first();
	window_render(window);
}

void window_event_key(Key key, bool up)
{
	void *ce;
	if(!_current_window)
	{
		return;
	}

	if(_current_window->OnKey)
	{
		_current_window->OnKey(key, up);
	}

	if(_current_window->Selected < 0)
	{
		return;
	}

	if(up)
	{
		return;
	}

	ce = &_current_window->Elements[_current_window->Selected];
	if(key == KEY_UP)
	{
		element_prev();
	}
	else if(key == KEY_DOWN)
	{
		element_next();
	}
	else
	{
		ElementType type = ((Element *)ce)->Type;
		if(type == ELEMENT_TYPE_BUTTON)
		{
			if(key == KEY_ENTER)
			{
				((Button *)ce)->Click();
			}
		}
		else if(type == ELEMENT_TYPE_INPUT)
		{
			Input *input = (Input *)ce;
			if(key == KEY_LEFT)
			{
				input_left(input);
			}
			else if(key == KEY_RIGHT)
			{
				input_right(input);
			}
			else
			{
				input_event_key(input, key);
			}
		}
	}
}
