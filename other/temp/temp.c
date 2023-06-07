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

