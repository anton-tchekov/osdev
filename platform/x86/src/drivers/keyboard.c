#include "keyboard.h"
#include "io.h"
#include "isr.h"
#include "keys.h"
#include "layout.h"

static Key _scancode_to_key(u8 scancode)
{
	switch(scancode)
	{
		case 0x01: return KEY_ESCAPE;
		case 0x02: return KEY_1;
		case 0x03: return KEY_2;
		case 0x04: return KEY_3;
		case 0x05: return KEY_4;
		case 0x06: return KEY_5;
		case 0x07: return KEY_6;
		case 0x08: return KEY_7;
		case 0x09: return KEY_8;
		case 0x0A: return KEY_9;
		case 0x0B: return KEY_0;
		case 0x0C: return KEY_MINUS;
		// case 0x0D: return KEY_PLUS;
		case 0x0E: return KEY_BACKSPACE;
		case 0x0F: return KEY_TAB;
		case 0x10: return KEY_Q;
		case 0x11: return KEY_W;
		case 0x12: return KEY_E;
		case 0x13: return KEY_R;
		case 0x14: return KEY_T;
		case 0x15: return KEY_Y;
		case 0x16: return KEY_U;
		case 0x17: return KEY_I;
		case 0x18: return KEY_O;
		case 0x19: return KEY_P;
		case 0x1A: return KEY_L_BRACKET;
		case 0x1B: return KEY_R_BRACKET;
		case 0x1C: return KEY_RETURN;
		case 0x1D: return KEY_L_CTRL;
		case 0x1E: return KEY_A;
		case 0x1F: return KEY_S;
		case 0x20: return KEY_D;
		case 0x21: return KEY_F;
		case 0x22: return KEY_G;
		case 0x23: return KEY_H;
		case 0x24: return KEY_J;
		case 0x25: return KEY_K;
		case 0x26: return KEY_L;
		case 0x27: return KEY_SEMICOLON;
		case 0x28: return KEY_BACKSLASH;
		case 0x29: return KEY_EQUALS;
		case 0x2A: return KEY_L_SHIFT;
		case 0x2B: return KEY_MINUS;
		case 0x2C: return KEY_Z;
		case 0x2D: return KEY_X;
		case 0x2E: return KEY_C;
		case 0x2F: return KEY_V;
		case 0x30: return KEY_B;
		case 0x31: return KEY_N;
		case 0x32: return KEY_M;
		case 0x33: return KEY_COMMA;
		case 0x34: return KEY_PERIOD;
		case 0x36: return KEY_R_SHIFT;
		case 0x37: return KEY_KP_MULTIPLY;
		case 0x38: return KEY_L_ALT;
		case 0x39: return KEY_SPACE;
		case 0x5B: return KEY_L_GUI;
	}

	return KEY_UNKNOWN;
}

/*
#include "font_noto.h"
#include "graphics.h"
#include "framebuffer.h"
#include "sprintf.h"

Framebuffer test;
*/

static void keyboard_callback(registers_t regs)
{
	static Key _mods;

	u8 scancode = inb(0x60);

/*
	char buf[32];
	framebuffer_rect(&test, 0, 0, 100, 40, 0x00000000);
	sprintf(buf, "%d", scancode);
	font_string(&test, 0, 0, buf, font_noto, 0xFFFFFFFF);
	graphics_blit_framebuffer(&test, 20, 20);
*/

	bool released = scancode & 0x80;
	Key key = _scancode_to_key(scancode & 0x7F);
	Key mod = 0;
	i32 codepoint;

	if(key == KEY_L_ALT)
	{
		mod = MOD_ALT;
	}
	else if(key == KEY_R_ALT)
	{
		mod = MOD_ALT_GR;
	}
	else if(key == KEY_L_SHIFT || key == KEY_R_SHIFT)
	{
		mod = MOD_SHIFT;
	}
	else if(key == KEY_L_CTRL || key == KEY_R_CTRL)
	{
		mod = MOD_CTRL;
	}
	else if(key == KEY_L_GUI || key == KEY_R_GUI)
	{
		mod = MOD_OS;
	}

	if(mod)
	{
		if(released)
		{
			_mods &= ~mod;
		}
		else
		{
			_mods |= mod;
		}
	}

	key |= _mods;

	codepoint = key_to_codepoint(key);
	key_event(key, codepoint,
		released ? KEYSTATE_RELEASED : KEYSTATE_PRESSED);

	(void)regs;
}

void keyboard_init(void)
{
	/*framebuffer_init(&test, (u32 *)0x5000000, 100, 40);*/

	isr_register(IRQ1, keyboard_callback);
}
