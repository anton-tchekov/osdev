/**
 * @file    platform.c
 * @author  Anton Tchekov
 * @version 0.1
 * @date    25.05.2023
 * @brief   OS simulator platform
 */

#include <emulator.h>
#include <platform.h>
#include <SDL2/SDL.h>
#include <sys/time.h>

#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <stdlib.h>
#include <util.h>
#include <status.h>
#include <keyboard-shared.h>
#include <layout.h>
#include <gfx-types.h>

/* --- CONSTANTS --- */

/** Title of the simulator window */
#define WINDOW_TITLE             "OS Simulator"

/** Number of bytes to read at once when loading binary */
#define READ_SIZE            1024

/* --- VARIABLES --- */

/** Emulated memory */
static u8 _memory[3 * 128 * 1024];

/** Screen pixel array buffer */
static u32 _pixels[GFX_HEIGHT * GFX_WIDTH];

/** Seconds since start */
static u32 _sec_start;

/** Microseconds since start */
static u32 _usec_start;

/** SDL Framebuffer containing pixel buffer */
static SDL_Texture *_framebuffer;

/** SDL Window */
static SDL_Window *_window;

/** SDL Renderer */
static SDL_Renderer *_renderer;

/* --- MEMORY --- */

u32 env_memory_size(void)
{
	return sizeof(_memory);
}

void env_memory_read(u8 bank, u32 addr, void *data, u32 size)
{
	memcpy(data, _memory + bank * 128 * 1024 + addr, size);
}

void env_memory_write(u8 bank, u32 addr, const void *data, u32 size)
{
	memcpy(_memory + bank * 128 * 1024 + addr, data, size);
}

/**
 * @brief Dump emulator memory from address with length
 *
 * @param address Start address
 * @param length Number of bytes
 */
void memory_dump(u32 address, u32 length)
{
	u32 start, end, i;
	start = (address >> 4) << 4;
	end = address + length;
	for(; start < end; start += 16)
	{
		printf("%08X | ", start);
		for(i = 0; i < 16; ++i)
		{
			printf("%02X ", _memory[start + i]);
		}

		printf("| ");
		for(i = 0; i < 16; ++i)
		{
			char c = _memory[start + i];
			printf("%c", (c < 32 || c > 126) ? '.' : c);
		}

		printf("\n");
	}

	printf("\n");
}

/* --- GRAPHICS --- */

/**
 * @brief Initialize SDL graphics
 */
static void gfx_init(void)
{
	/* Init SDL */
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("Error initializing SDL; SDL_Init: %s\n",
			SDL_GetError());
		exit(1);
	}

	/* Create SDL_Window */
	if(!(_window = SDL_CreateWindow(WINDOW_TITLE,
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		GFX_WIDTH, GFX_HEIGHT, 0)))
	{
		printf("Error creating SDL_Window: %s\n",
			SDL_GetError());
		SDL_Quit();
		exit(1);
	}

	/* Create SDL_Renderer */
	if(!(_renderer = SDL_CreateRenderer
		(_window, -1, SDL_RENDERER_ACCELERATED)))
	{
		printf("Error creating SDL_Renderer: %s\n",
			SDL_GetError());
		SDL_DestroyWindow(_window);
		SDL_Quit();
		exit(1);
	}

	SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);

	_framebuffer = SDL_CreateTexture(_renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		GFX_WIDTH, GFX_HEIGHT);
}

/**
 * @brief Destructor for SDL graphics renderer
 */
static void gfx_destroy(void)
{
	SDL_DestroyRenderer(_renderer);
	SDL_DestroyWindow(_window);
	SDL_Quit();
}

/**
 * @brief Generate ABGR color from red, green and blue channel values
 *
 * @param r Red channel value (0-255)
 * @param g Green channel value (0-255)
 * @param b Blue channel value (0-255)
 * @return ABGR color value
 */
static u32 gfx_color(u8 r, u8 g, u8 b)
{
	return ((u32)0xFF << 24) |
		((u32)r << 16) |
		((u32)g << 8) |
		((u32)b);
}

/**
 * @brief Convert a ABGR color to a ARGB color
 *
 * @param color ABGR color value
 * @return ARGB color value
 */
static u32 _abgr_to_argb(u32 color)
{
	return gfx_color(
		_abgr_r(color),
		_abgr_g(color),
		_abgr_b(color));
}

/**
 * @brief Mix two colors according to a ratio. A ratio of 255 means 100% of
 *        the first color and 0% of the second color will be mixed together.
 *
 * @param color1 First ABGR color
 * @param color2 Second ABGR color
 * @param ratio Merge ratio from 0-255
 * @return Merged ARGB color value
 */
static u32 _color_merge(u32 color1, u32 color2, u32 ratio)
{
	u8 r1, g1, b1, r2, g2, b2;

	r1 = _abgr_r(color1);
	g1 = _abgr_g(color1);
	b1 = _abgr_b(color1);

	r2 = _abgr_r(color2);
	g2 = _abgr_g(color2);
	b2 = _abgr_b(color2);

	return gfx_color(
		(r1 * ratio + r2 * (255 - ratio)) / 255,
		(g1 * ratio + g2 * (255 - ratio)) / 255,
		(b1 * ratio + b2 * (255 - ratio)) / 255);
}

/**
 * @brief Convert a RGB565 color to an BGRA color
 *
 * @param color RGB565 color
 * @return BGRA color
 */
static u32 _rgb565_to_bgra(u16 color)
{
	u32 r, g, b;
	r = (((((u32)color >> 11) & 0x1F) * 527) + 23) >> 6;
	g = (((((u32)color >> 5) & 0x3F) * 259) + 33) >> 6;
	b = ((((u32)color & 0x1F) * 527) + 23) >> 6;
	return gfx_color(r, g, b);
}

void env_gfx_rect(u16 x, u16 y, u16 w, u16 h, u32 color)
{
	i32 x0, y0;

	color = _abgr_to_argb(color);
	for(y0 = y; y0 < y + h; ++y0)
	{
		for(x0 = x; x0 < x + w; ++x0)
		{
			_pixels[y0 * GFX_WIDTH + x0] = color;
		}
	}
}

void env_gfx_image_rgb565(u16 x, u16 y, u16 w, u16 h, u32 addr)
{
	i32 x0, y0;
	u16 *image;

	image = (u16 *)(_memory + addr);
	for(y0 = y; y0 < y + h; ++y0)
	{
		for(x0 = x; x0 < x + w; ++x0)
		{
			_pixels[y0 * GFX_WIDTH + x0] = _rgb565_to_bgra(*image++);
		}
	}
}

void env_gfx_image_grayscale(
	u16 x, u16 y, u16 w, u16 h, u32 addr, u32 fg, u32 bg)
{
	i32 x0, y0;
	u8 *image;

	image = (u8 *)(_memory + addr);
	for(y0 = y; y0 < y + h; ++y0)
	{
		for(x0 = x; x0 < x + w; ++x0)
		{
			_pixels[y0 * GFX_WIDTH + x0] =
				_color_merge(fg, bg, *image++);
		}
	}
}

void env_gfx_image_1bit(
	u16 x, u16 y, u16 w, u16 h, u32 addr, u32 fg, u32 bg)
{
	u8 byte, stride;
	i32 x0, y0, byte_offset, bit_mask;
	u8 *image;

	image = (u8 *)(_memory + addr);
	fg = _abgr_to_argb(fg);
	bg = _abgr_to_argb(bg);
	stride = (w + 7) / 8;
	for(y0 = y; y0 < y + h; ++y0)
	{
		bit_mask = 0x80;
		byte_offset = 0;
		for(x0 = x; x0 < x + w; ++x0)
		{
			if(bit_mask == 0x80)
			{
				byte = image[byte_offset++];
				bit_mask = 1;
			}

			_pixels[y0 * GFX_WIDTH + x0] = (byte & bit_mask) ? fg : bg;
			bit_mask <<= 1;
		}

		image += stride;
	}
}

/* --- SERIAL --- */

void env_serial_write(const void *data, u32 len)
{
	/* Debug print */
	printf("%.*s", len, (char *)data);
}

/* --- RANDOM --- */

u32 env_random_get(void)
{
	return rand();
}

/* --- TIMER --- */

/**
 * @brief Initialize simulated millisecond timer
 */
static void timer_init(void)
{
	struct timeval ts;
	gettimeofday(&ts, NULL);
	_sec_start = ts.tv_sec;
	_usec_start = ts.tv_usec;

	srand(time(NULL));
}

u32 env_millis(void)
{
	struct timeval ts;
	gettimeofday(&ts, NULL);
	return (ts.tv_sec - _sec_start) * 1000 +
		(ts.tv_usec - _usec_start) / 1000;
}

/* --- KEYBOARD --- */

/**
 * @brief Combine scancode and key modifiers into one value
 *
 * @param scancode Scancode
 * @param mod Key Modifiers
 * @return Combined Keycode
 */
static u32 _convert_key(i32 scancode, i32 mod)
{
	u32 key = scancode;

	if(mod & (KMOD_LCTRL | KMOD_RCTRL))
	{
		key |= MOD_CTRL;
	}

	if(mod & KMOD_LALT)
	{
		key |= MOD_ALT;
	}

	if(mod & KMOD_RALT)
	{
		key |= MOD_ALT_GR;
	}

	if(mod & (KMOD_LGUI | KMOD_RGUI))
	{
		key |= MOD_OS;
	}

	if(mod & (KMOD_LSHIFT | KMOD_RSHIFT))
	{
		key |= MOD_SHIFT;
	}

	return key;
}

/* --- MAIN --- */

/**
 * @brief Simulator main function
 *
 * @param argc Number of command line parameters
 * @param argv Array of command line parameters
 * @return Exit code
 */
int main(int argc, char **argv)
{
	bool running = true;
	size_t len, offset;
	FILE *fp;

	if(argc != 2)
	{
		fprintf(stderr, "Usage: ./emu rv32i-binary\n");
		return 1;
	}

	/* Load binary into memory */
	if(!(fp = fopen(argv[1], "rb")))
	{
		fprintf(stderr, "Failed to open file \"%s\"\n", argv[1]);
		return 1;
	}

	offset = 0;
	do
	{
		len = fread(_memory + offset, 1, READ_SIZE, fp);
		offset += len;
	}
	while(len == READ_SIZE);

	timer_init();
	gfx_init();
	kernel_init();

	while(running)
	{
		SDL_Event e;
		while(SDL_PollEvent(&e))
		{
			switch(e.type)
			{
			case SDL_QUIT:
				running = false;
				break;

			case SDL_KEYDOWN:
				{
					u32 key;
					if(e.key.keysym.sym == SDLK_ESCAPE)
					{
						running = false;
						break;
					}

					key = _convert_key(e.key.keysym.scancode, e.key.keysym.mod);
					keyboard_event(key, key_to_codepoint(key),
						e.key.repeat ? KEYSTATE_REPEAT : KEYSTATE_PRESSED);
				}
				break;

			case SDL_KEYUP:
				{
					u32 key = _convert_key(e.key.keysym.scancode, e.key.keysym.mod);
					keyboard_event(key, key_to_codepoint(key), KEYSTATE_RELEASED);
				}
				break;
			}
		}

		kernel_update();

		SDL_UpdateTexture(_framebuffer, NULL, _pixels, GFX_WIDTH * sizeof(u32));
		SDL_RenderClear(_renderer);
		SDL_RenderCopy(_renderer, _framebuffer, NULL, NULL);
		SDL_RenderPresent(_renderer);
	}

	gfx_destroy();
	return 0;
}
