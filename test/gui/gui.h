#ifndef __GUI_H__
#define __GUI_H__


#include "types.h"

typedef struct LABEL
{
	u32 X, Y;
	char *Text;
} Label;

typedef struct BUTTON
{
	u32 X, Y, W, H;
	char *Text;
	void (*Click)(void);
} Button;

typedef struct INPUT
{
	u32 X, Y, W;
	u32 Position, Length, Size;
	char *Text;
} Input;

typedef struct ELEMENT
{
	u32 Type;
	union
	{
		Label *L;
		Input *I;
		Button *B;
	} E;
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

static void window_open(Window *window);
static void window_render(Window *f);
static void element_render_sel(Element *e, u32 sel);
static void element_render(Element *e);
static void label_render(Label *l);
static void button_render(Button *b, u32 sel);
static void input_render(Input *i, u32 sel);
static void window_event_button(u32 button, u32 action);
static void window_event_key(u32 key, u32 ascii);

typedef enum {
	ELEMENT_TYPE_BUTTON,
	ELEMENT_TYPE_INPUT,
	ELEMENT_TYPE_LABEL
} ElementType;

#endif