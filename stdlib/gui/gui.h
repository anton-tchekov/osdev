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
#include <gfx.h>

/** Element type */
typedef enum
{
	ELEMENT_TYPE_BUTTON,
	ELEMENT_TYPE_INPUT,
	ELEMENT_TYPE_LABEL
} ElementType;

/** Align bitmask */
#define ALIGN_MASK            0x03

/** Left align (default) */
#define FLAG_ALIGN_LEFT       0x00

/** Center align */
#define FLAG_ALIGN_CENTER     0x01

/** Right align */
#define FLAG_ALIGN_RIGHT      0x02

/** Enable/Disable Line numbers in input field */
#define FLAG_LINE_NUMBERS     0x04

/** Enable/Disable Syntax highlighting in input field */
#define FLAG_SYNTAX_HIGHLIGHT 0x08

/** Display content as stars in input field */
#define FLAG_PASSWORD         0x10

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

	/** Various Flags (see above) */
	u32 Flags;

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

/** Theme struct for GUI appearance */
typedef struct
{
	/** Foreground (text) color */
	Color ColorFG;

	/** Background color */
	Color ColorBG;

	/** Text selection background */
	Color ColorTextSelBG;

	/** Text selection foreground */
	Color ColorTextSelFG;

	/** Element border color */
	Color ColorBorder;

	/** Element border color when selected */
	Color ColorBorderSel;

	/** Element background color */
	Color ElementBG;

	/** Element background color when selected */
	Color ElementSelBG;

	/** Cursor color */
	Color ColorCursor;

	/** Background color of the title bar */
	Color ColorTitleBar;
} Theme;

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
	KeyEvent OnKey;
} Window;

/**
 * @brief Open and render a window
 *
 * @param window Pointer to Window structure
 */
void window_open(Window *window);

/**
 * @brief Forward a key event to the currently open window
 *
 * @param key Key code
 * @param chr Character for key code
 * @param state Key State
 */
void window_event_key(Key key, i32 chr, KeyState state);

/**
 * @brief Initialize a window
 *
 * @param window The window
 * @param title Window title
 * @param elems Pointer to elements array
 * @param count Number of elements
 * @param onkey Key event
 */
void window_init(Window *window, char *title, void *elems, i32 count,
	KeyEvent onkey);

/**
 * @brief Initialize a label
 *
 * @param label The label
 * @param x X-Coordinate
 * @param y Y-Coordinate
 * @param flags Label flags
 * @param text Label text
 */
void label_init(Label *label, i32 x, i32 y, u32 flags, char *text);

/**
 * @brief Initialize a button
 *
 * @param button The button
 * @param x X-Coordinate
 * @param y Y-Coordinate
 * @param w Width
 * @param h Height
 * @param text Button text
 * @param onclick Click event
 */
void button_init(Button *button, i32 x, i32 y, i32 w, i32 h, char *text,
	void (*onclick)(void));

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

/**
 * @brief Render an element by taking its selection status from the
 *        current window
 *
 * @param e Pointer to the Element
 */
void element_render(void *e);

/**
 * @brief Set the current GUI Theme
 *
 * @param theme Pointer to new theme
 */
void theme_set(Theme *theme);

/**
 * @brief Reset the current GUI Theme to default values
 */
void theme_default(void);

#endif /* __GUI_H__ */
