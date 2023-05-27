/**
 * @file    font-convert.c
 * @author  Anton Tchekov
 * @version 0.1
 * @date    25.05.2023
 * @brief   Bitmap font from TTF generator
 */

#include <stdio.h>
#include <locale.h>
#include <types.h>
#include <utf8.h>
#include "ft2build.h"
#include FT_FREETYPE_H

/** Font character data struct */
typedef struct
{
	/** Unicode codepoint */
	i32 Codepoint;

	/** Number of pixels to advance in x direction after this character */
	i32 Advance;

	/** Size of the character */
	i32 Size[2];

	/** Offset of the character inside size box */
	i32 Bearing[2];

	/** Offset of the character in bytes */
	i32 Offset;

	/** Grayscale character bitmap */
	u8 *Bitmap;
} FontChar;

/** Font data struct */
typedef struct
{
	/** Characters contained in this font */
	FontChar *Characters;

	/** Number of characters */
	u32 NumCharacters;

	/** Size of the font */
	u32 Size;

	/** Grayscale / 1 BPP flag */
	u32 Flags;
} Font;

/**
 * @brief Malloc wrapper that prints an error and exits if the memory
 *        allocation failed
 *
 * @param size Number of bytes to allocate
 * @return Block of memory with the requested size
 */
static void *_malloc(u32 size)
{
	void *p = malloc(size);
	if(!p)
	{
		fprintf(stderr, "Memory allocation failed\n");
		exit(1);
	}

	return p;
}

/* --- UTF-8 and string functions --- */

/**
 * @brief Compare two i32 numbers for qsort
 *
 * @param a First element
 * @param b Second element
 * @return Comparison result (ordering)
 */
static i32 _compare_i32(const void *a, const void *b)
{
	return *(const i32 *)a  - *(const i32 *)b;
}

/**
 * @brief Sort an array of i32 numbers in place and remove duplicates
 *
 * @param elements Pointer to array of numbers
 * @param count Number of elements in the array
 * @return New count after duplocates have been removed
 */
static i32 _sort_unique(i32 *elements, i32 count)
{
	i32 a, b;
	qsort(elements, count, sizeof(*elements), _compare_i32);

	/* Remove duplicates */
	for(a = 1, b = 1; a < count; ++a)
	{
		if(elements[a] == elements[a - 1])
		{
			continue;
		}

		elements[b++] = elements[a];
	}

	return b;
}

/**
 * @brief Get an array of unique unicode codepoints from a UTF-8 string
 *
 * @param s The string
 * @param length Output parameter for number of unique codepoints
 * @return Pointer to heap-allocated unicode codepoint array
 */
static i32 *_unique_codepoints(const char *s, i32 *length)
{
	const char *p;
	i32 *cp, c, count;

	if(!strcmp(s, "printable_ascii"))
	{
		s = " !\"#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
	}

	count = utf8_length(s);
	cp = _malloc(count * sizeof(*cp));
	p = s;
	for(count = 0; ; ++count)
	{
		p = utf8_codepoint(p, &c);
		if(!c)
		{
			break;
		}

		cp[count] = c;
	}

	*length = _sort_unique(cp, count);
	return cp;
}

/**
 * @brief Font converter main function
 *
 * @param argc Command line argument count
 * @param argv Command line arguments array
 * @return Exit code
 */
int main(int argc, char *argv[])
{
	FT_Library library;
	FT_Face face;

	Font font;
	FontChar *fc;

	const char *input_filename, *font_name;
	i32 *codepoints, i, c, x, y, num_chars, font_size, bytes, offset;

	/* Set locale so that non-ascii characters are displayed correctly */
	setlocale(LC_CTYPE, "");

	/* Check correct usage */
	if(argc != 6)
	{
		fprintf(stderr, "Usage: "
			"./font-convert input-file font-name font-size char-list mode\n"
			"Mode: 1bit or grayscale\n");
		return 1;
	}

	/* Initialize FreeType */
	if(FT_Init_FreeType(&library))
	{
		fprintf(stderr, "FreeType initialization failed\n");
		return 1;
	}

	if(!strcmp(argv[5], "1bit"))
	{
		font.Flags = 0;
	}
	else if(!strcmp(argv[5], "grayscale"))
	{
		font.Flags = 1;
	}
	else
	{
		fprintf(stderr, "Invalid mode\n");
		return 1;
	}

	input_filename = argv[1];
	font_name = argv[2];
	if(!(font_size = strtol(argv[3], NULL, 10)))
	{
		fprintf(stderr, "Invalid font size\n");
		return 1;
	}

	codepoints = _unique_codepoints(argv[4], &num_chars);

	/* Load Input */
	/* fprintf(stderr, "Loading font \"%s\"\n", input_filename); */
	if(FT_New_Face(library, input_filename, 0, &face))
	{
		fprintf(stderr, "Failed to load font \"%s\"\n", input_filename);
		return 1;
	}

	font.Size = font_size;
	font.NumCharacters = num_chars;
	font.Characters = _malloc(num_chars * sizeof(FontChar));
	FT_Set_Pixel_Sizes(face, 0, font_size);
	offset = 0;
	for(i = 0; i < num_chars; ++i)
	{
		c = codepoints[i];
		/* fprintf(stderr, "Loading character \'%lc\'\n", c); */
		if(FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			fprintf(stderr, "Failed to load character: \'%lc\'", c);
			return 1;
		}

		fc = &font.Characters[i];
		fc->Codepoint = c;
		fc->Advance = face->glyph->advance.x >> 6;
		fc->Size[0] = face->glyph->bitmap.width;
		fc->Size[1] = face->glyph->bitmap.rows;
		fc->Bearing[0] = face->glyph->bitmap_left;
		fc->Bearing[1] = face->glyph->bitmap_top;
		bytes = fc->Size[0] * fc->Size[1];
		fc->Bitmap = _malloc(bytes);
		fc->Offset = offset;
		memcpy(fc->Bitmap, face->glyph->bitmap.buffer, bytes);
		if(font.Flags & 1)
		{
			offset += bytes;
		}
		else
		{
			offset += fc->Size[1] * ((fc->Size[0] + 7) / 8);
		}
	}

	/* Write Output */
	fprintf(stdout,
		"#include <%s.h>\n"
		"\n", font_name);

	fprintf(stdout,
		"static FontChar _chars[] =\n"
		"{\n");

	for(i = 0; i < num_chars; ++i)
	{
		fc = &font.Characters[i];
		fprintf(stdout,
			"\t{\n"
			"\t\t.Codepoint = %d,\n"
			"\t\t.Advance = %d,\n"
			"\t\t.Size = { %d, %d },\n"
			"\t\t.Bearing = { %d, %d },\n"
			"\t\t.Offset = %d\n"
			"\t}%s\n",
			fc->Codepoint,
			fc->Advance,
			fc->Size[0],
			fc->Size[1],
			fc->Bearing[0],
			fc->Bearing[1],
			fc->Offset,
			i == num_chars - 1 ? "" : ",");
	}

	fprintf(stdout, "};\n"
		"\n");

	fprintf(stdout, "static u8 _bitmap[] =\n"
		"{\n");

	for(i = 0; i < num_chars; ++i)
	{
		fc = &font.Characters[i];

		for(y = 0; y < fc->Size[1]; ++y)
		{
			fprintf(stdout, "\t\t");

			if(font.Flags & 1)
			{
				/* grayscale */
				for(x = 0; x < fc->Size[0]; ++x)
				{
					fprintf(stdout, "0x%02X, ", fc->Bitmap[y * fc->Size[0] + x]);
				}

				fprintf(stdout, "\n");
			}
			else
			{
				/* 1bit */
				i32 byte = 0, bit = 1;
				for(x = 0; x < fc->Size[0]; ++x)
				{
					if(bit == 0x100)
					{
						fprintf(stdout, "0x%02X, ", byte);
						byte = 0;
						bit = 1;
					}

					if(fc->Bitmap[y * fc->Size[0] + x] > 128)
					{
						byte |= bit;
					}

					bit <<= 1;
				}

				if(bit != 1)
				{
					fprintf(stdout, "0x%02X, ", byte);
				}

				fprintf(stdout, "/* ");
				for(x = 0; x < fc->Size[0]; ++x)
				{
					fprintf(stdout, "%c",
						fc->Bitmap[y * fc->Size[0] + x] > 128 ? '#' : ' ');
				}

				fprintf(stdout, " */\n");
			}
		}

		fprintf(stdout, "\n");
	}

	fprintf(stdout, "};\n"
		"\n");

	fprintf(stdout,
		"static Font _%s =\n"
		"{\n"
		"\t.Characters = _chars,\n"
		"\t.Size = %d,\n"
		"\t.NumCharacters = %d,\n"
		"\t.Flags = %d,\n"
		"\t.Bitmap = _bitmap\n"
		"};\n\n"
		"Font *%s = &_%s;\n"
		"\n",
		font_name, font_size, num_chars, font.Flags, font_name, font_name);

	/* Cleanup */
	for(i = 0; i < num_chars; ++i)
	{
		free(font.Characters[i].Bitmap);
	}

	free(font.Characters);
	free(codepoints);
	FT_Done_Face(face);
	FT_Done_FreeType(library);
	return 0;
}
