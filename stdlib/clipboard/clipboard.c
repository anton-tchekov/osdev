/**
 * @file    clipboard.c
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    14.05.2023
 */

#include <clipboard.h>
#include <string.h>
#include <vector.h>

/** Vector that contains the textual data content */
static Vector _text_content;

/** Already initialized */
static bool _init;

/**
 * @brief Initialize clipboard internals
 */
static void _clipboard_init(void)
{
	if(!_init)
	{
		_init = true;
		vector_init(&_text_content, 1, 16);
	}
}

void clipboard_save_text_len(const char *text, i32 len)
{
	_clipboard_init();
	vector_set(&_text_content, text, len);
}

const char *clipboard_get_text(void)
{
	_clipboard_init();
	return vector_data(&_text_content);
}

i32 clipboard_get_text_len(void)
{
	_clipboard_init();
	return vector_len(&_text_content);
}
