/**
 * @file    keyboard.c
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    23.04.2023
 */

#include <keyboard.h>
#include <syscall.h>

bool keyboard_is_key_pressed(Key key)
{
	return syscall1(SYSCALL_KEYBOARD_IS_KEY_PRESSED, key);
}

void keyboard_register_event(void (*event)(Key key, bool up))
{
	syscall1(SYSCALL_KEYBOARD_REGISTER_EVENT, (u32)event);
}