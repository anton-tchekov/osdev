#include "spi.c"
#include "uart.c"

#include <assert.h>
#include <string.h>

#include <types.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

int main(void)
{
	xmem_init();
	adc_init();
	uart_init();
	timer_init();
	gfx_init();
	kbd_init();

	for (;;)
	{
	}

	/* unreachable */
	return 0;
}

void memory_sb(u32 address, u32 value)
{
	u8 value8 = value;
	xmem_write(address, &value8, sizeof(value8));
}

void memory_sh(u32 address, u32 value)
{
	u16 value16 = value;
	xmem_write(address, &value16, sizeof(value16));
}

void memory_sw(u32 address, u32 value)
{
	xmem_write(address, &value, sizeof(value));
}

u32 memory_lb(u32 address)
{
	i8 value = 0;
	xmem_read(address, &value, sizeof(value));
	return (i32)value;
}

u32 memory_lh(u32 address)
{
	i16 value = 0;
	xmem_read(address, &value, sizeof(value));
	return (i32)value;
}

u32 memory_lw(u32 address)
{
	u32 value = 0;
	xmem_read(address, &value, sizeof(value));
	return value;
}

u32 memory_lbu(u32 address)
{
	u8 value = 0;
	xmem_read(address, &value, sizeof(value));
	return value;
}

u32 memory_lhu(u32 address)
{
	u16 value = 0;
	xmem_read(address, &value, sizeof(value));
	return value;
}

u32 syscall_exit(u32 *args)
{
	return 0;
}

u32 syscall_event_register(u32 *args)
{
	return 0;
}

u32 syscall_memcpy(u32 *args)
{
	u32 dest = args[0];
	u32 src = args[1];
	u32 count = args[2];
	return xmem_cpy(dest, src, count);
}

u32 syscall_memmove(u32 *args)
{
	return syscall_memcpy(args);
}

u32 syscall_memcmp(u32 *args)
{
	u32 dest = args[0];
	u32 src = args[1];
	u32 count = args[2];
	return xmem_cmp(dest, src, count);
}

u32 syscall_memchr(u32 *args)
{
	u32 dest = args[0];
	u32 src = args[1];
	u32 count = args[2];
	return xmem_chr(dest, src, count);
}

u32 syscall_memset(u32 *args)
{
	u32 ptr = args[0];
	u32 val = args[1];
	u32 count = args[2];
	return xmem_cpy(ptr, val, count);
}
u32 syscall_strcpy(u32 *args)
{
	u32 dest = args[0];
	u32 src = args[1];
	return xmem_strcpy(dest, src);
}

u32 syscall_strncpy(u32 *args)
{
	u32 dest = args[0];
	u32 src = args[1];
	u32 count = args[2];
	return xmem_strncpy(dest, src, count);
}

u32 syscall_strlen(u32 *args)
{
	return xmem_strlen(args[0]);
}

u32 syscall_strcmp(u32 *args)
{
	u32 dest = args[0];
	u32 src = args[1];
	return xmem_strcmp(dest, src);
}

u32 syscall_strncmp(u32 *args)
{
	u32 dest = args[0];
	u32 src = args[1];
	u32 count = args[2];
	return xmem_strncmp(dest, src, count);
}

u32 syscall_strchr(u32 *args)
{
	u32 chr = args[0];
	u32 c = args[1];
	return xmem_strchr(chr, c);
}

u32 syscall_rand(u32 *args)
{
	return random();
	(void)args;
}

u32 syscall_gfx_rect(u32 *args)
{
	/* TODO */
	return 0;
	(void)args;
}

u32 syscall_gfx_image_rgba(u32 *args)
{
	/* TODO */
	return 0;
	(void)args;
}

u32 syscall_gfx_image_rgb(u32 *args)
{
	/* TODO */
	return 0;
	(void)args;
}

u32 syscall_gfx_image_rgb565(u32 *args)
{
	/* TODO */
	return 0;
	(void)args;
}

u32 syscall_gfx_image_grayscale(u32 *args)
{
	/* TODO */
	return 0;
	(void)args;
}

u32 syscall_gfx_image_1bit(u32 *args)
{
	/* TODO */
	return 0;
	(void)args;
}

u32 syscall_file_open(u32 *args)
{
	/* TODO */
	return 0;
	(void)args;
}

u32 syscall_file_write(u32 *args)
{
	/* TODO */
	return 0;
	(void)args;
}

u32 syscall_file_close(u32 *args)
{
	/* TODO */
	return 0;
	(void)args;
}

u32 syscall_file_size(u32 *args)
{
	/* TODO */
	return 0;
	(void)args;
}

u32 syscall_keyboard_is_key_pressed(u32 *args)
{
	/* TODO */
	return 0;
	(void)args;
}

u32 syscall_serial_write(u32 *args)
{
	u32 str = args[0];
	u32 len = args[1];
	uart_tx_str_X(str, len);
	return 0;
}

u32 syscall_datetime_now(u32 *args)
{
	datetime_now(args[0]);
	return 0;
}

u32 syscall_millis(u32 *args)
{
	return millis();
	(void)args;
}
