#include <font-default.h>

static u8 _char_65[] =
{
	0xFF,
	0xAA,
	0x55
};

static FontChar _chars[] =
{
	{
		.Character = 'A',
		.Advance = 8,
		.Size = { 0, 0 },
		.Bearing = { 0, 0 },
		.Image = _char_65
	}
};

static Font _font_default =
{
	.Characters = _chars,
	.Size = 16,
	.NumCharacters = 1
};

Font *font_default = &_font_default;
