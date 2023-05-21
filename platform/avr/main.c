#include <ps2.h>
#include <serial.h>
#include <adc.h>
#include <random.h>
#include <timer.h>
#include <rtc.h>
#include <xmem.h>
#include <logger.h>
#include <spi.h>
#include <lcd.h>
#include <gpio.h>
#include <sd.h>
#include <initrd.h>
#include <avr/pgmspace.h>
#include "logo/logo_tiny.c"
#include "logo/logo_text.c"
#include <emulator.h>

char byte_to_hex(u8 byte)
{
	if(byte < 10)
	{
		return byte + '0';
	}

	return byte - 10 + 'A';
}

void ps2_event(u8 byte)
{
	/* Called when a byte is received from the keyboard */
	/* serial_tx('0'); */
	/* serial_tx('x'); */
	/* serial_tx(byte_to_hex((byte >> 4) & 0x0F)); */
	/* serial_tx(byte_to_hex(byte & 0x0F)); */
}

void serial_write(const void *data, u32 len)
{
	u32 i;
	const char *data8 = data;
	for(i = 0; i < len; ++i)
	{
		serial_tx(data8[i]);
	}
}

u32 memory_size(void)
{
	return 3UL * 128UL * 1024UL;
}

void memory_read(u32 addr, void *data, u32 size)
{
	xmem_read(addr, data, size);
}

void memory_write(u32 addr, const void *data, u32 size)
{
	xmem_write(addr, data, size);
}

static inline u8 _abgr_r(u32 color) { return (color >> 24) & 0xFF; }
static inline u8 _abgr_g(u32 color) { return (color >> 16) & 0xFF; }
static inline u8 _abgr_b(u32 color) { return (color >>  8) & 0xFF; }

const uint8_t PROGMEM gamma[] = {
    0, 21, 28, 34, 39, 43, 46, 50, 53, 56, 59, 61, 64, 66, 68, 70,
   72, 74, 76, 78, 80, 82, 84, 85, 87, 89, 90, 92, 93, 95, 96, 98,
   99,101,102,103,105,106,107,109,110,111,112,114,115,116,117,118,
  119,120,122,123,124,125,126,127,128,129,130,131,132,133,134,135,
  136,137,138,139,140,141,142,143,144,144,145,146,147,148,149,150,
  151,151,152,153,154,155,156,156,157,158,159,160,160,161,162,163,
  164,164,165,166,167,167,168,169,170,170,171,172,173,173,174,175,
  175,176,177,178,178,179,180,180,181,182,182,183,184,184,185,186,
  186,187,188,188,189,190,190,191,192,192,193,194,194,195,195,196,
  197,197,198,199,199,200,200,201,202,202,203,203,204,205,205,206,
  206,207,207,208,209,209,210,210,211,212,212,213,213,214,214,215,
  215,216,217,217,218,218,219,219,220,220,221,221,222,223,223,224,
  224,225,225,226,226,227,227,228,228,229,229,230,230,231,231,232,
  232,233,233,234,234,235,235,236,236,237,237,238,238,239,239,240,
  240,241,241,242,242,243,243,244,244,245,245,246,246,247,247,248,
  248,249,249,249,250,250,251,251,252,252,253,253,254,254,255,255
  };

static inline u8 _gamcor(u8 value)
{
	return pgm_read_byte(gamma + value);
}

static RGB565 _abgr_to_rgb565(u32 color)
{
	return lcd_color(
		_gamcor(_abgr_r(color)),
		_gamcor(_abgr_g(color)),
		_gamcor(_abgr_b(color)));
}

void gfx_rect(i32 x, i32 y, i32 w, i32 h, u32 color)
{
	lcd_rect(x, y, w, h, _abgr_to_rgb565(color));
}

/** TODO: To be removed */
u32 syscall_gfx_image_rgba(u32 *args) { (void)args; }

/** TODO: To be removed */
u32 syscall_gfx_image_rgb(u32 *args) { (void)args; }

/** TODO: To be removed */
u32 syscall_gfx_image_rgb565(u32 *args) { (void)args; }

/** TODO: To be removed */
u32 syscall_gfx_image_grayscale(u32 *args) { (void)args; }

/** TODO: To be removed */
u32 syscall_gfx_image_1bit(u32 *args)
{
	u8 byte, stride, bit_mask;
	u32 addr;
	u16 x, y, w, h, x0, y0, byte_offset;
	Rectangle r;
	RGB565 fg, bg;
	u8 image[64];

	xmem_read(args[0], &r, sizeof(r));

	x = r.X;
	y = r.Y;
	w = r.W;
	h = r.H;

	addr = args[1];
	fg = _abgr_to_rgb565(args[2]);
	bg = _abgr_to_rgb565(args[3]);
	stride = (w + 7) / 8;
	for(y0 = y; y0 < y + h; ++y0)
	{
		xmem_read(addr, image, stride);

		_lcd_window_start(x, y0, w, 1);

		bit_mask = 0x80;
		byte_offset = 0;
		for(x0 = x; x0 < x + w; ++x0)
		{
			if(bit_mask == 0x80)
			{
				byte = image[byte_offset++];
				bit_mask = 1;
			}

			_lcd_pixel((byte & bit_mask) ? fg : bg);

			bit_mask <<= 1;
		}

		_lcd_window_end();
		addr += stride;
	}

	return 0;
}

#define LOGO_X_OFFSET      30
#define LOGO_TEXT_X_OFFSET 56
#define LOGO_Y_OFFSET      50

int main(void)
{
	/* --- BOOT SEQUENCE --- */

	/* Enable interrupts */
	sei();

	/* First, configure GPIO, because all modules that do I/O depend on it */
	gpio_configure();

	/* Second, initialize serial port so that we can print boot messages */
	serial_init();

	/* Print startup message */
	log_boot_P(LOG_NONE, PSTR("\nImaginaryOS Version 0.1 Booting ...\n"));

	/* Initialize millisecond timer */
	timer_init();

	/* Initialize SPI (Needed for SD Card, External Memory and LCD) */
	spi_init();

	/* Initialize LCD driver early for boot image and messages */
	lcd_init(0xFF, COLOR_BLACK);
	lcd_logo_P(
		LOGO_X_OFFSET,
		LOGO_Y_OFFSET,
		LOGO_TINY_WIDTH,
		LOGO_TINY_HEIGHT,
		logo_tiny);

	lcd_logo_P(
		LOGO_X_OFFSET + LOGO_TEXT_X_OFFSET,
		LOGO_Y_OFFSET + LOGO_TINY_HEIGHT / 2 - LOGO_TEXT_HEIGHT / 2,
		LOGO_TEXT_WIDTH,
		LOGO_TEXT_HEIGHT,
		logo_text);

	/* Initialize keyboard (fast) */
	ps2_init();

	/* Initialize ADC (fast) */
	adc_init();

	/* Initialize RNG (fast) */
	random_init();

	/* Initialize RTC driver (fast) */
	rtc_init();

	/* Initialize external memory late because of slow memory test */
	xmem_init();

	/* Initialize SD card driver */
	sd_init();

	/* --- READY --- */

	/* Load initial ram disk */
	initrd_load();

	log_boot_P(LOG_NONE, PSTR("\nStarting RISC-V Emulator Kernel ...\n\n"));

	kernel_init();
	log_boot_P(LOG_EXT, PSTR("Setup done!"));

	/* Infinite loop */
	for(;;)
	{
	}

	/* Unreachable */
	return 0;
}

