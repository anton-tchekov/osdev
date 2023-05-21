/**
 * @file    german.c
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    14.05.2023
 * @brief   German keyboard layout conversion
 */

#include <keyboard-shared.h>
#include <ctype.h>

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

/* This is horrible, but it works */
i32 key_to_codepoint(Key k)
{
	Key nomods = key_mod_remove(k);

	if(nomods == KEY_TAB)                           { return '\t'; }
	else if(nomods == KEY_BACKSPACE)                { return '\b'; }
	else if(nomods == KEY_RETURN)                   { return '\n'; }
	else if(nomods == KEY_SPACE)                    { return ' '; }
	else if(k == (KEY_COMMA | MOD_SHIFT))           { return ';'; }
	else if(k == (KEY_COMMA))                       { return ','; }
	else if(k == (KEY_PERIOD | MOD_SHIFT))          { return ':'; }
	else if(k == (KEY_PERIOD))                      { return '.'; }
	else if(k == (KEY_SLASH | MOD_SHIFT))           { return '_'; }
	else if(k == (KEY_SLASH))                       { return '-'; }
	else if(k == (KEY_BACKSLASH | MOD_SHIFT))       { return '\''; }
	else if(k == (KEY_BACKSLASH))                   { return '#'; }
	else if(k == (KEY_RIGHTBRACKET | MOD_SHIFT))    { return '*'; }
	else if(k == (KEY_RIGHTBRACKET | MOD_ALT_GR))   { return '~'; }
	else if(k == (KEY_RIGHTBRACKET))                { return '+'; }
	else if(k == (KEY_NONUSBACKSLASH | MOD_SHIFT))  { return '>'; }
	else if(k == (KEY_NONUSBACKSLASH | MOD_ALT_GR)) { return '|'; }
	else if(k == KEY_NONUSBACKSLASH)                { return '<'; }
	else if(k == (KEY_MINUS | MOD_SHIFT))           { return '?'; }
	else if(k == (KEY_MINUS | MOD_ALT_GR))          { return '\\'; }
	else if(k == KEY_MINUS)                         { return UNICODE_SMALL_SHARP_S; }
	else if(k == (KEY_EQUALS | MOD_SHIFT))          { return '`'; }
	else if(k == (KEY_GRAVE | MOD_SHIFT))           { return UNICODE_DEGREE_SIGN; }
	else if(k == KEY_GRAVE)                         { return '^'; }
	else if(k == (KEY_APOSTROPHE | MOD_SHIFT))      { return UNICODE_CAPITAL_A_UMLAUT; }
	else if(k == KEY_APOSTROPHE)                    { return UNICODE_SMALL_A_UMLAUT; }
	else if(k == (KEY_SEMICOLON | MOD_SHIFT))       { return UNICODE_CAPITAL_O_UMLAUT; }
	else if(k == KEY_SEMICOLON)                     { return UNICODE_SMALL_O_UMLAUT; }
	else if(k == (KEY_LEFTBRACKET | MOD_SHIFT))     { return UNICODE_CAPITAL_U_UMLAUT; }
	else if(k == KEY_LEFTBRACKET)                   { return UNICODE_SMALL_U_UMLAUT; }
	else if(nomods >= KEY_A && nomods <= KEY_Z)
	{
		i32 c = nomods - KEY_A + 'a';

		if(c == 'z') { c = 'y'; }
		else if(c == 'y') { c = 'z'; }

		if(k & MOD_ALT_GR)
		{
			if(c == 'q') { return '@'; }
			else if(c == 'e') { return UNICODE_EURO_SIGN; }
		}

		if(k & MOD_SHIFT)
		{
			c = toupper(c);
		}

		return c;
	}
	else if(nomods >= KEY_1 && nomods <= KEY_0)
	{
		static const char *numbers = "1234567890";

		static const i32 numbers_shift[] = { '!', '\"', UNICODE_SECTION_SIGN,
			'$', '%', '&', '/', '(', ')', '=' };

		static const i32 numbers_altgr[] = { 0, UNICODE_SUPERSCRIPT_TWO,
			UNICODE_SUPERSCRIPT_THREE, 0, 0, 0, '{', '[', ']', '}' };

		u32 idx = nomods - KEY_1;

		if(k & MOD_SHIFT)
		{
			return numbers_shift[idx];
		}
		else if(k & MOD_ALT_GR)
		{
			return numbers_altgr[idx];
		}
		else
		{
			return numbers[idx];
		}
	}

	return 0;
}
