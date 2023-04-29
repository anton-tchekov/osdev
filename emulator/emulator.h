#ifndef __EMULATOR_H__
#define __EMULATOR_H__

#include <types.h>

typedef struct
{
	u32 PC;
	u32 Registers[32];
} Emulator;

void emulator_dump_registers(Emulator *emu);
i32 emulator_next(Emulator *emu);
i32 emulator_call(Emulator *emu, u32 addr, u32 *args, u32 num, u32 sp, u32 max_instr);

#endif /* __EMULATOR_H__ */
