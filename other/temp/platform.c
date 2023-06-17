/**
 * @file    platform.c
 * @author  Anton Tchekov
 * @version 0.1
 * @date    22.05.2023
 * @brief   Platform specific functions for AVR
 */

#include <platform.h>
#include <serial.h>
#include <lcd.h>
#include <xmem.h>
#include <random.h>
#include <timer.h>
#include <avr/pgmspace.h>
#include <gfx-types.h>

/* --- MEMORY --- */
void env_memory_read(u32 addr, void *data, u32 size)
{
	xmem_read(addr, data, size);
}

void env_memory_write(u32 addr, const void *data, u32 size)
{
	xmem_write(addr, data, size);
}

/* --- GRAPHICS --- */

/** Reverse gamma correction table */
static const u8 _gamma[] PROGMEM =
{
	  0,  21,  28,  34,  39,  43,  46,  50,  53,  56,  59,  61,  64,  66,  68,  70,
	 72,  74,  76,  78,  80,  82,  84,  85,  87,  89,  90,  92,  93,  95,  96,  98,
	 99, 101, 102, 103, 105, 106, 107, 109, 110, 111, 112, 114, 115, 116, 117, 118,
	119, 120, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135,
	136, 137, 138, 139, 140, 141, 142, 143, 144, 144, 145, 146, 147, 148, 149, 150,
	151, 151, 152, 153, 154, 155, 156, 156, 157, 158, 159, 160, 160, 161, 162, 163,
	164, 164, 165, 166, 167, 167, 168, 169, 170, 170, 171, 172, 173, 173, 174, 175,
	175, 176, 177, 178, 178, 179, 180, 180, 181, 182, 182, 183, 184, 184, 185, 186,
	186, 187, 188, 188, 189, 190, 190, 191, 192, 192, 193, 194, 194, 195, 195, 196,
	197, 197, 198, 199, 199, 200, 200, 201, 202, 202, 203, 203, 204, 205, 205, 206,
	206, 207, 207, 208, 209, 209, 210, 210, 211, 212, 212, 213, 213, 214, 214, 215,
	215, 216, 217, 217, 218, 218, 219, 219, 220, 220, 221, 221, 222, 223, 223, 224,
	224, 225, 225, 226, 226, 227, 227, 228, 228, 229, 229, 230, 230, 231, 231, 232,
	232, 233, 233, 234, 234, 235, 235, 236, 236, 237, 237, 238, 238, 239, 239, 240,
	240, 241, 241, 242, 242, 243, 243, 244, 244, 245, 245, 246, 246, 247, 247, 248,
	248, 249, 249, 249, 250, 250, 251, 251, 252, 252, 253, 253, 254, 254, 255, 255
};

/**
 * @brief Perform gamma correction on a color channel
 *
 * @param value Input color channel value
 * @return Gamma corrected output value
 */
static inline u8 _gamma_correction(u8 value)
{
	return pgm_read_byte(_gamma + value);
}

/**
 * @brief Convert ABGR8888 to RGB565 and do gamma correction
 *        for LCD with low contrast
 *
 * @param color The color to be converted
 * @return RGB565 color
 */
static RGB565 _abgr_to_rgb565(u32 color)
{
	return lcd_color(
		_gamma_correction(_abgr_r(color)),
		_gamma_correction(_abgr_g(color)),
		_gamma_correction(_abgr_b(color)));
}

void env_gfx_rect(u16 x, u16 y, u16 w, u16 h, u32 color)
{
	lcd_rect(x, y, w, h, _abgr_to_rgb565(color));
}

void env_gfx_image_rgb565(u16 x, u16 y, u16 w, u16 h, u32 addr)
{
	lcd_image_rgb565(x, y, w, h, addr);
}

void env_gfx_image_grayscale(
	u16 x, u16 y, u16 w, u16 h, u32 addr, u32 fg, u32 bg)
{
	lcd_image_grayscale(x, y, w, h, addr, fg, bg);
}

void env_gfx_image_1bit(
	u16 x, u16 y, u16 w, u16 h, u32 addr, u32 fg, u32 bg)
{
	lcd_image_1bit(x, y, w, h, addr,
		_abgr_to_rgb565(fg),
		_abgr_to_rgb565(bg));
}

/* --- SERIAL --- */

void env_serial_write(const void *data, u32 len)
{
	u32 i;
	const char *data8 = data;
	for(i = 0; i < len; ++i)
	{
		serial_tx(data8[i]);
	}
}

/* --- RANDOM --- */

u32 env_random_get(void)
{
	return random_get();
}

/* --- TIMER --- */

u32 env_millis(void)
{
	return millis();
}

/**
 * @brief Get the size of the installed memory
 *
 * @return The size of the memory in bytes
 */
u32 env_memory_size(void);

/**
 * @brief Draw an RGB565 image
 *
 * @param x X-Coordinate
 * @param y Y-Coordinate
 * @param w Width
 * @param h Height
 * @param addr Image memory address
 */
void env_gfx_image_rgb565(u16 x, u16 y, u16 w, u16 h, u32 addr);

/**
 * @brief Draw a colored grayscale image
 *
 * @param x X-Coordinate
 * @param y Y-Coordinate
 * @param w Width
 * @param h Height
 * @param addr Image memory address
 * @param fg ABGR foreground color
 * @param bg ABGR background color
 */
void env_gfx_image_grayscale(
	u16 x, u16 y, u16 w, u16 h, u32 addr, u32 fg, u32 bg);

/**
 * @brief Draw a colored 1-bit image
 *
 * @param x X-Coordinate
 * @param y Y-Coordinate
 * @param w Width
 * @param h Height
 * @param addr Image memory address
 * @param fg ABGR foreground color
 * @param bg ABGR background color
 */
void env_gfx_image_1bit(
	u16 x, u16 y, u16 w, u16 h, u32 addr, u32 fg, u32 bg);
