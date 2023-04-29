#ifndef __EMULATOR_H__
#define __EMULATOR_H__

#include <types.h>

typedef struct
{
	u32 PC;
	u32 Registers[32];
} Emulator;

void emulator_dump_registers(Emulator *emu);
void emulator_init(Emulator *emu, u32 pc, u32 sp);
i32 emulator_next(Emulator *emu);

#endif /* __EMULATOR_H__ */
