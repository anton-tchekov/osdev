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
#include <keyboard-shared.h>

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

#ifdef DEBUG

/**
 * @brief Print the emulators current register content for debugging purposes
 *
 * @param emu The emulator
 */
void emulator_dump_registers(Emulator *emu);

#endif /* DEBUG */

/**
 * @brief Initialize emulator kernel and load first process (NOT systemd)
 */
void kernel_init(void);

/**
 * @brief TODO: This is ugly and needs to be replaced
 */
void process_setup(void);

/**
 * @brief Call this function periodically in main loop
 */
void os_update(void);

/**
 * @brief Inform the emulator about a key press
 *
 * @param key The scancode
 * @param chr The unicode codepoint of the scancode or 0
 * @param state The state of the key (pressed, repeat, released)
 */
void keyboard_event(Key key, i32 chr, KeyState state);

/**
 * @brief Call a function in the emulated process
 *
 * @param emu
 * @param addr
 * @param args
 * @param num
 * @param sp
 * @return i32
 */
void emulator_call(Emulator *emu, u32 addr, u32 *args, u8 num, u32 sp);

#endif /* __EMULATOR_H__ */
