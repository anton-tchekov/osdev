/**
 * @file    gui.h
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    23.04.2023
 * @brief   Graphical User Interface Toolkit
 */

#ifndef __GUI_H__
#define __GUI_H__

#include <vector.h>
#include <types.h>
#include <keyboard.h>

/** Element type */
typedef enum
{
	ELEMENT_TYPE_BUTTON,
	ELEMENT_TYPE_INPUT,
	ELEMENT_TYPE_LABEL
} ElementType;

/** Label align bitmask */
#define LABEL_ALIGN_MASK  0x03

/** Left align label (default) */
#define LABEL_FLAG_LEFT   0x00

/** Center align label */
#define LABEL_FLAG_CENTER 0x01

/** Right align label */
#define LABEL_FLAG_RIGHT  0x02

/** Label element structure */
typedef struct
{
	/** Element Type (must be first member) */
	ElementType Type;

	/** Various Flags (see above) */
	u8 Flags;

	/** X-Coordinate */
	i32 X;

	/** Y-Coordinate */
	i32 Y;

	/** Label text */
	char *Text;
} Label;

/** Button element structure */
typedef struct
{
	/** Element Type (must be first member) */
	ElementType Type;

	/** X-Coordinate */
	i32 X;

	/** Y-Coordinate */
	i32 Y;

	/** Width */
	i32 W;

	/** Height */
	i32 H;

	/** Button text */
	char *Text;

	/** OnClick event */
	void (*Click)(void);
} Button;

/** Input element structure */
typedef struct
{
	/** Element Type (must be first member) */
	ElementType Type;

	/** X-Coordinate */
	i32 X;

	/** Y-Coordinate */
	i32 Y;

	/** Width */
	i32 W;

	/** Cursor position */
	i32 Position;

	/** Selection start */
	i32 Selection;

	/** Text Vector */
	Vector Text;
} Input;

/** Window structure */
typedef struct
{
	/** Window title (displayed in top bar) */
	char *Title;

	/** Pointer to array of pointers to elements */
	void **Elements;

	/** Number of elements */
	i32 Count;

	/** Currently selected element index */
	i32 Selected;

	/** Window OnKey event */
	void (*OnKey)(u32, Key, i32);
} Window;

/** Create a label element */
#define LABEL_CREATE(_X, _Y, _FLAGS, _TEXT) \
	{ \
		.Type = ELEMENT_TYPE_LABEL, \
		.X = _X, \
		.Y = _Y, \
		.Flags = _FLAGS, \
		.Text = _TEXT, \
	}

/** Create a button element */
#define BUTTON_CREATE(_X, _Y, _W, _H, _TEXT, _CLICK) \
	{ \
		.Type = ELEMENT_TYPE_BUTTON, \
		.X = _X, \
		.Y = _Y, \
		.W = _W, \
		.H = _H, \
		.Text = _TEXT, \
		.Click = _CLICK \
	}

/** Create a window */
#define WINDOW_CREATE(_TITLE, _ELEMENTS, _COUNT, _SELECTED, _ONKEY) \
	{ \
		.Title = _TITLE, \
		.Elements = _ELEMENTS, \
		.Count = _COUNT, \
		.Selected = _SELECTED, \
		.OnKey = _ONKEY \
	}

/**
 * @brief Open and render a window
 *
 * @param window Pointer to Window structure
 */
void window_open(Window *window);

/**
 * @brief Forward a key event to the currently open window
 *
 * @param key Key Code
 * @param state Key State
 */
void window_event_key(Key key, i32 chr, KeyState state);

/**
 * @brief Initialize an input field
 *
 * @param input The input field
 * @param x X-Coordinate
 * @param y Y-Coordinate
 * @param w Width
 */
void input_init(Input *input, i32 x, i32 y, i32 w);

/**
 * @brief Clear an input field
 *
 * @param i The input field
 */
void input_clear(Input *i);

#endif /* __GUI_H__ */
