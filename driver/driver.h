#ifndef __DRIVER_H__
#define __DRIVER_H__

#include <types.h>

u32 syscall_exit(u32 *args);

u32 syscall_memcpy(u32 *args);
u32 syscall_memmove(u32 *args);
u32 syscall_memcmp(u32 *args);
u32 syscall_memchr(u32 *args);
u32 syscall_memset(u32 *args);

u32 syscall_strcpy(u32 *args);
u32 syscall_strncpy(u32 *args);
u32 syscall_strlen(u32 *args);
u32 syscall_strcmp(u32 *args);
u32 syscall_strncmp(u32 *args);
u32 syscall_strchr(u32 *args);

u32 syscall_rand(u32 *args);

u32 syscall_gfx_rect(u32 *args);
u32 syscall_gfx_image_rgba(u32 *args);
u32 syscall_gfx_image_rgb(u32 *args);
u32 syscall_gfx_image_rgb565(u32 *args);
u32 syscall_gfx_image_grayscale(u32 *args);
u32 syscall_gfx_image_1bit(u32 *args);

u32 syscall_file_open(u32 *args);
u32 syscall_file_read(u32 *args);
u32 syscall_file_write(u32 *args);
u32 syscall_file_close(u32 *args);
u32 syscall_file_size(u32 *args);

u32 syscall_keyboard_is_key_pressed(u32 *args);
u32 syscall_keyboard_register_event(u32 *args);

u32 syscall_serial_write(u32 *args);
u32 syscall_serial_read(u32 *args);

#endif /* __DRIVER_H__ */
