/**
 * @file    gui.h
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    23.04.2023
 * @brief   TODO
 */

#ifndef __GUI_H__
#define __GUI_H__

#include <types.h>
#include <keyboard.h>

typedef enum
{
	ELEMENT_TYPE_BUTTON,
	ELEMENT_TYPE_INPUT,
	ELEMENT_TYPE_LABEL
} ElementType;

typedef struct LABEL
{
	ElementType Type;
	i32 X, Y;
	char *Text;
} Label;

typedef struct BUTTON
{
	ElementType Type;
	i32 X, Y, W, H;
	char *Text;
	void (*Click)(void);
} Button;

typedef struct INPUT
{
	ElementType Type;
	i32 X, Y, W, Position, Length, Size;
	char *Text;
} Input;

typedef struct
{
	char *Title;
	void **Elements;
	i32 Count;
	void (*OnKey)(u32, u32);
} Window;

#define LABEL_CREATE(_X, _Y, _TEXT) \
	{ \
		.Type = ELEMENT_TYPE_LABEL, \
		.X = _X, \
		.Y = _Y, \
		.Text = _TEXT, \
	}

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

void window_open(Window *window);
void window_render(Window *window);
void window_event_key(Key key);

void input_clear(Input *i);

#endif /* __GUI_H__ */