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
#define WINDOW_TITLE             "OS Simulator"
#define WINDOW_WIDTH          320
#define WINDOW_HEIGHT         480

#define READ_SIZE            1024

/* --- VARIABLES --- */

static u8 _memory[3 * 128 * 1024];
static u32 _pixels[WINDOW_HEIGHT * WINDOW_WIDTH];
static u32 _sec_start, _usec_start;
static SDL_Texture *_framebuffer;
static SDL_Window *_window;
static SDL_Renderer *_renderer;

/* --- MEMORY --- */

u32 env_memory_size(void)
{
	return sizeof(_memory);
}

void env_memory_read(u32 addr, void *data, u32 size)
{
	memcpy(data, _memory + addr, size);
}

void env_memory_write(u32 addr, const void *data, u32 size)
{
	memcpy(_memory + addr, data, size);
}

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
		WINDOW_WIDTH, WINDOW_HEIGHT, 0)))
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
		WINDOW_WIDTH, WINDOW_HEIGHT);
}

static void gfx_destroy(void)
{
	SDL_DestroyRenderer(_renderer);
	SDL_DestroyWindow(_window);
	SDL_Quit();
}

static u32 gfx_color(u8 r, u8 g, u8 b)
{
	return ((u32)0xFF << 24) |
		((u32)r << 16) |
		((u32)g << 8) |
		((u32)b);
}

static inline u8 _abgr_r(u32 color) { return (color >> 24) & 0xFF; }
static inline u8 _abgr_g(u32 color) { return (color >> 16) & 0xFF; }
static inline u8 _abgr_b(u32 color) { return (color >>  8) & 0xFF; }

static u32 _abgr_to_argb(u32 color)
{
	return gfx_color(
		_abgr_r(color),
		_abgr_g(color),
		_abgr_b(color));
}

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

static u32 _rgb565_to_bgra(u16 color)
{
	u32 r, g, b;
	r = (((((u32)color >> 11) & 0x1F) * 527) + 23) >> 6;
	g = (((((u32)color >> 5) & 0x3F) * 259) + 33) >> 6;
	b = ((((u32)color & 0x1F) * 527) + 23) >> 6;
	return gfx_color(r, g, b);
}

void env_gfx_rect(i32 x, i32 y, i32 w, i32 h, u32 color)
{
	i32 x0, y0;

	color = _abgr_to_argb(color);
	for(y0 = y; y0 < y + h; ++y0)
	{
		for(x0 = x; x0 < x + w; ++x0)
		{
			_pixels[y0 * WINDOW_WIDTH + x0] = color;
		}
	}
}

void env_gfx_image_rgba(i32 x, i32 y, i32 w, i32 h, u32 addr)
{
	i32 x0, y0;
	u32 *image;

	image = (u32 *)(_memory + addr);
	for(y0 = y; y0 < y + h; ++y0)
	{
		for(x0 = x; x0 < x + w; ++x0)
		{
			_pixels[y0 * WINDOW_WIDTH + x0] = _abgr_to_argb(*image++);
		}
	}
}

void env_gfx_image_rgb(i32 x, i32 y, i32 w, i32 h, u32 addr)
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
			_pixels[y0 * WINDOW_WIDTH + x0] = gfx_color(r, g, b);
		}
	}
}

void env_gfx_image_rgb565(i32 x, i32 y, i32 w, i32 h, u32 addr)
{
	i32 x0, y0;
	u16 *image;

	image = (u16 *)(_memory + addr);
	for(y0 = y; y0 < y + h; ++y0)
	{
		for(x0 = x; x0 < x + w; ++x0)
		{
			_pixels[y0 * WINDOW_WIDTH + x0] = _rgb565_to_bgra(*image++);
		}
	}
}

void env_gfx_image_grayscale(
	i32 x, i32 y, i32 w, i32 h, u32 fg, u32 bg, u32 addr)
{
	i32 x0, y0;
	u8 *image;

	image = (u8 *)(_memory + addr);
	for(y0 = y; y0 < y + h; ++y0)
	{
		for(x0 = x; x0 < x + w; ++x0)
		{
			_pixels[y0 * WINDOW_WIDTH + x0] =
				_color_merge(fg, bg, *image++);
		}
	}
}

void env_gfx_image_1bit(
	i32 x, i32 y, i32 w, i32 h, u32 addr, u32 fg, u32 bg)
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

			_pixels[y0 * WINDOW_WIDTH + x0] = (byte & bit_mask) ? fg : bg;
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

static Key _convert_key(i32 scancode, i32 mod)
{
	Key key = scancode;

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
					Key key;
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
					Key key = _convert_key(e.key.keysym.scancode, e.key.keysym.mod);
					keyboard_event(key, key_to_codepoint(key), KEYSTATE_RELEASED);
				}
				break;
			}
		}

		os_update();

		SDL_UpdateTexture(_framebuffer, NULL, _pixels, WINDOW_WIDTH * sizeof(u32));
		SDL_RenderClear(_renderer);
		SDL_RenderCopy(_renderer, _framebuffer, NULL, NULL);
		SDL_RenderPresent(_renderer);
	}

	gfx_destroy();
	return 0;
}
