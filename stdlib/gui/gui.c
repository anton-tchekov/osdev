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

/** Height of the title bar in pixels */
#define TITLE_BAR_HEIGHT 20

#define INPUT_PADDING     5

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
	/* TODO: Centered and Right Aligned Text */
	font_string(l->X, TITLE_BAR_HEIGHT + l->Y, l->Text,
		font_default, COLOR_BLACK, COLOR_WHITE);
}

/* BUTTON */

/**
 * @brief Renders an button on the screen.
 *
 * @param b Pointer to the Button structure
 * @param sel A boolean value indicating if the input field is selected or not
 */
static void button_render(Button *b, bool sel)
{
	i32 x, y;

	y = TITLE_BAR_HEIGHT + b->Y;
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
	font_string(x, y + b->H / 2 - 5,
		b->Text, font_default, COLOR_BLACK, COLOR_WHITE);
}

/* INPUT */

/**
 * @brief Renders an input field on the screen.
 *
 * @param i Pointer to the Input structure
 * @param sel A boolean value indicating if the input field is selected or not
 */
static void input_render(Input *i, bool sel)
{
	i32 y = TITLE_BAR_HEIGHT + i->Y;

	gfx_rect(i->X + 1, TITLE_BAR_HEIGHT + i->Y + 1, i->W - 2, 20 - 2, COLOR_WHITE);
	if(sel)
	{
		gfx_rect_border(i->X, TITLE_BAR_HEIGHT + i->Y, i->W, 20, 2, COLOR_RED);

		/* Cursor */
		gfx_rect(i->X + 5 +
			font_string_width_len(i->Text, i->Position, font_default) - 1,
			TITLE_BAR_HEIGHT + i->Y + 3, 1, 14, COLOR_BLACK);
	}
	else
	{
		gfx_rect_border(i->X, y, i->W, 20, 1, COLOR_BLACK);
	}

	font_string(i->X + 5, y + 5, i->Text,
		font_default, COLOR_BLACK, COLOR_WHITE);
}

/**
 * @brief Increases the size of an input field's buffer by a specified amount.
 *
 * @param i Pointer to the Input structure
 * @param n The number of characters by which to grow the input buffer
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
 * @brief Decreases the size of an input field's buffer by a specified amount,
 *        removing characters to the left of the cursor position.
 *
 * @param i Pointer to the Input structure
 * @param n The number of characters by which to shrink the input buffer
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
 * @brief Inserts a character into an input field's buffer at the current
 *        cursor position.
 *
 * @param i Pointer to the Input structure
 * @param c The character to insert into the input buffer
 */
static void input_insert(Input *i, i32 c)
{
	i->Text[i->Position] = c;
	if(font_string_width_len(i->Text, i->Position + 1, font_default) >=
		i->W - 2 * INPUT_PADDING)
	{
		return;
	}

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
 * @brief Deletes the character to the left of the cursor
 *
 * @param i Pointer to the Input structure
 */
static void input_backspace(Input *i)
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
 * @brief Move input cursor one position to the left and render input.
 *
 * @param i Pointer to the Input structure
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
 * @brief Move input cursor one position to the right and render input.
 *
 * @param i Pointer to the Input structure
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
 * @brief Forward a key event to an input field
 *
 * @param i The input
 * @param c The character
 */
static void input_event_key(Input *i, i32 c)
{
	/* TODO: Text selection */
	if(c >= 32 && c <= 126)
	{
		input_insert(i, c);
	}
	else if(c == '\b')
	{
		input_backspace(i);
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

/**
 * @brief Render an element by taking its selection status from the
 *        current window
 *
 * @param e Pointer to the Element
 */
static void element_render(Element *e)
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
	gfx_rect(0, 0, GFX_WIDTH, 20, COLOR_BLUE);

	/* Background */
	gfx_rect(0, TITLE_BAR_HEIGHT, GFX_WIDTH, GFX_HEIGHT - 20, COLOR_WHITE);

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
	if(_current_window->Selected < 0)
	{
		element_first();
	}

	window_render(window);
}

void window_event_key(Key key, KeyState state)
{
	void *ce;
	if(!_current_window)
	{
		return;
	}

	if(_current_window->OnKey)
	{
		_current_window->OnKey(key, state);
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
	else if(key == KEY_TAB)
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
