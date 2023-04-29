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

/* --- MEMORY --- */
static u8 _memory[1024 * 1024];

/* Store */
void memory_sb(u32 address, u32 value)
{
	i8 value8 = (i8)value;
	if(address >= sizeof(_memory))
	{
		return;
	}

	_memory[address] = value8;
}

void memory_sh(u32 address, u32 value)
{
	i16 value16 = (i16)value;
	address >>= 1;
	if(address >= (sizeof(_memory) / 2))
	{
		return;
	}

	((i16 *)_memory)[address] = value16;
}

void memory_sw(u32 address, u32 value)
{
	address >>= 2;
	if(address >= (sizeof(_memory) / 4))
	{
		return;
	}

	((u32 *)_memory)[address] = value;
}

/* Load */
u32 memory_lb(u32 address)
{
	if(address >= sizeof(_memory))
	{
		return 0;
	}

	return (i32)(((i8 *)_memory)[address]);
}

u32 memory_lh(u32 address)
{
	address >>= 1;
	if(address >= (sizeof(_memory) / 2))
	{
		return 0;
	}

	return ((i16 *)_memory)[address];
}

u32 memory_lw(u32 address)
{
	address >>= 2;
	if(address >= (sizeof(_memory) / 4))
	{
		return 0;
	}

	return ((u32 *)_memory)[address];
}

u32 memory_lbu(u32 address)
{
	return _memory[address];
}

u32 memory_lhu(u32 address)
{
	address >>= 1;
	return ((u16 *)_memory)[address];
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
#define WINDOW_TITLE             "OS Simulator"
#define WINDOW_WIDTH          320
#define WINDOW_HEIGHT         480

static u32 _pixels[WINDOW_HEIGHT * WINDOW_WIDTH];
static SDL_Texture *_framebuffer;
static SDL_Window *_window;
static SDL_Renderer *_renderer;

static void gfx_init(void)
{
	/* Init SDL */
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("Error initializing SDL; SDL_Init: %s\n",
			SDL_GetError());
		goto ERROR_INIT_SDL;
	}

	/* Create SDL_Window */
	if(!(_window = SDL_CreateWindow(WINDOW_TITLE,
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH, WINDOW_HEIGHT, 0)))
	{
		printf("Error creating SDL_Window: %s\n",
			SDL_GetError());
		goto ERROR_CREATE_WINDOW;
	}

	/* Create SDL_Renderer */
	if(!(_renderer = SDL_CreateRenderer
		(_window, -1, SDL_RENDERER_ACCELERATED)))
	{
		printf("Error creating SDL_Renderer: %s\n",
			SDL_GetError());
		goto ERROR_CREATE_RENDERER;
	}

	SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);

	_framebuffer = SDL_CreateTexture(_renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		WINDOW_WIDTH, WINDOW_HEIGHT);

	return;

ERROR_CREATE_RENDERER:
	SDL_DestroyWindow(_window);

ERROR_CREATE_WINDOW:
	SDL_Quit();

ERROR_INIT_SDL:
	exit(1);
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
static inline u8 _abgr_b(u32 color) { return (color >> 8) & 0xFF; }

static u32 _abgr_to_argb(u32 color)
{
	return gfx_color(
		_abgr_r(color),
		_abgr_g(color),
		_abgr_b(color));
}

static void gfx_rect(i32 x, i32 y, i32 w, i32 h, u32 color)
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

static void gfx_image_1bit(
	i32 x, i32 y, i32 w, i32 h, u8 *image, u32 fg, u32 bg)
{
	u8 byte, stride;
	i32 x0, y0, byte_offset, bit_mask;

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

static void gfx_image_grayscale(
	i32 x, i32 y, i32 w, i32 h, u8 *image, u32 fg, u32 bg)
{
	i32 x0, y0;
	for(y0 = y; y0 < y + h; ++y0)
	{
		for(x0 = x; x0 < x + w; ++x0)
		{
			_pixels[y0 * WINDOW_WIDTH + x0] =
				_color_merge(fg, bg, *image++);
		}
	}
}

static u32 _rgb565_to_bgra(u16 color)
{
	u32 r, g, b;
	r = (((((u32)color >> 11) & 0x1F) * 527) + 23) >> 6;
	g = (((((u32)color >> 5) & 0x3F) * 259) + 33) >> 6;
	b = ((((u32)color & 0x1F) * 527) + 23) >> 6;
	return gfx_color(r, g, b);
}

static void gfx_image_rgb565(i32 x, i32 y, i32 w, i32 h, u16 *image)
{
	i32 x0, y0;
	for(y0 = y; y0 < y + h; ++y0)
	{
		for(x0 = x; x0 < x + w; ++x0)
		{
			_pixels[y0 * WINDOW_WIDTH + x0] = _rgb565_to_bgra(*image++);
		}
	}
}

static void gfx_image_rgb(i32 x, i32 y, i32 w, i32 h, u8 *image)
{
	u8 r, g, b;
	i32 x0, y0;
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

static void gfx_image_rgba(i32 x, i32 y, i32 w, i32 h, u32 *image)
{
	i32 x0, y0;
	for(y0 = y; y0 < y + h; ++y0)
	{
		for(x0 = x; x0 < x + w; ++x0)
		{
			_pixels[y0 * WINDOW_WIDTH + x0] = _abgr_to_argb(*image++);
		}
	}
}

static Emulator emu;

void os_update(void)
{
	i32 i;
	for(i = 0; i < 10000; ++i)
	{
		emulator_next(&emu);
	}

	/* registers_dump(&emu); */
	/* memory_dump(0, 1024); */
}

/* --- KEYBOARD --- */
#define NUM_KEYS 340

typedef u32 Key;

static bool _keys[NUM_KEYS];

void keyboard_event(Key key, bool up)
{
	if(key >= NUM_KEYS)
	{
		_keys[key] = up;
	}
}

/* --- PLATFORM --- */
static bool platform_run(void)
{
	SDL_Event e;
	while(SDL_PollEvent(&e))
	{
		switch(e.type)
		{
		case SDL_QUIT:
			return false;

		case SDL_KEYDOWN:
			if(e.key.keysym.sym == SDLK_ESCAPE)
			{
				return false;
			}

			keyboard_event(e.key.keysym.sym, false);
			break;

		case SDL_KEYUP:
			keyboard_event(e.key.keysym.sym, true);
			break;
		}
	}

	os_update();

	SDL_UpdateTexture(_framebuffer, NULL, _pixels, WINDOW_WIDTH * sizeof(u32));
	SDL_RenderClear(_renderer);
	SDL_RenderCopy(_renderer, _framebuffer, NULL, NULL);
	SDL_RenderPresent(_renderer);
	return true;
}

#define READ_SIZE 1024

/* TIME */
typedef struct
{
	i32 Year, Month, Day;
	i32 Hour, Minute, Second;
} DateTime;

static u32 _sec_start, _usec_start;

void timer_init(void)
{
	struct timeval ts;
	gettimeofday(&ts, NULL);
	_sec_start = ts.tv_sec;
	_usec_start = ts.tv_usec;

	srand(time(0));
}

u32 syscall_datetime_now(u32 *args)
{
	time_t t = time(NULL);
	struct tm now = *localtime(&t);
	DateTime *out = (DateTime *)(_memory + args[0]);

	out->Year = now.tm_year + 1900;
	out->Month = now.tm_mon + 1;
	out->Day = now.tm_mday;

	out->Hour = now.tm_hour;
	out->Minute = now.tm_min;
	out->Second = now.tm_sec;

	return 0;
}

u32 syscall_millis(u32 *args)
{
	struct timeval ts;
	gettimeofday(&ts, NULL);
	return (ts.tv_sec - _sec_start) * 1000 +
		(ts.tv_usec - _usec_start) / 1000;

	(void)args;
}

/* --- SYSCALLS --- */
u32 syscall_exit(u32 *args)
{
	/* TODO */
	exit(args[0]);
	return 0;
}

/* MEM */
u32 syscall_memcpy(u32 *args)
{
	return (u8 *)memcpy(_memory + args[0],
		_memory + args[1], args[2]) - _memory;
}

u32 syscall_memmove(u32 *args)
{
	return (u8 *)memmove(_memory + args[0],
		_memory + args[1], args[2]) - _memory;
}

u32 syscall_memcmp(u32 *args)
{
	return memcmp(_memory + args[0], _memory + args[1], args[2]);
}

u32 syscall_memchr(u32 *args)
{
	return (u8 *)memchr(_memory + args[0], args[1], args[2]) - _memory;
}

u32 syscall_memset(u32 *args)
{
	return (u8 *)memset(_memory + args[0], args[1], args[2]) - _memory;
}

/* STR */
u32 syscall_strcpy(u32 *args)
{
	return strcpy((char *)(_memory + args[0]),
		(char *)(_memory + args[1])) - (char *)_memory;
}

u32 syscall_strncpy(u32 *args)
{
	return strncpy((char *)(_memory + args[0]),
		(char *)(_memory + args[1]), args[2]) - (char *)_memory;
}

u32 syscall_strlen(u32 *args)
{
	return strlen((char *)(_memory + args[0]));
}

u32 syscall_strcmp(u32 *args)
{
	return strcmp((char *)(_memory + args[0]), (char *)(_memory + args[1]));
}

u32 syscall_strncmp(u32 *args)
{
	return strncmp((char *)(_memory + args[0]),
		(char *)(_memory + args[1]), args[2]);
}

u32 syscall_strchr(u32 *args)
{
	return strchr((char *)(_memory + args[0]), args[1]) - (char *)_memory;
}

/* RAND */
u32 syscall_rand(u32 *args)
{
	return rand();
	(void)args;
}

/* GFX */
typedef struct
{
	i32 X, Y, W, H;
} Rectangle;

u32 syscall_gfx_rect(u32 *args)
{
	gfx_rect(args[0], args[1], args[2], args[3], args[4]);
	return 0;
}

u32 syscall_gfx_image_rgba(u32 *args)
{
	gfx_image_rgba(args[0], args[1], args[2], args[3],
		(u32 *)(_memory + args[4]));
	return 0;
}

u32 syscall_gfx_image_rgb(u32 *args)
{
	gfx_image_rgb(args[0], args[1], args[2], args[3],
		(u8 *)(_memory + args[4]));
	return 0;
}

u32 syscall_gfx_image_rgb565(u32 *args)
{
	gfx_image_rgb565(args[0], args[1], args[2], args[3],
		(u16 *)(_memory + args[4]));
	return 0;
}

u32 syscall_gfx_image_grayscale(u32 *args)
{
	Rectangle *r = (Rectangle *)(_memory + args[0]);
	gfx_image_grayscale(r->X, r->Y, r->W, r->H,
		(u8 *)(_memory + args[1]), args[2], args[3]);
	return 0;
}

u32 syscall_gfx_image_1bit(u32 *args)
{
	Rectangle *r = (Rectangle *)(_memory + args[0]);
	gfx_image_1bit(r->X, r->Y, r->W, r->H,
		(u8 *)(_memory + args[1]), args[2], args[3]);
	return 0;
}

/* FS */
static FILE *_files[256];

static u32 _find_slot(void)
{
	u32 i;
	for(i = 1; i < ARRLEN(_files); ++i)
	{
		if(!_files[i])
		{
			return i;
		}
	}

	return 0;
}

u32 syscall_file_open(u32 *args)
{
	u32 id = _find_slot();
	u32 *handle = (u32 *)(_memory + args[1]);

	if(!(_files[id] = fopen((char *)(_memory + args[0]), "rb")))
	{
		return STATUS_FAIL;
	}

	*handle = id;
	return STATUS_OK;
}

u32 syscall_file_read(u32 *args)
{
	u32 file = args[0];
	u32 start = args[1];
	u32 len = args[2];
	void *buf = _memory + args[3];
	FILE *fp;

	if(file < 1 || file >= ARRLEN(_files))
	{
		return STATUS_FAIL;
	}

	if(!(fp = _files[file]))
	{
		return STATUS_FAIL;
	}

	fseek(fp, start, SEEK_SET);
	return fread(buf, 1, len, fp) ? STATUS_FAIL : STATUS_OK;
}

u32 syscall_file_write(u32 *args)
{
	/* File writing is not supported */
	return 0;
	(void)args;
}

u32 syscall_file_close(u32 *args)
{
	u32 file = args[0];
	fclose(_files[file]);
	_files[file] = 0;
	return 0;
}

u32 syscall_file_size(u32 *args)
{
	u32 file = args[0];
	u32 *size = (u32 *)(_memory + args[1]);
	FILE *fp;

	if(!(fp = _files[file]))
	{
		return STATUS_FAIL;
	}

	fseek(fp, 0, SEEK_END);
	*size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	return STATUS_OK;
}

/* KBD */
u32 syscall_keyboard_is_key_pressed(u32 *args)
{
	Key key = args[0];
	if(key > NUM_KEYS)
	{
		return false;
	}

	return _keys[key];
}

u32 syscall_keyboard_register_event(u32 *args)
{
	/* TODO */
	return 0;
	(void)args;
}

u32 syscall_serial_write(u32 *args)
{
	/* Debug print */
	fputs((char *)(_memory + args[0]), stdout);
	return 0;
}

u32 syscall_serial_read(u32 *args)
{
	/* Always return 0 bytes read */
	return 0;
	(void)args;
}

int main(int argc, char **argv)
{
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

	emulator_init(&emu, 0, sizeof(_memory));

	timer_init();
	gfx_init();
	while(platform_run()) {}
	gfx_destroy();
	return 0;
}
