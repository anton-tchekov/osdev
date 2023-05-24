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
#include <clipboard.h>

/** Height of the title bar in pixels */
#define TITLE_BAR_HEIGHT 20

/** Title bar text left offset */
#define TITLE_OFFSET_X    4

/** Title bar text top offset */
#define TITLE_OFFSET_Y    4

/** Border width when not selected */
#define BORDER_SIZE       1

/** Border width when selected */
#define BORDER_SIZE_SEL   2

/* Input */

/** Height of an input field */
#define INPUT_HEIGHT     24

/** Input field text X padding */
#define INPUT_PADDING_X   6

/** Input field text Y padding */
#define INPUT_PADDING_Y   7

/** Input field cursor width */
#define CURSOR_WIDTH      1

/** Input field cursor height */
#define CURSOR_HEIGHT    14

/** Input field cursor X offset */
#define CURSOR_OFFSET    -1

/** Input field cursor Y offset */
#define CURSOR_OFFSET_Y   5

/** Input field initial capacity */
#define INPUT_CAPACITY   16

/* Default Theme */

/** Theme Black Color */
#define THEME_BLACK  0x0F0000FF

/** Theme Dark Color */
#define THEME_DARK   0x2F0000FF

/** Theme Middle Color */
#define THEME_MIDDLE 0x4F0000FF

/** Theme Light Color */
#define THEME_LIGHT  0xFF8000FF

/** Pointer to the currently open window */
static Window *_current_window;

/** Default Theme */
static Theme _default_theme =
{
	.ColorFG = THEME_LIGHT,
	.ColorBG = THEME_BLACK,
	.ColorTextSelBG = THEME_LIGHT,
	.ColorTextSelFG = THEME_DARK,
	.ColorBorder = THEME_MIDDLE,
	.ColorBorderSel = THEME_LIGHT,
	.ElementBG = THEME_DARK,
	.ElementSelBG = THEME_MIDDLE,
	.ColorCursor = THEME_LIGHT,
	.ColorTitleBar = THEME_DARK,
};

/** Pointer to the currently used theme */
static Theme *_current_theme = &_default_theme;

/** Generic element (header of every element) */
typedef struct
{
	/** Element type */
	ElementType Type;
} Element;

/** Password stars */
static const char _password_stars[] = "***********************************";

/* LABEL */

/**
 * @brief Render a label on the screen
 *
 * @param l The label
 */
static void label_render(Label *l)
{
	u32 align = l->Flags & ALIGN_MASK;
	if(align == FLAG_ALIGN_CENTER)
	{
		font_string(
			l->X - font_string_width(l->Text, font_default) / 2,
			TITLE_BAR_HEIGHT + l->Y,
			l->Text,
			font_default,
			_current_theme->ColorFG,
			_current_theme->ColorBG);
	}
	else if(align == FLAG_ALIGN_RIGHT)
	{
		font_string(
			l->X - font_string_width(l->Text, font_default),
			TITLE_BAR_HEIGHT + l->Y,
			l->Text,
			font_default,
			_current_theme->ColorFG,
			_current_theme->ColorBG);
	}
	else if(align == FLAG_ALIGN_LEFT)
	{
		font_string(
			l->X,
			TITLE_BAR_HEIGHT + l->Y,
			l->Text,
			font_default,
			_current_theme->ColorFG,
			_current_theme->ColorBG);
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

	inner_color = sel ? _current_theme->ElementSelBG :
		_current_theme->ElementBG;

	gfx_rect(b->X, y, b->W, b->H, inner_color);

	if(sel)
	{
		gfx_rect_border(b->X, y, b->W, b->H, BORDER_SIZE_SEL,
			_current_theme->ColorBorderSel);
	}
	else
	{
		gfx_rect_border(b->X, y, b->W, b->H, BORDER_SIZE,
			_current_theme->ColorBorder);
	}

	x = b->X + b->W / 2 - font_string_width(b->Text, font_default) / 2;
	font_string(x, y + b->H / 2 - 5,
		b->Text, font_default,
		_current_theme->ColorFG,
		inner_color);
}

/* INPUT */

/**
 * @brief Renders an input field on the screen
 *
 * @param i Pointer to the input structure
 * @param sel A boolean value indicating if the input field is selected or not
 */
static void input_render(Input *i, bool sel)
{
	Color inner_color;
	i32 y = TITLE_BAR_HEIGHT + i->Y;

	const char *text = i->Flags & FLAG_PASSWORD ?
		_password_stars : vector_data(&i->Text);

	inner_color = sel ?
		_current_theme->ElementSelBG :
		_current_theme->ElementBG;

	gfx_rect(i->X + BORDER_SIZE,
		TITLE_BAR_HEIGHT + i->Y + BORDER_SIZE,
		i->W - 2 * BORDER_SIZE,
		INPUT_HEIGHT - 2 + BORDER_SIZE,
		inner_color);

	if(sel)
	{
		gfx_rect_border(i->X, TITLE_BAR_HEIGHT + i->Y,
			i->W, INPUT_HEIGHT, BORDER_SIZE_SEL,
			_current_theme->ColorBorderSel);
	}
	else
	{
		gfx_rect_border(i->X, y, i->W, INPUT_HEIGHT, BORDER_SIZE,
			_current_theme->ColorBorder);
	}

	if(i->Selection == i->Position)
	{
		font_string_len(i->X + INPUT_PADDING_X, y + INPUT_PADDING_Y,
			text, vector_len(&i->Text),
			font_default,
			_current_theme->ColorFG,
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
			_current_theme->ColorFG,
			inner_color);

		/* Selection */
		sel_x = font_string_width_len(text, sel_start, font_default);

		gfx_rect(i->X + INPUT_PADDING_X + sel_x, y + CURSOR_OFFSET_Y,
			font_string_width_len(text + sel_start, sel_len, font_default),
			CURSOR_HEIGHT,
			_current_theme->ColorFG);

		font_string_len(i->X + INPUT_PADDING_X + sel_x, y + INPUT_PADDING_Y,
			text + sel_start, sel_len,
			font_default,
			_current_theme->ColorTextSelFG,
			_current_theme->ColorTextSelBG);

		/* After selection */
		font_string_len(i->X + INPUT_PADDING_X +
			font_string_width_len(text, sel_start + sel_len, font_default),
			y + INPUT_PADDING_Y,
			text + sel_start + sel_len,
			vector_len(&i->Text) - sel_start - sel_len,
			font_default,
			_current_theme->ColorFG,
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
			_current_theme->ColorCursor);
	}
}

void input_clear(Input *i)
{
	vector_clear(&i->Text);
	input_render(i, true);
}

/**
 * @brief Inserts a string into an input field's buffer at the current
 *        cursor position, replacing the current selection
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
 * @brief Move input cursor one position to the left and render input
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

/**
 * @brief Move the cursor one position to the left and select
 *
 * @param i Pointer to the input structure
 */
static void input_select_left(Input *i)
{
	if(i->Position > 0)
	{
		--i->Position;
		input_render(i, true);
	}
}

/**
 * @brief Move input cursor one position to the right and render input
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

/**
 * @brief Move the cursor one position to the right and select
 *
 * @param i Pointer to the input structure
 */
static void input_select_right(Input *i)
{
	if(i->Position < (i32)vector_len(&i->Text))
	{
		++i->Position;
		input_render(i, true);
	}
}

/**
 * @brief Delete the character before the cursor
 *
 * @param i Pointer to the input structure
 */
static void input_backspace(Input *i)
{
	if(i->Selection != i->Position)
	{
		input_selection_replace(i, NULL, 0);
	}
	else if(i->Position > 0)
	{
		--i->Position;
		i->Selection = i->Position;
		vector_remove(&i->Text, i->Position);
		input_render(i, true);
	}
}

/**
 * @brief Delete the character after the cursor
 *
 * @param i Pointer to the input structure
 */
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

/**
 * @brief Move the cursor to the beginning of the input field
 *
 * @param i Pointer to the input structure
 */
static void input_home(Input *i)
{
	i->Selection = 0;
	i->Position = 0;
	input_render(i, true);
}

/**
 * @brief Select the characters from the current cursor position
 *        to the beginning
 *
 * @param i Pointer to the input structure
 */
static void input_select_home(Input *i)
{
	i->Position = 0;
	input_render(i, true);
}

/**
 * @brief Move the cursor to the end of the input field
 *
 * @param i Pointer to the input structure
 */
static void input_end(Input *i)
{
	i->Position = vector_len(&i->Text);
	i->Selection = i->Position;
	input_render(i, true);
}

/**
 * @brief Select the characters from the current cursor position to the end
 *
 * @param i Pointer to the input structure
 */
static void input_select_end(Input *i)
{
	i->Position = vector_len(&i->Text);
	input_render(i, true);
}

/**
 * @brief Insert a character at the cursor position
 *
 * @param i Pointer to the input structure
 * @param chr
 */
static void input_char(Input *i, i32 chr)
{
	char ins = chr;
	input_selection_replace(i, &ins, 1);
}

/**
 * @brief Do a CTRL+A select all operation on an input field
 *
 * @param i Pointer to the input structure
 */
static void input_select_all(Input *i)
{
	i->Selection = 0;
	i->Position = vector_len(&i->Text);
	input_render(i, true);
}

/**
 * @brief Save the current selection of an input to the clipboard
 *
 * @param i Pointer to the input structure
 */
static void _selection_save(Input *i)
{
	i32 sel_start, sel_len;

	sel_start = i32_min(i->Selection, i->Position);
	sel_len = i32_max(i->Selection, i->Position) - sel_start;
	clipboard_save_text_len(vector_get(&i->Text, sel_start), sel_len);
}

/**
 * @brief Do a CTRL+C copy operation on an input field
 *
 * @param i Pointer to the input structure
 */
static void input_copy(Input *i)
{
	_selection_save(i);
}

/**
 * @brief Do a CTRL+X cut operation on an input field
 *
 * @param i Pointer to the input structure
 */
static void input_cut(Input *i)
{
	_selection_save(i);
	input_selection_replace(i, NULL, 0);
}

/**
 * @brief Do a CTRL+V paste operation on an input field
 *
 * @param i Pointer to the input structure
 */
static void input_paste(Input *i)
{
	input_selection_replace(i, clipboard_get_text(),
		clipboard_get_text_len());
}

/**
 * @brief Forward a key event to an input field
 *
 * @param i The input
 * @param key The key that was pressed
 * @param chr The character that corresponds to the key
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
 * @brief Render an element based on its type and selection status
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
		_current_theme->ColorTitleBar);

	gfx_rect(0, TITLE_BAR_HEIGHT - 1, GFX_WIDTH, 1,
		_current_theme->ColorFG);

	/* Background */
	gfx_rect(0, TITLE_BAR_HEIGHT, GFX_WIDTH, GFX_HEIGHT - TITLE_BAR_HEIGHT,
		_current_theme->ColorBG);

	/* Title */
	font_string(TITLE_OFFSET_X, TITLE_OFFSET_Y, window->Title,
		font_default, _current_theme->ColorFG,
		_current_theme->ColorTitleBar);

	for(i = 0; i < window->Count; ++i)
	{
		element_render(window->Elements[i]);
	}
}

void window_open(Window *window)
{
	_current_window = window;
	if(window->Selected < 0)
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

void window_init(Window *window, char *title, void *elems, i32 count,
	KeyEvent onkey)
{
	window->Selected = -1;
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

void theme_set(Theme *theme)
{
	_current_theme = theme;
}

void theme_default(void)
{
	_current_theme = &_default_theme;
}
