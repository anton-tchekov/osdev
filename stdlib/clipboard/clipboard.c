/**
 * @file    clipboard.c
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    14.05.2023
 */

#include <clipboard.h>
#include <string.h>

/** Length of the clipboard in bytes */
static i32 _clipboard_len;

/** Type of the current data in the clipboad */
static ClipboardType _clipboard_type;

/**
 * @brief Get memory to place clipboard contents into.
 *
 * @param space Number of bytes needed, -1 to get previous clipboard content
 * @return Pointer to buffer
 */
static char *_get_memory(i32 space)
{
	/**
	 * TODO: Clipboard should be shared across processes.
	 * The following is a hack and just temporary, not to mention a
	 * potential overflow risk.
	 */

	static char arr[1024];
	if(space >= 0)
	{
		_clipboard_len = space;
	}

	return arr;
}

void clipboard_save_text(const char *text)
{
	_clipboard_type = CLIPBOARD_TYPE_TEXT;
	char *clipboard = _get_memory(strlen(text));
	strcpy(clipboard, text);
}

void clipboard_save_text_len(const char *text, i32 len)
{
	_clipboard_type = CLIPBOARD_TYPE_TEXT;
	char *clipboard = _get_memory(len);
	strncpy(clipboard, text, len);
}

const void *clipboard_get(void)
{
	return _get_memory(-1);
}

i32 clipboard_len(void)
{
	return _clipboard_len;
}

ClipboardType clipboard_type(void)
{
	return _clipboard_type;
}
