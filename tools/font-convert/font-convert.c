/**
 * @file    font-convert.c
 * @author  Anton Tchekov
 * @version 0.1
 * @date    25.05.2023
 * @brief   Bitmap font from TTF generator
 */

#include <stdio.h>
#include <locale.h>
#include <ctype.h>

#include <types.h>
#include <utf8.h>

#include "ft2build.h"
#include FT_FREETYPE_H

/** Font pixel format bit */
#define BIT_PIXEL_FORMAT   0

/** 1 bit per pixel format */
#define FLAG_1BPP          0

/** Grayscale pixel format */
#define FLAG_GRAYSCALE     1

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
	else if(!strcmp(s, "extended_german"))
	{
		s = " !\"#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~äöüÄÖÜß";
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

static char *str_uppercase(char *out, const char *in)
{
	char c;
	while((c = *in++))
	{
		*out++ = toupper(c);
	}

	*out = '\0';
	return out;
}

static bool valid_name(const char *name)
{
	char c;
	c = *name++;
	if(!isalpha(c) && c != '_')
	{
		return false;
	}

	while((c = *name++))
	{
		if(!isalnum(c) && c != '_')
		{
			return false;
		}
	}

	return true;
}

static char *build_path(char *out,
	const char *dir, const char *name, const char *ext)
{
	char *p, c;

	p = out;
	while((c = *dir++))
	{
		*p++ = c;
	}

	if(p - 1 >= out && *(p - 1) != '/')
	{
		*p++ = '/';
	}

	while((c = *name++))
	{
		*p++ = c;
	}

	while((c = *ext++))
	{
		*p++ = c;
	}

	*p = '\0';
	return out;
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

	char name_upper[512], path_c[512], path_h[512];
	FILE *fp_c, *fp_h;
	const char *input_filename, *font_name, *output_dir;
	i32 *codepoints, i, c, x, y, num_chars, font_size, bytes, offset;

	/* Set locale so that non-ascii characters are displayed correctly */
	setlocale(LC_CTYPE, "");

	/* Check correct usage */
	if(argc != 7)
	{
		fprintf(stderr,
			"[ Simple bitmap font generator with unicode support ]\n"
			"- Created by Anton Tchekov\n\n"

			"Usage:\n"
			"./font-convert input-file font-name font-size char-list mode "
			"output-dir\n\n"

			"`input-file` should be a TTF file or any other font format FreeType "
			"supports.\n\n"

			"`font-name` should be a valid C identifier.\n\n"

			"`font-size` is the size of the font in pixels that should be "
			"extracted.\n\n"

			"`char-list` can be a custom string of letters,\n"
			"repetitions are ignored so that one can create a huge font\n"
			"just to print a single message. Alternatively, it is possible\n"
			"to pass `printable_ascii` to get only the ASCII characters or\n"
			"`extended_german` to also get the german umlauts and sz.\n"

			"`mode` determines which pixel format to use, allowed values are "
			"`1bit` and `grayscale`.\n\n"

			"`output-dir` is the path to a directory where the generated "
			"C header and source file should be placed. The output files "
			"will be called `font-name`.c and `font-name`.h and will overwrite "
			"any already existing files with the same name.\n\n");
		return 1;
	}

	/* Initialize FreeType */
	if(FT_Init_FreeType(&library))
	{
		fprintf(stderr, "FreeType initialization failed\n");
		return 1;
	}

	font.Flags = 0;
	if(!strcmp(argv[5], "1bit"))
	{
		font.Flags |= (FLAG_1BPP << BIT_PIXEL_FORMAT);
	}
	else if(!strcmp(argv[5], "grayscale"))
	{
		font.Flags |= (FLAG_GRAYSCALE << BIT_PIXEL_FORMAT);
	}
	else
	{
		fprintf(stderr, "Invalid mode\n");
		return 1;
	}

	input_filename = argv[1];
	font_name = argv[2];
	if(!valid_name(font_name))
	{
		fprintf(stderr, "Invalid font name\n");
		return 1;
	}

	output_dir = argv[6];
	if(!(fp_c = fopen(build_path(path_c, output_dir, font_name, ".c"), "w")))
	{
		fprintf(stderr, "Failed to open output C file\n");
		return 1;
	}

	if(!(fp_h = fopen(build_path(path_h, output_dir, font_name, ".h"), "w")))
	{
		fprintf(stderr, "Failed to open output H file\n");
		return 1;
	}

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
		if((font.Flags >> BIT_PIXEL_FORMAT) & FLAG_GRAYSCALE)
		{
			offset += bytes;
		}
		else
		{
			offset += fc->Size[1] * ((fc->Size[0] + 7) / 8);
		}
	}

	/* Write Output */
	fprintf(fp_c,
		"#include <%s.h>\n"
		"\n", font_name);

	fprintf(fp_c,
		"static FontChar _chars[] =\n"
		"{\n");

	for(i = 0; i < num_chars; ++i)
	{
		fc = &font.Characters[i];
		fprintf(fp_c,
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

	fprintf(fp_c, "};\n"
		"\n");

	fprintf(fp_c, "static u8 _bitmap[] =\n"
		"{\n");

	for(i = 0; i < num_chars; ++i)
	{
		fc = &font.Characters[i];

		for(y = 0; y < fc->Size[1]; ++y)
		{
			fprintf(fp_c, "\t");
			if((font.Flags >> BIT_PIXEL_FORMAT) & FLAG_GRAYSCALE)
			{
				/* grayscale */
				for(x = 0; x < fc->Size[0]; ++x)
				{
					fprintf(fp_c, "0x%02X, ", fc->Bitmap[y * fc->Size[0] + x]);
				}

				fprintf(fp_c, "\n");
			}
			else
			{
				/* 1bit */
				i32 byte = 0, bit = 1;
				for(x = 0; x < fc->Size[0]; ++x)
				{
					if(bit == 0x100)
					{
						fprintf(fp_c, "0x%02X, ", byte);
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
					fprintf(fp_c, "0x%02X, ", byte);
				}

				fprintf(fp_c, "/* ");
				for(x = 0; x < fc->Size[0]; ++x)
				{
					fprintf(fp_c, "%c",
						fc->Bitmap[y * fc->Size[0] + x] > 128 ? '#' : ' ');
				}

				fprintf(fp_c, " */\n");
			}
		}

		fprintf(fp_c, "\n");
	}

	fprintf(fp_c, "};\n"
		"\n");

	fprintf(fp_c,
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

	/* Write header file */
	str_uppercase(name_upper, font_name);
	fprintf(fp_h,
		"#ifndef __%s_H__\n"
		"#define __%s_H__\n"
		"\n"
		"#include \"font.h\"\n"
		"\n"
		"extern Font *%s;\n"
		"\n"
		"#endif /* __%s_H__ */\n",
		name_upper, name_upper, font_name, name_upper);

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
