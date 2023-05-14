#include <clipboard.h>
#include <string.h>

static i32 _clipboard_len;
static ClipboardType _clipboard_type;

static char *get_memory(i32 space)
{
	if (space >= 0)
	{
		_clipboard_len = space;
	}
	static char arr[1024];
	return arr;
}

void clipboard_save_text(const char *text)
{
	_clipboard_type = CLIPBOARD_TYPE_TEXT;
	char *clipboard = get_memory(strlen(text));
	strcpy(clipboard, text);
}

void clipboard_save_text_len(const char *text, i32 len)
{
	_clipboard_type = CLIPBOARD_TYPE_TEXT;
	char *clipboard = get_memory(len);
	strncpy(clipboard, text, len);
}

const void *clipboard_get(void)
{
	return get_memory(-1);
}

i32 clipboard_len(void)
{
	return _clipboard_len;
}

ClipboardType clipboard_type(void)
{
	return _clipboard_type;
}