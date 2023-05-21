#include <types.h>
#include <platform.h>
#include <emulator.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>
#include <util.h>

/* --- LOGGING --- */
#ifdef DEBUG

#ifdef __linux__

#define EMU_LOG(...) { printf(__VA_ARGS__); fputc('\n', stdout); }
#define PSTR(X) X

#else /* __linux__ */

#include <logger.h>
#include <avr/pgmspace.h>

#define EMU_LOG(...) log_boot_P(LOG_EXT, __VA_ARGS__)

#endif /* __linux__ */

static void emulator_dump_registers(Emulator *emu)
{
	EMU_LOG(PSTR("\nPC  0x%08"PRIx32" %10d"), emu->PC, emu->PC);
	for(i32 i = 0; i < 32; ++i)
	{
		EMU_LOG(PSTR("r%"PRIu8"%s 0x%08"PRIx32" %10d"), i, i < 10 ? " " : "",
			emu->Registers[i], emu->Registers[i]);
	}
}

#else /* DEBUG */

#define EMU_LOG(...)

#endif /* DEBUG */

/* --- OPCODES --- */
#define OPCODE_LOAD   0x00
#define OPCODE_IMM    0x04
#define OPCODE_AUIPC  0x05 /* Add Upper Immediate to Program Counter */
#define OPCODE_STORE  0x08
#define OPCODE_REG    0x0C
#define OPCODE_LUI    0x0D /* Load Upper Immediate */
#define OPCODE_BRANCH 0x18
#define OPCODE_JALR   0x19 /* Jump and Link Register */
#define OPCODE_JAL    0x1B /* Jump and Link */
#define OPCODE_ECALL  0x1C

/**
 * @brief Sign extend twos-complement value to 32-bit
 *
 * @param bits Number of bits
 * @param value Value to sign extent
 * @return Result
 */
static inline u32 sext(u8 bits, u32 value)
{
	u32 m = ((u32)1) << (bits - 1);
	return (value ^ m) - m;
}

static bool _finished;
static Emulator _cur, *_emu = &_cur;
static bool _keys[KEY_COUNT];
static u32 _memory_size;

void kernel_init(void)
{
	_memory_size = memory_size();
	emulator_call(_emu, 0, NULL, 0, _memory_size);
}

void os_update(void)
{
	u32 addr = _emu->Events[EVENT_LOOP];
	if(addr)
	{
		emulator_call(_emu, addr, NULL, 0, _memory_size);
	}
}

void keyboard_event(Key key, i32 chr, KeyState down)
{
	u32 addr = _emu->Events[EVENT_KEY];
	if(addr)
	{
		u32 args[] = { key, chr, down };
		emulator_call(_emu, addr, args, ARRLEN(args), _memory_size);
	}

	key = key_mod_remove(key);
	if(key >= KEY_COUNT)
	{
		return;
	}

	_keys[key] = down;
}

/* Store */
static void memory_sb(u32 addr, u32 value)
{
	i8 value8 = (i8)value;
	memory_write(addr, &value8, 1);
}

static void memory_sh(u32 addr, u32 value)
{
	i16 value16 = (i16)value;
	memory_write(addr, &value16, 2);
}

static void memory_sw(u32 addr, u32 value)
{
	memory_write(addr, &value, 4);
}

/* Load */
static u32 memory_lb(u32 addr)
{
	i8 value8;
	memory_read(addr, &value8, 1);
	return (i32)value8;
}

static u32 memory_lh(u32 addr)
{
	i16 value16;
	memory_read(addr, &value16, 2);
	return (i32)value16;
}

static u32 memory_lw(u32 addr)
{
	u32 value;
	memory_read(addr, &value, 4);
	return value;
}

static u32 memory_lbu(u32 addr)
{
	u8 value8;
	memory_read(addr, &value8, 1);
	return value8;
}

static u32 memory_lhu(u32 addr)
{
	u16 value16;
	memory_read(addr, &value16, 2);
	return value16;
}

/* Syscalls */
static u32 syscall_exit(u32 *args)
{
	/* TODO */

	/* Remove process from scheduler */

	/* If init process, crash system */
	return 0;
	(void)args;
}

static u32 syscall_finish(u32 *args)
{
	_finished = true;
	return 0;
	(void)args;
}

static u32 syscall_event_register(u32 *args)
{
	u32 type = args[0];
	u32 addr = args[1];
	if(type < EVENT_COUNT)
	{
		EMU_LOG(PSTR("Registered Event %"PRIu32" %"PRIu32), type, addr);
		_emu->Events[type] = addr;
	}

	return 0;
}

static u32 syscall_millis(u32 *args)
{
	return millis();
	(void)args;
}

static u32 syscall_datetime_now(u32 *args)
{
	DateTime dt;
	datetime_now(&dt);
	memory_write(args[0], &dt, sizeof(dt));
	return 0;
}

static u32 syscall_rand(u32 *args)
{
	return random_get();
	(void)args;
}

static u32 syscall_keyboard_is_key_pressed(u32 *args)
{
	u32 key = args[0];
	if(key >= KEY_COUNT)
	{
		return false;
	}

	return _keys[key];
}

static u32 syscall_serial_write(u32 *args)
{
	u16 cur;
	u32 ptr = args[0];
	u32 len = args[1];
	u8 buf[128];

	while(len)
	{
		cur = len > sizeof(buf) ? sizeof(buf) : len;
		memory_read(ptr, buf, cur);
		serial_write(buf, cur);
		len -= cur;
		ptr += cur;
	}

	return 0;
}

static u32 syscall_gfx_rect(u32 *args)
{
	gfx_rect(args[0], args[1], args[2], args[3], args[4]);
	return 0;
}

static u32 (*syscalls[])(u32 *) =
{
	syscall_exit,
	syscall_finish,
	syscall_event_register,

	syscall_gfx_rect,
	syscall_gfx_image_rgba,
	syscall_gfx_image_rgb,
	syscall_gfx_image_rgb565,
	syscall_gfx_image_grayscale,
	syscall_gfx_image_1bit,

	syscall_rand,
	syscall_keyboard_is_key_pressed,
	syscall_serial_write,
	syscall_datetime_now,
	syscall_millis,
};

static i32 syscall(u32 id, u32 *args)
{
	if(id >= ARRLEN(syscalls))
	{
		return 1;
	}

	args[0] = syscalls[id](args);
	return 0;
}

/* EMULATOR */
static bool emulator_next(Emulator *emu)
{
	u32 instr, opcode;
	instr = memory_lw(emu->PC);

	/* r0 = Zero Register */
	emu->Registers[0] = 0;

	/* bits 7 .. 0 */
	opcode = instr & 0x7F;
	if((opcode & 0x03) != 0x03)
	{
		EMU_LOG(PSTR("INVALID"));
		return true;
	}

	opcode >>= 2;
	switch(opcode)
	{
		case OPCODE_ECALL:
			if(syscall(emu->Registers[17], &emu->Registers[10]))
			{
				EMU_LOG(PSTR("SYSCALL ERROR"));
				return true;
			}
			break;

		case OPCODE_LOAD:
		{
			i32 offset;
			u32 imm;
			u8 rd, rs1, funct3;
			rd = (instr >> 7) & 0x1F;
			funct3 = (instr >> 12) & 0x07;
			rs1 = (instr >> 15) & 0x1F;
			imm = (instr >> 20);
			offset = sext(12, imm);
			switch(funct3)
			{
				case 0:
					/* LB */
					EMU_LOG(PSTR("lb r%"PRIu8", r%"PRIu8"%+"PRIi32), rd, rs1, offset);
					emu->Registers[rd] = memory_lb(emu->Registers[rs1] + offset);
					break;

				case 1:
					/* LH */
					EMU_LOG(PSTR("lh r%"PRIu8", r%"PRIu8"%"PRIi32), rd, rs1, offset);
					emu->Registers[rd] = memory_lh(emu->Registers[rs1] + offset);
					break;

				case 2:
					/* LW */
					EMU_LOG(PSTR("lw r%"PRIu8", r%"PRIu8"%"PRIi32), rd, rs1, offset);
					emu->Registers[rd] = memory_lw(emu->Registers[rs1] + offset);
					break;

				case 4:
					/* LBU */
					EMU_LOG(PSTR("lbu r%"PRIu8", r%"PRIu8"%"PRIi32), rd, rs1, offset);
					emu->Registers[rd] = memory_lbu(emu->Registers[rs1] + offset);
					break;

				case 5:
					/* LHU */
					EMU_LOG(PSTR("lhu r%"PRIu8", r%"PRIu8"%"PRIi32), rd, rs1, offset);
					emu->Registers[rd] = memory_lhu(emu->Registers[rs1] + offset);
					break;

				default:
					EMU_LOG(PSTR("INVALID"));
					return true;
			}
			break;
		}

		case OPCODE_IMM:
		{
			u8 rd, rs1, funct3;
			u32 imm;
			rd = (instr >> 7) & 0x1F;
			funct3 = (instr >> 12) & 0x07;
			rs1 = (instr >> 15) & 0x1F;
			imm = (instr >> 20);
			switch(funct3)
			{
				case 0:
					/* ADDI */
					imm = sext(12, imm);
					EMU_LOG(PSTR("addi r%"PRIu8", r%"PRIu8", %"PRIi32), rd, rs1, imm);
					emu->Registers[rd] = emu->Registers[rs1] + imm;
					break;

				case 1:
					/* SLLI */
					imm &= 0x1F;
					EMU_LOG(PSTR("slli r%"PRIu8", r%"PRIu8", %"PRIi32), rd, rs1, imm);
					emu->Registers[rd] = emu->Registers[rs1] << imm;
					break;

				case 2:
					/* SLTI */
					imm = sext(12, imm);
					EMU_LOG(PSTR("slti r%"PRIu8", r%"PRIu8", %"PRIi32), rd, rs1, imm);
					emu->Registers[rd] = (i32)emu->Registers[rs1] < (i32)imm;
					break;

				case 3:
					/* SLTIU */
					EMU_LOG(PSTR("sltiu r%"PRIu8", r%"PRIu8", %"PRIi32), rd, rs1, imm);
					emu->Registers[rd] = emu->Registers[rs1] < imm;
					break;

				case 4:
					/* XORI */
					imm = sext(12, imm);
					EMU_LOG(PSTR("xori r%"PRIu8", r%"PRIu8", 0x%08"PRIx32), rd, rs1, imm);
					emu->Registers[rd] = emu->Registers[rs1] ^ imm;
					break;

				case 5:
					if(imm >> 5)
					{
						/* SRAI */
						imm &= 0x1F;
						EMU_LOG(PSTR("srai r%"PRIu8", r%"PRIu8", %"PRIi32), rd, rs1, imm);
						emu->Registers[rd] = (i32)emu->Registers[rs1] >> imm;
					}
					else
					{
						/* SRLI */
						imm &= 0x1F;
						EMU_LOG(PSTR("srli r%"PRIu8", r%"PRIu8", %"PRIi32), rd, rs1, imm);
						emu->Registers[rd] = emu->Registers[rs1] >> imm;
					}
					break;

				case 6:
					/* ORI */
					imm = sext(12, imm);
					EMU_LOG(PSTR("ori r%"PRIu8", r%"PRIu8", 0x%08"PRIx32), rd, rs1, imm);
					emu->Registers[rd] = emu->Registers[rs1] | imm;
					break;

				case 7:
					/* ANDI */
					imm = sext(12, imm);
					EMU_LOG(PSTR("andi r%"PRIu8", r%"PRIu8", 0x%08"PRIx32), rd, rs1, imm);
					emu->Registers[rd] = emu->Registers[rs1] & imm;
					break;
			}
			break;
		}

		case OPCODE_AUIPC:
		{
			u8 rd;
			u32 imm;
			rd = (instr >> 7) & 0x1F;
			imm = (instr >> 12);
			EMU_LOG(PSTR("auipc r%"PRIu8", 0x%08"PRIx32), rd, imm);
			emu->Registers[rd] = emu->PC + (imm << 12);
			break;
		}

		case OPCODE_STORE:
		{
			i32 offset;
			u32 imm;
			u8 rs1, rs2, funct3;
			funct3 = (instr >> 12) & 0x07;
			rs1 = (instr >> 15) & 0x1F;
			rs2 = (instr >> 20) & 0x1F;
			imm = ((instr & 0xF80) >> 7) | ((instr & 0xFE000000) >> 20);
			offset = sext(12, imm);
			switch(funct3)
			{
				case 0:
					/* SB */
					EMU_LOG(PSTR("sb r%"PRIu8"%"PRIi32", r%"PRIu8), rs1, offset, rs2);
					memory_sb(emu->Registers[rs1] + offset, emu->Registers[rs2]);
					break;

				case 1:
					/* SH */
					EMU_LOG(PSTR("sh r%"PRIu8"%"PRIi32", r%"PRIu8), rs1, offset, rs2);
					memory_sh(emu->Registers[rs1] + offset, emu->Registers[rs2]);
					break;

				case 2:
					/* SW */
					EMU_LOG(PSTR("sw r%"PRIu8"%"PRIi32", r%"PRIu8), rs1, offset, rs2);
					memory_sw(emu->Registers[rs1] + offset, emu->Registers[rs2]);
					break;

				default:
					EMU_LOG(PSTR("INVALID"));
					return true;
			}
			break;
		}

		case OPCODE_REG:
		{
			u8 rd, rs1, rs2, funct3, funct7;
			rd = (instr >> 7) & 0x1F;
			funct3 = (instr >> 12) & 0x07;
			rs1 = (instr >> 15) & 0x1F;
			rs2 = (instr >> 20) & 0x1F;
			funct7 = (instr >> 25);
			if(funct7 == 1)
			{
				/* Multiplication Extension */
				switch(funct3)
				{
					case 0:
						/* MUL */
						EMU_LOG(PSTR("mul r%"PRIu8", r%"PRIu8", r%"PRIu8), rd, rs1, rs2);
						emu->Registers[rd] = (i32)emu->Registers[rs1] * (i32)emu->Registers[rs2];
						break;

					case 1:
						/* MULH */
						EMU_LOG(PSTR("mulh r%"PRIu8", r%"PRIu8", r%"PRIu8), rd, rs1, rs2);
						emu->Registers[rd] = ((i64)emu->Registers[rs1] * (i64)emu->Registers[rs2]) >> 32;
						break;

					case 2:
						/* MULHSU */
						EMU_LOG(PSTR("mulhsu r%"PRIu8", r%"PRIu8", r%"PRIu8), rd, rs1, rs2);
						emu->Registers[rd] = ((i64)emu->Registers[rs1] * (u64)emu->Registers[rs2]) >> 32;
						break;

					case 3:
						/* MULHU */
						EMU_LOG(PSTR("mulhu r%"PRIu8", r%"PRIu8", r%"PRIu8), rd, rs1, rs2);
						emu->Registers[rd] = ((u64)emu->Registers[rs1] * (u64)emu->Registers[rs2]) >> 32;
						break;

					case 4:
						/* DIV */
						EMU_LOG(PSTR("div r%"PRIu8", r%"PRIu8", r%"PRIu8), rd, rs1, rs2);
						emu->Registers[rd] = (i32)emu->Registers[rs1] / (i32)emu->Registers[rs2];
						break;

					case 5:
						/* DIVU */
						EMU_LOG(PSTR("divu r%"PRIu8", r%"PRIu8", r%"PRIu8), rd, rs1, rs2);
						emu->Registers[rd] = emu->Registers[rs1] / emu->Registers[rs2];
						break;

					case 6:
						/* REM */
						EMU_LOG(PSTR("rem r%"PRIu8", r%"PRIu8", r%"PRIu8), rd, rs1, rs2);
						emu->Registers[rd] = (i32)emu->Registers[rs1] % (i32)emu->Registers[rs2];
						break;

					case 7:
						/* REMU */
						EMU_LOG(PSTR("remu r%"PRIu8", r%"PRIu8", r%"PRIu8), rd, rs1, rs2);
						emu->Registers[rd] = emu->Registers[rs1] % emu->Registers[rs2];
						break;
				}
			}
			else
			{
				switch(funct3)
				{
					case 0:
						if(funct7)
						{
							/* SUB */
							EMU_LOG(PSTR("sub r%"PRIu8", r%"PRIu8", r%"PRIu8), rd, rs1, rs2);
							emu->Registers[rd] = emu->Registers[rs1] - emu->Registers[rs2];
						}
						else
						{
							/* ADD */
							EMU_LOG(PSTR("add r%"PRIu8", r%"PRIu8", r%"PRIu8), rd, rs1, rs2);
							emu->Registers[rd] = emu->Registers[rs1] + emu->Registers[rs2];
						}
						break;

					case 1:
						/* SLL */
						EMU_LOG(PSTR("sll r%"PRIu8", r%"PRIu8", r%"PRIu8), rd, rs1, rs2);
						emu->Registers[rd] = emu->Registers[rs1] << (emu->Registers[rs2] & 0x1F);
						break;

					case 2:
						/* SLT */
						EMU_LOG(PSTR("slt r%"PRIu8", r%"PRIu8", r%"PRIu8), rd, rs1, rs2);
						emu->Registers[rd] = (i32)emu->Registers[rs1] < (i32)emu->Registers[rs2];
						break;

					case 3:
						/* SLTU */
						EMU_LOG(PSTR("sltu r%"PRIu8", r%"PRIu8", r%"PRIu8), rd, rs1, rs2);
						emu->Registers[rd] = emu->Registers[rs1] < emu->Registers[rs2];
						break;

					case 4:
						/* XOR */
						EMU_LOG(PSTR("xor r%"PRIu8", r%"PRIu8", r%"PRIu8), rd, rs1, rs2);
						emu->Registers[rd] = emu->Registers[rs1] ^ emu->Registers[rs2];
						break;

					case 5:
						if(funct7)
						{
							/* SRA */
							EMU_LOG(PSTR("srl r%"PRIu8", r%"PRIu8", r%"PRIu8), rd, rs1, rs2);
							emu->Registers[rd] = (i32)emu->Registers[rs1] >> (emu->Registers[rs2] & 0x1F);
						}
						else
						{
							/* SRL */
							EMU_LOG(PSTR("sra r%"PRIu8", r%"PRIu8", r%"PRIu8), rd, rs1, rs2);
							emu->Registers[rd] = emu->Registers[rs1] >> (emu->Registers[rs2] & 0x1F);
						}
						break;

					case 6:
						/* OR */
						EMU_LOG(PSTR("or r%"PRIu8", r%"PRIu8", r%"PRIu8), rd, rs1, rs2);
						emu->Registers[rd] = emu->Registers[rs1] | emu->Registers[rs2];
						break;

					case 7:
						/* AND */
						EMU_LOG(PSTR("and r%"PRIu8", r%"PRIu8", r%"PRIu8), rd, rs1, rs2);
						emu->Registers[rd] = emu->Registers[rs1] & emu->Registers[rs2];
						break;
				}
			}
			break;
		}

		case OPCODE_LUI:
		{
			u8 rd;
			u32 imm;
			rd = (instr >> 7) & 0x1F;
			imm = (instr >> 12);
			EMU_LOG(PSTR("lui r%"PRIu8", 0x%08"PRIx32), rd, imm);
			emu->Registers[rd] = (imm << 12);
			break;
		}

		case OPCODE_BRANCH:
		{
			i32 offset;
			u32 imm;
			u8 rs1, rs2, funct3;
			funct3 = (instr >> 12) & 0x07;
			rs1 = (instr >> 15) & 0x1F;
			rs2 = (instr >> 20) & 0x1F;
			imm = ((instr & 0xF00) >> 7) |
				((instr & 0x7E000000) >> 20) |
				((instr & 0x80) << 4) |
				((instr & 0x80000000) >> 20);

			offset = sext(12, imm);
			switch(funct3)
			{
				case 0:
					/* BEQ */
					EMU_LOG(PSTR("beq r%"PRIu8", r%"PRIu8", pc%+"PRIi32), rs1, rs2, offset);
					if(emu->Registers[rs1] == emu->Registers[rs2])
					{
						emu->PC += offset;
						return 0;
					}
					break;

				case 1:
					/* BNE */
					EMU_LOG(PSTR("bne r%"PRIu8", r%"PRIu8", pc%+"PRIi32), rs1, rs2, offset);
					if(emu->Registers[rs1] != emu->Registers[rs2])
					{
						emu->PC += offset;
						return 0;
					}
					break;

				case 4:
					/* BLT */
					EMU_LOG(PSTR("blt r%"PRIu8", r%"PRIu8", pc%+"PRIi32), rs1, rs2, offset);
					if((i32)emu->Registers[rs1] < (i32)emu->Registers[rs2])
					{
						emu->PC += offset;
						return 0;
					}
					break;

				case 5:
					/* BGE */
					EMU_LOG(PSTR("bge r%"PRIu8", r%"PRIu8", pc%+"PRIi32), rs1, rs2, offset);
					if((i32)emu->Registers[rs1] >= (i32)emu->Registers[rs2])
					{
						emu->PC += offset;
						return 0;
					}
					break;

				case 6:
					/* BLTU */
					EMU_LOG(PSTR("bltu r%"PRIu8", r%"PRIu8", pc%+"PRIi32), rs1, rs2, offset);
					if(emu->Registers[rs1] < emu->Registers[rs2])
					{
						emu->PC += offset;
						return 0;
					}
					break;

				case 7:
					/* BGEU */
					EMU_LOG(PSTR("bgeu r%"PRIu8", r%"PRIu8", pc%+"PRIi32), rs1, rs2, offset);
					if(emu->Registers[rs1] >= emu->Registers[rs2])
					{
						emu->PC += offset;
						return 0;
					}
					break;

				default:
					EMU_LOG(PSTR("INVALID"));
					return true;
			}
			break;
		}

		case OPCODE_JAL:
		{
			i32 offset;
			u32 imm;
			u8 rd;

			rd = (instr >> 7) & 0x1F;

			imm = ((instr & 0x80000000) >> 12) |
					(instr & 0xFF000) |
					((instr & 0x100000) >> 9) |
					((instr & 0x7FE00000) >> 20);

			offset = sext(20, imm);
			EMU_LOG(PSTR("jal r%"PRIu8", pc%+"PRIi32), rd, offset);
			emu->Registers[rd] = emu->PC + 4;
			emu->PC += offset;
			return 0;
		}

		case OPCODE_JALR:
		{
			u8 rd, rs1;
			i32 offset;
			u32 imm;
			rd = (instr >> 7) & 0x1F;
			rs1 = (instr >> 15) & 0x1F;
			imm = (instr >> 20) & ~1;

			offset = sext(12, imm);

			EMU_LOG(PSTR("jalr r%"PRIu8", r%"PRIu8"%+"PRIi32),
				rd, rs1, offset);

			emu->Registers[rd] = emu->PC + 4;
			emu->PC = emu->Registers[rs1] + offset;
			return 0;
		}

		default:
			EMU_LOG(PSTR("INVALID"));
			return true;
	}

	emu->PC += 4;
	return false;
}

void emulator_call(Emulator *emu, u32 addr, u32 *args, u8 num, u32 sp)
{
	/* Initialize Stack Pointer at end of memory */
	emu->Registers[2] = sp - 4;

	emu->PC = addr;
	memcpy(&emu->Registers[10], args, num * sizeof(*emu->Registers));

	u32 i = 0;
	_finished = false;
	for(;;)
	{
		if(emulator_next(emu))
		{
			return;
		}

		++i;
		if(_finished)
		{
			EMU_LOG(PSTR("%"PRIu32" Cycles"), i);
			return;
		}
	}
}
