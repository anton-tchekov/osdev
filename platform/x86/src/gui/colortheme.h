#ifndef __COLORTHEME_H__
#define __COLORTHEME_H__

#include "types.h"

typedef struct ColorTheme {
	u32 primary;
	u32 secondary;
	u32 background;
	u32 text;
} ColorTheme;

ColorTheme nordPalette = {
	.primary = 0xFF40342E,
	.secondary = 0xFF52423B,
	.background = 0xFF5E4C43,
	.text = 0xFFF4EFEC};

#endif /* __COLORTHEME_H__ */