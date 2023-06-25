#ifndef __COLORTHEME_H__
#define __COLORTHEME_H__

#include "types.h"

typedef struct ColorTheme {
	u32 primary;
	u32 secondary;
	u32 background;
	u32 text;
} ColorTheme;

extern ColorTheme nordPalette;

#endif /* __COLORTHEME_H__ */
