#ifndef __EMULATOR_H__
#define __EMULATOR_H__

#include <types.h>

/** Emulated process */
typedef struct
{
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
 * @brief Call a function in the emulated process
 *
 * @param emu
 * @param addr
 * @param args
 * @param num
 * @param sp
 * @param max_instr
 * @return i32
 */
i32 emulator_call(Emulator *emu, u32 addr, u32 *args, u32 num, u32 sp, u32 max_instr);

#endif /* __EMULATOR_H__ */
