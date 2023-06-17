/* TEMP FILE CONTAINS CODE THAT IS NO LONGER USED BUT MIGHT GET USEFUL IN THE FUTURE*/

/* --- optimization of string.h by forwarding to kernel --- */

SYSCALL_MEMCPY,
SYSCALL_MEMMOVE,
SYSCALL_MEMCMP,
SYSCALL_MEMCHR,
SYSCALL_MEMSET,

SYSCALL_STRCPY,
SYSCALL_STRNCPY,
SYSCALL_STRLEN,
SYSCALL_STRCMP,
SYSCALL_STRNCMP,
SYSCALL_STRCHR,

#include <syscall.h>

char *strcpy(char *dest, const char *src)
{
	return (char *)syscall2(SYSCALL_STRCPY, (u32)dest, (u32)src);
}

char *strncpy(char *dest, const char *src, u32 count)
{
	return (char *)syscall3(SYSCALL_STRNCPY, (u32)dest, (u32)src, count);
}

u32 strlen(const char *str)
{
	return syscall1(SYSCALL_STRLEN, (u32)str);
}

i32 strcmp(const char *str1, const char *str2)
{
	return syscall2(SYSCALL_STRCMP, (u32)str1, (u32)str2);
}

i32 strncmp(const char *str1, const char *str2, u32 count)
{
	return syscall3(SYSCALL_STRNCMP, (u32)str1, (u32)str2, count);
}

const char *strchr(const char *str, i32 c)
{
	return (const char *)syscall2(SYSCALL_STRCHR, (u32)str, c);
}

void *memcpy(void *dest, const void *src, u32 count)
{
	return (void *)syscall3(SYSCALL_MEMCPY, (u32)dest, (u32)src, count);
}

void *memmove(void *dest, const void *src, u32 count)
{
	return (void *)syscall3(SYSCALL_MEMMOVE, (u32)dest, (u32)src, count);
}

i32 memcmp(const void *ptr1, const void *ptr2, u32 count)
{
	return syscall3(SYSCALL_MEMCMP, (u32)ptr1, (u32)ptr2, count);
}

const void *memchr(const void *ptr, i32 value, u32 count)
{
	return (void *)syscall3(SYSCALL_MEMCHR, (u32)ptr, (u32)value, count);
}

void *memset(void *ptr, i32 value, u32 count)
{
	return (void *)syscall3(SYSCALL_MEMSET, (u32)ptr, (u32)value, count);
}

syscall_memcpy,
syscall_memmove,
syscall_memcmp,
syscall_memchr,
syscall_memset,

syscall_strcpy,
syscall_strncpy,
syscall_strlen,
syscall_strcmp,
syscall_strncmp,
syscall_strchr,

u32 syscall_memcpy(u32 *args);
u32 syscall_memmove(u32 *args);
u32 syscall_memcmp(u32 *args);
u32 syscall_memchr(u32 *args);
u32 syscall_memset(u32 *args);

u32 syscall_strcpy(u32 *args);
u32 syscall_strncpy(u32 *args);
u32 syscall_strlen(u32 *args);
u32 syscall_strcmp(u32 *args);
u32 syscall_strncmp(u32 *args);
u32 syscall_strchr(u32 *args);

u32 syscall_memcpy(u32 *args)
{
	return (u8 *)memcpy(_memory + args[0],
		_memory + args[1], args[2]) - _memory;
}

u32 syscall_memmove(u32 *args)
{
	return (u8 *)memmove(_memory + args[0],
		_memory + args[1], args[2]) - _memory;
}

u32 syscall_memcmp(u32 *args)
{
	return memcmp(_memory + args[0], _memory + args[1], args[2]);
}

u32 syscall_memchr(u32 *args)
{
	return (u8 *)memchr(_memory + args[0], args[1], args[2]) - _memory;
}

u32 syscall_memset(u32 *args)
{
	return (u8 *)memset(_memory + args[0], args[1], args[2]) - _memory;
}

u32 syscall_strcpy(u32 *args)
{
	return strcpy((char *)(_memory + args[0]),
		(char *)(_memory + args[1])) - (char *)_memory;
}

u32 syscall_strncpy(u32 *args)
{
	return strncpy((char *)(_memory + args[0]),
		(char *)(_memory + args[1]), args[2]) - (char *)_memory;
}

u32 syscall_strlen(u32 *args)
{
	return strlen((char *)(_memory + args[0]));
}

u32 syscall_strcmp(u32 *args)
{
	return strcmp((char *)(_memory + args[0]), (char *)(_memory + args[1]));
}

u32 syscall_strncmp(u32 *args)
{
	return strncmp((char *)(_memory + args[0]),
		(char *)(_memory + args[1]), args[2]);
}

u32 syscall_strchr(u32 *args)
{
	return strchr((char *)(_memory + args[0]), args[1]) - (char *)_memory;
}





	syscall_gfx_image_rgba,
	syscall_gfx_image_rgb,

/**
 * @brief System call to draw an RGBA image
 *
 * @param args args[0]: X-Coordinate
 *             args[1]: Y-Coordinate
 *             args[2]: Width
 *             args[3]: Height
 *             args[4]: Pointer to RGBA image
 * @return Non-zero on error
 */
static u8 syscall_gfx_image_rgba(u32 *args)
{
	u16 x, y, w, h;
	u32 image, bytes;

	x = args[0];
	y = args[1];
	w = args[2];
	h = args[3];
	if(_gfx_check_bounds(x, y, w, h))
	{
		return 1;
	}

	image = args[4];
	bytes = (u32)4 * (u32)w * (u32)h;
	if(_memory_check_bounds(image, bytes))
	{
		return 1;
	}

	env_gfx_image_rgba(x, y, w, h, image);
	return 0;
}

/**
 * @brief System call to draw an RGB image
 *
 * @param args args[0]: X-Coordinate
 *             args[1]: Y-Coordinate
 *             args[2]: Width
 *             args[3]: Height
 *             args[4]: Pointer to RGB image
 * @return Non-zero on error
 */
static u8 syscall_gfx_image_rgb(u32 *args)
{
	u16 x, y, w, h;
	u32 image, bytes;

	x = args[0];
	y = args[1];
	w = args[2];
	h = args[3];
	if(_gfx_check_bounds(x, y, w, h))
	{
		return 1;
	}

	image = args[4];
	bytes = (u32)3 * (u32)w * (u32)h;
	if(_memory_check_bounds(image, bytes))
	{
		return 1;
	}

	env_gfx_image_rgb(x, y, w, h, image);
	return 0;
}


/**
 * @brief Draw an RGBA image
 *
 * @param x X-Coordinate
 * @param y Y-Coordinate
 * @param w Width
 * @param h Height
 * @param addr Image memory address
 */
void env_gfx_image_rgba(u16 x, u16 y, u16 w, u16 h, u32 addr);

/**
 * @brief Draw an RGB image
 *
 * @param x X-Coordinate
 * @param y Y-Coordinate
 * @param w Width
 * @param h Height
 * @param addr Image memory address
 */
void env_gfx_image_rgb(u16 x, u16 y, u16 w, u16 h, u32 addr);




void env_gfx_image_rgba(u16 x, u16 y, u16 w, u16 h, u32 addr)
{
	lcd_image_rgba(x, y, w, h, addr);
}

void env_gfx_image_rgb(u16 x, u16 y, u16 w, u16 h, u32 addr)
{
	lcd_image_rgb(x, y, w, h, addr);
}


void env_gfx_image_rgba(u16 x, u16 y, u16 w, u16 h, u32 addr)
{
	i32 x0, y0;
	u32 *image;

	image = (u32 *)(_memory + addr);
	for(y0 = y; y0 < y + h; ++y0)
	{
		for(x0 = x; x0 < x + w; ++x0)
		{
			_pixels[y0 * GFX_WIDTH + x0] = _abgr_to_argb(*image++);
		}
	}
}

void env_gfx_image_rgb(u16 x, u16 y, u16 w, u16 h, u32 addr)
{
	u8 r, g, b;
	i32 x0, y0;
	u8 *image;

	image = (u8 *)(_memory + addr);
	for(y0 = y; y0 < y + h; ++y0)
	{
		for(x0 = x; x0 < x + w; ++x0)
		{
			r = *image++;
			g = *image++;
			b = *image++;
			_pixels[y0 * GFX_WIDTH + x0] = gfx_color(r, g, b);
		}
	}
}

void gfx_image_rgba(i32 x, i32 y, i32 w, i32 h, const u8 *data)
{
	syscall5(SYSCALL_GFX_IMAGE_RGBA, x, y, w, h, (u32)data);
}

void gfx_image_rgb(i32 x, i32 y, i32 w, i32 h, const u8 *data)
{
	syscall5(SYSCALL_GFX_IMAGE_RGB, x, y, w, h, (u32)data);
}

/**
 * @brief Draws an image in RGBA format
 *
 * @param x X-Coordinate (Left)
 * @param y Y-Coordinate (Top)
 * @param w Width
 * @param h Height
 * @param data Pixel data (32 bits per pixel)
 */
void gfx_image_rgba(i32 x, i32 y, i32 w, i32 h, const u8 *data);

/**
 * @brief Draws an image in RGB format
 *
 * @param x X-Coordinate (Left)
 * @param y Y-Coordinate (Top)
 * @param w Width
 * @param h Height
 * @param data Pixel data (24 bits per pixel)
 */
void gfx_image_rgb(i32 x, i32 y, i32 w, i32 h, const u8 *data);



#ifdef TEST

#include <stdio.h>

void print_chain(void)
{
	i32 i = 0;
	ChunkHeader *cur;
	printf("\n| Block | Adr Dec  | Size Dec |\n");
	for (cur = _get_next(_first.Next), i = 1;
		 cur; cur = _get_next(cur->Next), i++)
	{
		u32 offset = (ptr)cur - (ptr)_heap_start;
		printf("| #%04d | %8d | %8d |\n",
			i, offset, cur->Size);
	}
}

void print_stats(void)
{
	f32 percent_used = (f32)_used / (f32)_size * 100.0f;
	i32 free = _size - _total;
	f32 percent_free = (f32)free / (f32)_size * 100.0f;
	f32 percent_total = (f32)_total / (f32)_size * 100.0f;
	i32 wasted = _total - _used;
	f32 percent_wasted = (f32)wasted / (f32)_total * 100.0f;
	f32 wu_ratio = (f32)_used / (f32)wasted;
	printf(
		"header size                  = %d bytes\n"
		"heap size                    = %6d (%6.2f%%)\n"
		"total stored (used + header) = %6d (%6.2f%%)\n"
		"data stored (used)           = %6d (%6.2f%%)\n"
		"wasted (header)              = %6d (%6.2f%%)\n"
		"free                         = %6d (%6.2f%%)\n"
		"used to waste factor         = 1 : %3.2f\n",
		(u32)sizeof(ChunkHeader),
		_size, 100.0f,
		_total, percent_total,
		_used, percent_used,
		wasted, percent_wasted,
		free, percent_free,
		wu_ratio);
}

#endif /* TEST */


/**
 * @brief Get a textual representation of a key state
 *
 * @param state Key state (see enum above)
 * @return String
 */
const char *keystate_str(KeyState state);



#ifdef ENABLE_UNICODE

/** Uniocde codepoint for section sign (paragraph) */
#define UNICODE_SECTION_SIGN               0xA7

/** Uniocde codepoint for degree sign */
#define UNICODE_DEGREE_SIGN                0xB0

/** Uniocde codepoint for superscript two */
#define UNICODE_SUPERSCRIPT_TWO            0xB2

/** Uniocde codepoint for superscript three */
#define UNICODE_SUPERSCRIPT_THREE          0xB3

/** Uniocde codepoint for euro sign */
#define UNICODE_EURO_SIGN                  0x20AC

/** Uniocde codepoint for german sharp S */
#define UNICODE_SMALL_SHARP_S              0xDF

/** Uniocde codepoint for german capital A-Umlaut */
#define UNICODE_CAPITAL_A_UMLAUT           0xE4

/** Uniocde codepoint for german small A-Umlaut */
#define UNICODE_SMALL_A_UMLAUT             0xC4

/** Uniocde codepoint for german capital O-Umlaut */
#define UNICODE_CAPITAL_O_UMLAUT           0xD6

/** Uniocde codepoint for german small O-Umlaut */
#define UNICODE_SMALL_O_UMLAUT             0xF6

/** Uniocde codepoint for german capital U-Umlaut */
#define UNICODE_CAPITAL_U_UMLAUT           0xFC

/** Uniocde codepoint for german small U-Umlaut */
#define UNICODE_SMALL_U_UMLAUT             0xDC

#endif /* ENABLE_UNICODE */


#ifdef ENABLE_UNICODE
	else if(k == KEY_MINUS)                           { return UNICODE_SMALL_SHARP_S; }
	else if(k == (KEY_GRAVE | MOD_SHIFT))             { return UNICODE_DEGREE_SIGN; }
	else if(k == (KEY_APOSTROPHE | MOD_SHIFT))        { return UNICODE_CAPITAL_A_UMLAUT; }
	else if(k == KEY_APOSTROPHE)                      { return UNICODE_SMALL_A_UMLAUT; }
	else if(k == (KEY_SEMICOLON | MOD_SHIFT))         { return UNICODE_CAPITAL_O_UMLAUT; }
	else if(k == KEY_SEMICOLON)                       { return UNICODE_SMALL_O_UMLAUT; }
	else if(k == (KEY_L_BRACKET | MOD_SHIFT))         { return UNICODE_CAPITAL_U_UMLAUT; }
	else if(k == KEY_L_BRACKET)                       { return UNICODE_SMALL_U_UMLAUT; }
#endif /* ENABLE_UNICODE */

#ifdef ENABLE_UNICODE
			else if(c == 'e') { return UNICODE_EURO_SIGN; }
#endif /* ENABLE_UNICODE */


#ifdef ENABLE_UNICODE
		static const i32 numbers_shift[] PROGMEM = { '!', '\"', UNICODE_SECTION_SIGN,
			'$', '%', '&', '/', '(', ')', '=' };

		static const i32 numbers_altgr[] PROGMEM = { 0, UNICODE_SUPERSCRIPT_TWO,
			UNICODE_SUPERSCRIPT_THREE, 0, 0, 0, '{', '[', ']', '}' };
#else /* ENABLE_UNICODE */

#endif /* ENABLE_UNICODE */

/** Enable Unicode support */
/* #define ENABLE_UNICODE */

static char *reverse(char *s)
{
	i32 i, j, tmp;
	for(i = 0, j = strlen(s) - 1; i < j; ++i, --j)
	{
		tmp = s[i];
		s[i] = s[j];
		s[j] = tmp;
	}

	return s;
}

i32 is_prime(i32 v)
{
	i32 i;
	for(i = 2; i <= v / 2; ++i)
	{
		if(v % i == 0)
		{
			return 0;
		}
	}

	return 1;
}


	u32 begin, end;
	i32 i, count;

	debug_print("Start!\n");

	begin = millis();
	debug_print("begin = %d\n", 0);

	count = 0;
	for(i = 2; i <= 10000; ++i)
	{
		debug_print("i = %d\n", i);
		if(is_prime(i))
		{
			++count;
			debug_print("prime! count = %d\n", count);
		}
	}

	end = millis();
	debug_print("Number of primes: %d - Time spent: %d ms\n",
		count, end - begin);


	reverse(str);





#include <types.h>
#include <vector.h>

typedef struct
{
	u32 ColorComment, ColorNumber, ColorString, ColorKeyword;
} SyntaxHighlighter;

typedef struct
{
	char *Screen;
	Vector Lines;

	i32 CursorX, CursorY;
	i32 PageW, PageH, PageX, PageY;
	i32 SelectionX, SelectionY;
	i32 LineNumberDigits;
	i32 TabSize;
} Editor;

typedef struct
{
	char *Screen;
	Vector Text;
	i32 Cursor, Selection;
	i32 LineNumberDigits, TabSize;
	i32 PageW, PageH;
} Editor;

void editor_init(Editor *ed);
void editor_render(Editor *ed);

void editor_char(Editor *ed, char c);
void editor_backspace(Editor *ed);
void editor_delete(Editor *ed);
void editor_new_line(Editor *ed);
void editor_home(Editor *ed);
void editor_end(Editor *ed);
void editor_page_up(Editor *ed);
void editor_page_down(Editor *ed);
void editor_left(Editor *ed);
void editor_right(Editor *ed);
void editor_up(Editor *ed);
void editor_down(Editor *ed);

#include "editor.h"
#include <std.h>

void editor_init(Editor *ed)
{
	ed->TabSize = 4;
	ed->LineNumberDigits = 4;
	ed->Cursor = 0;

	vector_init(&ed->Text, sizeof(char), 128);

	ed->Screen = memalloc((ed->PageW + 1 + ed->LineNumberDigits) * ed->PageH);
}

void editor_render(Editor *ed)
{
	i32 x, y;
	i32 line;
	char c;
	const char *s, *text;

	line = 1;
	x = 0;
	y = 0;

	text = vector_data(&ed->Text);

	for(s = text; (c = *s); ++s)
	{
		if(c == '\n')
		{
			++line;
			++y;
			x = 0;

			/* Print line number */
			debug_print("\n%4d | ", line);
		}
		else
		{
			debug_print("%c", c);
		}
	}
}


void editor_char(Editor *ed, char c)
{

}

void editor_backspace(Editor *ed)
{

}

void editor_delete(Editor *ed)
{

}

void editor_new_line(Editor *ed)
{

}

void editor_home(Editor *ed)
{

}

void editor_end(Editor *ed)
{

}

void editor_page_up(Editor *ed)
{

}

void editor_page_down(Editor *ed)
{

}

void editor_left(Editor *ed)
{

}

void editor_right(Editor *ed)
{

}

void editor_up(Editor *ed)
{

}

void editor_down(Editor *ed)
{

}

/* --- PUBLIC --- */
void editor_init(Editor *ed)
{
	ed->TabSize = 4;
	ed->LineNumberDigits = 4;

	vector_init(&ed->Lines, sizeof(Vector), 128);
	vector_push(&ed->Lines, ector_get(&ed->Lines, 0), sizeof(char), 8);

	ed->CursorX = 0;
	ed->CursorY = 0;

	ed->PageW = 30;
	ed->PageH = 50;
	ed->PageY = 0;
	ed->PageX = 0;

	ed->Screen = memalloc((ed->PageW + 1 + ed->LineNumberDigits) * ed->PageH);
}


void editor_render(Editor *ed)
{
	/* keywords blue, strings/chars red, comments green, brackets yellow,
	numbers light green */
	i32 x, y, w, h;
	w = ed->PageW;
	h = ed->PageH;
	for(y = 0; y < h; ++y)
	{
		for(x = 0; x < w; ++x)
		{
			/* only update the parts that have changed */

			editor_render_char(x, y, bg_color, fg_color);
		}
	}

	/* draw cursor */
	editor_render_cursor();
}

/* --- EDITING --- */
void editor_char(Editor *ed, char c)
{
	vector_insert(vector_get(&ed->Lines, ed->CursorY), ed->CursorX, &c);
}

void editor_backspace(Editor *ed)
{
	Vector *line = vector_get(&ed->Lines, ed->CursorY);
	if(ed->CursorX == 0)
	{
		if(ed->CursorY > 0)
		{
			/* merge with previous line */
			Vector *prev = vector_get(&ed->Lines, --ed->CursorY);
			ed->CursorX = vector_len(prev);
			vector_push_range(prev, vector_len(line), line->Data);
			vector_destroy(line);
			vector_remove(&ed->Lines, ed->CursorY + 1);
		}
	}
	else
	{
		/* delete prev char */
		vector_remove(line, --ed->CursorX);
	}
}

void editor_delete(Editor *ed)
{
	Vector *line = vector_get(&ed->Lines, ed->CursorY);
	i32 line_len = (i32)vector_len(line);
	if(ed->CursorX >= line_len)
	{
		i32 num_lines = (i32)vector_len(&ed->Lines);
		if(ed->CursorY < num_lines)
		{
			i32 next_idx = ed->CursorY + 1;
			Vector *next = vector_get(&ed->Lines, next_idx);

			/* merge with next line */
			vector_push_range(line, vector_len(next), next->Data);
			vector_destroy(next);
			vector_remove(&ed->Lines, next_idx);
		}
	}
	else
	{
		/* delete next char */
		vector_remove(line, ed->CursorX);
	}
}

void editor_new_line(Editor *ed)
{
	Vector *cur = vector_get(&ed->Lines, ed->CursorY);
	char *str = (char *)cur->Data + ed->CursorX;
	i32 len = vector_len(cur) - ed->CursorX;
	Vector new;

	/* Copy characters after cursor on current line to new line */
	vector_init(&new, sizeof(char), len);
	vector_push_range(&new, len, str);

	/* Insert new line */
	vector_insert(&ed->Lines, ed->CursorY + 1, &new);

	/* Remove characters after cursor on current line */
	vector_remove_range(cur, ed->CursorX, len);

	++ed->CursorY;
	ed->CursorX = 0;
}

/* --- CURSOR MOVEMENT --- */
void editor_home(Editor *ed)
{
	i32 i = 0;
	if(ed->CursorX == 0)
	{
		Vector *line = vector_get(&ed->Lines, ed->CursorY);
		char *buf = line->Data;
		while(isspace(buf[i]))
		{
			++i;
		}
	}

	ed->CursorX = i;
}

void editor_end(Editor *ed)
{
	ed->CursorX = vector_len(vector_get(&ed->Lines, ed->CursorY));
}

void editor_page_up(Editor *ed)
{
	ed->CursorY -= ed->PageH;
	if(ed->CursorY < 0)
	{
		ed->CursorY = 0;
		ed->CursorX = 0;
	}
}

void editor_page_down(Editor *ed)
{
	i32 num_lines = (i32)vector_len(&ed->Lines);
	ed->CursorY += ed->PageY;
	if(ed->CursorY >= num_lines)
	{
		ed->CursorY = num_lines - 1;
		ed->CursorX = vector_len(vector_get(&ed->Lines, ed->CursorY));
	}
}

void editor_left(Editor *ed)
{
	if(ed->CursorX == 0)
	{
		ed->CursorX = vector_len(vector_get(&ed->Lines, --ed->CursorY));
	}
	else
	{
		--ed->CursorX;
	}
}

void editor_right(Editor *ed)
{
	if(ed->CursorX == (i32)vector_len(vector_get(&ed->Lines, ed->CursorY)))
	{
		++ed->CursorY;
		ed->CursorX = 0;
	}
	else
	{
		++ed->CursorX;
	}
}

void editor_up(Editor *ed)
{
	if(ed->CursorY == 0)
	{
		ed->CursorX = 0;
	}
	else
	{
		--ed->CursorY;
		if(ed->CursorY < ed->PageY)
		{
			ed->PageY = ed->CursorY;
		}
	}
}

void editor_down(Editor *ed)
{
	i32 max = (i32)vector_len(&ed->Lines) - 1;
	if(ed->CursorY >= max)
	{
		ed->CursorX = (i32)vector_len(vector_get(&ed->Lines, max));
	}
	else
	{
		++ed->CursorY;
		if(ed->CursorY >= ed->PageY + ed->PageH)
		{
			ed->PageY = ed->CursorY - ed->PageH;
		}
	}
}

static void _render_char(i32 x, i32 y, char c, Color fg, Color bg)
{

}

static void _render_line(Line *line, i32 line_number)
{
	/* syntax highlighting */
	static const char *_keywords[] =
	{
		"var",
		"return",
		"continue",
		"break",
		"if",
		"elif",
		"else",
		"for",
		"to",
		"loop",
		"do",
		"while",
		"switch",
		"case",
		"default"
	};

	char *p;
	i32 i, len;

	p = line->Buffer;
	len = line->Length;
	for(i = 0; i < len; ++i)
	{
		c = p[i];
		if(c == '/')
		{
			if(i + i < len && p[i + 1] == '/')
			{
				for(; i < len; ++i)
				{
					c = p[i];
					_render_char(x, y, c, COLOR_GREEN, COLOR_BLACK);
				}
			}
		}
		else if(c == '\"')
		{
			/* string start */
			while(c != '\"')
			{
				/* skip escape sequence */
				if(c == '\\')
				{
					++i;
				}

				_render_char(x, y, c, COLOR_RED, COLOR_BLACK);
			}
		}
		else if(c == '{' || c == '}' ||
			c == '[' || c == ']' ||
			c == '(' || c == ')')
		{
			_render_char(x, y, c, COLOR_YELLOW, COLOR_BLACK);
		}
		else if(is_identifer_start_char(c))
		{
			i32 id_start;

			id_start = i;
			while(is_identifer_char(c))
			{
			}

			if(is_identifier())
			{

			}
		}
	}
}


