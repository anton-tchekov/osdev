/**
 * @file    gui.h
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    23.04.2023
 * @brief   Graphical User Interface Toolkit
 */

#ifndef __GUI_H__
#define __GUI_H__

#include <types.h>
#include <keyboard.h>

/** TODO */
typedef enum
{
	ELEMENT_TYPE_BUTTON,
	ELEMENT_TYPE_INPUT,
	ELEMENT_TYPE_LABEL
} ElementType;

/** TODO */
typedef struct LABEL
{
	/** TODO */
	ElementType Type;

	/** TODO */
	i32 X;

	/** TODO */
	i32 Y;

	/** TODO */
	char *Text;
} Label;

/** TODO */
typedef struct BUTTON
{
	/** TODO */
	ElementType Type;

	/** TODO */
	i32 X;

	/** TODO */
	i32 Y;

	/** TODO */
	i32 W;

	/** TODO */
	i32 H;

	/** TODO */
	char *Text;

	/** TODO */
	void (*Click)(void);
} Button;

/** TODO */
typedef struct INPUT
{
	/** TODO */
	ElementType Type;

	/** TODO */
	i32 X;

	/** TODO */
	i32 Y;

	/** TODO */
	i32 W;

	/** TODO */
	i32 Position;

	/** TODO */
	i32 Length;

	/** TODO */
	i32 Size;

	/** TODO */
	char *Text;
} Input;

/** TODO */
typedef struct
{
	/** TODO */
	char *Title;

	/** TODO */
	void **Elements;

	/** TODO */
	i32 Selected;

	/** TODO */
	i32 Count;

	/** TODO */
	void (*OnKey)(u32, u32);
} Window;

/** TODO */
#define LABEL_CREATE(_X, _Y, _TEXT) \
	{ \
		.Type = ELEMENT_TYPE_LABEL, \
		.X = _X, \
		.Y = _Y, \
		.Text = _TEXT, \
	}

/** TODO */
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

/** TODO */
#define INPUT_CREATE(_X, _Y, _W, _TEXT, _SIZE, _CLICK) \
	{ \
		.Type = ELEMENT_TYPE_INPUT, \
		.X = _X, \
		.Y = _Y, \
		.W = _W, \
		.Position = 0, \
		.Length = 0, \
		.Size = _SIZE, \
		.Text = _TEXT, \
		.Click = _CLICK \
	}

/**
 * @brief TODO
 *
 * @param window TODO
 */
void window_open(Window *window);

/**
 * @brief TODO
 *
 * @param window TODO
 */
void window_render(Window *window);

/**
 * @brief TODO
 *
 * @param key TODO
 * @param up TODO
 */
void window_event_key(Key key, bool up);


/**
 * @brief TODO
 *
 * @param i TODO
 */
void input_clear(Input *i);

#endif /* __GUI_H__ */
