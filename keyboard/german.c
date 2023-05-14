/**
 * @file	german.c
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date	14.05.2023
 * @brief   German keyboard layout conversion
 */

#include <keyboard-shared.h>
#include <ctype.h>

/* This is horrible, but it works */
i32 key_to_codepoint(Key k)
{
	Key nomods = MOD_REMOVE(k);

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
	/* else if(k == KEY_MINUS)                         { return 'ß'; } */
	else if(k == (KEY_EQUALS | MOD_SHIFT))          { return '`'; }
	/* else if(k == (KEY_GRAVE | MOD_SHIFT))           { return '°'; } */
	else if(k == KEY_GRAVE)                         { return '^'; }
	/* else if(k == (KEY_APOSTROPHE | MOD_SHIFT))      { return 'Ä'; } */
	/* else if(k == KEY_APOSTROPHE)                    { return 'ä'; } */
	/* else if(k == (KEY_SEMICOLON | MOD_SHIFT))       { return 'Ö'; } */
	/* else if(k == KEY_SEMICOLON)                     { return 'ö'; } */
	/* else if(k == (KEY_LEFTBRACKET | MOD_SHIFT))     { return 'Ü'; } */
	/* else if(k == KEY_LEFTBRACKET)                   { return 'ü'; } */
	else if(nomods >= KEY_A && nomods <= KEY_Z)
	{
		i32 c = nomods - KEY_A + 'a';

		if(c == 'z') { c = 'y'; }
		else if(c == 'y') { c = 'z'; }

		if(k & MOD_ALT_GR)
		{
			if(c == 'q') { return '@'; }
			/* else if(c == 'e') { return '€'; } */
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

		static const char numbers_shift[] =
			{ '!', '\"', 0, '$', '%', '&', '/', '(', ')', '=' };

		static const i32 numbers_altgr[] =
			{ 0, 0, 0, 0, 0, 0, '{', '[', ']', '}' };

		/*static const i32 numbers_shift[] =
			{ '!', '\"', '§', '$', '%', '&', '/', '(', ')', '=' };

		static const i32 numbers_altgr[] =
			{ 0, '²', '³', 0, 0, 0, '{', '[', ']', '}' };*/

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
