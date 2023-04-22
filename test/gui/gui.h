#ifndef __GUI_H__
#define __GUI_H__

#include <types.h>
#include <keyboard.h>

typedef struct LABEL
{
	i32 X, Y;
	char *Text;
} Label;

typedef struct BUTTON
{
	i32 X, Y, W, H;
	char *Text;
	void (*Click)(void);
} Button;

typedef struct INPUT
{
	i32 X, Y, W, Position, Length, Size;
	char *Text;
} Input;

typedef enum
{
	ELEMENT_TYPE_BUTTON,
	ELEMENT_TYPE_INPUT,
	ELEMENT_TYPE_LABEL
} ElementType;

typedef struct ELEMENT
{
	ElementType Type;
	void *Element;
} Element;

typedef struct
{
	char *Title;
	Element *Elements;
	i32 Count;
	void (*OnKey)(u32, u32);
} Window;

static Window *current_window;
static i32 current_element;

void window_open(Window *window);
void window_render(Window *window);
void window_event_key(Key key);

#endif /* __GUI_H__ */
