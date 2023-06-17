/**
 * @file    emulator.h
 * @author  Anton Tchekov
 * @version 0.1
 * @date    17.05.2023
 * @brief   RISC-V Emulator
 */

#ifndef __EMULATOR_H__
#define __EMULATOR_H__

#include <types.h>
#include <event-types.h>
#include <keyboard-shared.h>
#include <inttypes.h>

/** Emulated process */
typedef struct
{
	/** Event handler addresses */
	u32 Events[EVENT_COUNT];

	/** Program counter */
	u32 PC;

	/** CPU Registers */
	u32 Registers[32];
} Emulator;

/**
 * @brief Initialize emulator kernel and load first process (NOT systemd)
 */
void kernel_init(void);

/**
 * @brief Call this function periodically in main loop
 */
void kernel_update(void);

/**
 * @brief Inform the emulator about a key press
 *
 * @param key The scancode
 * @param chr The unicode codepoint of the scancode or 0
 * @param state The state of the key (pressed, repeat, released)
 */
void keyboard_event(u16 key, char chr, KeyState state);

/**
 * @brief Call an emulated RISC-V function
 *
 * @param emu Pointer to emulator struct
 * @param addr Function start address
 * @param args Function arguments
 * @param num Number of arguments
 */
u8 emulator_call(Emulator *emu, u32 addr, u32 *args, u8 num);

#endif /* __EMULATOR_H__ */
