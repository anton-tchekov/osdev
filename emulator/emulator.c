/**
 * @file    emulator.c
 * @author  Anton Tchekov
 * @version 0.1
 * @date    24.05.2023
 */

#include <types.h>
#include <platform.h>
#include <emulator.h>
#include <gfx-types.h>

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

/**
 * @brief Print all registers of the emulator
 *
 * @param emu The emulator
 */
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

/** Opcode for Load instructions */
#define OPCODE_LOAD   0x00

/** Opcodes for instructions with immediate values */
#define OPCODE_IMM    0x04

/** Opcode for AUIPC instruction */
#define OPCODE_AUIPC  0x05 /* Add Upper Immediate to Program Counter */

/** Opcode for store instructions */
#define OPCODE_STORE  0x08

/** Opcodes for instructions on registers */
#define OPCODE_REG    0x0C

/** Opcode for LUI instruction */
#define OPCODE_LUI    0x0D /* Load Upper Immediate */

/** Opcode for branch instructions */
#define OPCODE_BRANCH 0x18

/** Opcode for JALR instructions */
#define OPCODE_JALR   0x19 /* Jump and Link Register */

/** Opcode for JAL instructions */
#define OPCODE_JAL    0x1B /* Jump and Link */

/** Opcode for ECALL instructions */
#define OPCODE_ECALL  0x1C /* Environment Call */

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

/** Called function finished flag */
static bool _finished;

/**/
static Emulator _cur, *_emu = &_cur;

/** Total size of the memory in bytes */
static u32 _memory_size;

void kernel_init(void)
{
	_memory_size = env_memory_size();
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
}

/**
 * @brief Checks if a render window is out ot bounds
 *
 * @param x X-Coordinate
 * @param y Y-Coordinate
 * @param w Width
 * @param h Height
 * @return Non-zero if invalid
 */
static u8 _gfx_check_bounds(u16 x, u16 y, u16 w, u16 h)
{
	return (x >= GFX_WIDTH) ||
		(y >= GFX_HEIGHT) ||
		(w > GFX_WIDTH) ||
		(h > GFX_HEIGHT) ||
		(x + w > GFX_WIDTH) ||
		(y + h > GFX_HEIGHT);
}

/**
 * @brief Check memory location permissions
 *
 * @param addr
 * @param size
 * @return Non-zero if invalid
 */
static u8 _memory_check_bounds(u32 addr, u32 size)
{
	return 0;
/*
	return (addr < _emu->SegmentStart) ||
		(addr >= _emu->SegmentEnd) ||
		(size >= _emu->SegmentSize) ||
		(addr + size >= _emu->SegmentEnd);*/
}

/* --- Store --- */

/**
 * @brief Store byte
 *
 * @param addr The address at which to store the value
 * @param value The value to store
 * @return Non-zero if out ouf bounds
 */
static u8 memory_sb(u32 addr, u32 value)
{
	i8 value8 = (i8)value;
	if(_memory_check_bounds(addr, sizeof(value8)))
	{
		return 1;
	}

	env_memory_write(addr, &value8, sizeof(value8));
	return 0;
}

/**
 * @brief Store 16-bit half-word
 *
 * @param addr The address at which to store the value
 * @param value The value to store
 * @return Non-zero if out ouf bounds
 */
static u8 memory_sh(u32 addr, u32 value)
{
	i16 value16 = (i16)value;
	if(_memory_check_bounds(addr, sizeof(value16)))
	{
		return 1;
	}

	env_memory_write(addr, &value16, sizeof(value16));
	return 0;
}

/**
 * @brief Store 32-bit word
 *
 * @param addr The address at which to store the value
 * @param value The value to store
 * @return Non-zero if out ouf bounds
 */
static u8 memory_sw(u32 addr, u32 value)
{
	if(_memory_check_bounds(addr, sizeof(value)))
	{
		return 1;
	}

	env_memory_write(addr, &value, sizeof(value));
	return 0;
}

/* --- Load --- */
static u8 memory_lb(u32 addr, u32 *out)
{
	i8 value8;
	if(_memory_check_bounds(addr, sizeof(value8)))
	{
		return 1;
	}

	env_memory_read(addr, &value8, sizeof(value8));
	*out = (i32)value8;
	return 0;
}

static u8 memory_lh(u32 addr, u32 *out)
{
	i16 value16;
	if(_memory_check_bounds(addr, sizeof(value16)))
	{
		return 1;
	}

	env_memory_read(addr, &value16, sizeof(value16));
	*out = (i32)value16;
	return 0;
}

static u8 memory_lw(u32 addr, u32 *out)
{
	u32 value;
	if(_memory_check_bounds(addr, sizeof(value)))
	{
		return 1;
	}

	env_memory_read(addr, &value, sizeof(value));
	*out = value;
	return 0;
}

static u8 memory_lbu(u32 addr, u32 *out)
{
	u8 value8;
	if(_memory_check_bounds(addr, sizeof(value8)))
	{
		return 1;
	}

	env_memory_read(addr, &value8, sizeof(value8));
	*out = value8;
	return 0;
}

static u8 memory_lhu(u32 addr, u32 *out)
{
	u16 value16;
	if(_memory_check_bounds(addr, sizeof(value16)))
	{
		return 1;
	}

	env_memory_read(addr, &value16, sizeof(value16));
	*out = value16;
	return 0;
}

/* --- Syscalls --- */
static u8 syscall_exit(u32 *args)
{
	/* No parameter checking necessary */
	/* TODO */
	return 0;
	(void)args;
}

static u8 syscall_finish(u32 *args)
{
	/* No parameter checking necessary */
	_finished = true;
	return 0;
	(void)args;
}

static u8 syscall_event_register(u32 *args)
{
	u32 type = args[0];
	u32 addr = args[1];
	if(type < EVENT_COUNT)
	{
		EMU_LOG(PSTR("Registered Event Type: %"PRIu32" - Addr: %"PRIu32), type, addr);
		_emu->Events[type] = addr;
	}

	return 0;
}

static u8 syscall_millis(u32 *args)
{
	/* No parameter checking necessary */
	args[0] = env_millis();
	return 0;
}

static u8 syscall_rand(u32 *args)
{
	/* No parameter checking necessary */
	args[0] = env_random_get();
	return 0;
}

static u8 syscall_serial_write(u32 *args)
{
	u16 cur;
	u32 ptr = args[0];
	u32 len = args[1];
	u8 buf[128];

	while(len)
	{
		cur = len > sizeof(buf) ? sizeof(buf) : len;
		env_memory_read(ptr, buf, cur);
		env_serial_write(buf, cur);
		len -= cur;
		ptr += cur;
	}

	return 0;
}

static u8 syscall_gfx_rect(u32 *args)
{
	u16 x, y, w, h;
	u32 color;

	x = args[0];
	y = args[1];
	w = args[2];
	h = args[3];
	if(_gfx_check_bounds(x, y, w, h))
	{
		return 1;
	}

	color = args[4];
	env_gfx_rect(x, y, w, h, color);
	return 0;
}

static u8 syscall_gfx_image_rgba(u32 *args)
{
	u16 x, y, w, h;
	u32 image, bytes;

	x = args[0];
	y = args[1];
	w = args[2];
	h = args[3];
	if(_gfx_check_bounds(x, y, w, h))
	{
		return 1;
	}

	image = args[4];
	bytes = (u32)4 * (u32)w * (u32)h;
	if(_memory_check_bounds(image, bytes))
	{
		return 1;
	}

	env_gfx_image_rgba(x, y, w, h, image);
	return 0;
}

static u8 syscall_gfx_image_rgb(u32 *args)
{
	u16 x, y, w, h;
	u32 image, bytes;

	x = args[0];
	y = args[1];
	w = args[2];
	h = args[3];
	if(_gfx_check_bounds(x, y, w, h))
	{
		return 1;
	}

	image = args[4];
	bytes = (u32)3 * (u32)w * (u32)h;
	if(_memory_check_bounds(image, bytes))
	{
		return 1;
	}

	env_gfx_image_rgb(x, y, w, h, image);
	return 0;
}

static u8 syscall_gfx_image_rgb565(u32 *args)
{
	u32 image, bytes;
	u16 x, y, w, h;

	x = args[0];
	y = args[1];
	w = args[2];
	h = args[3];
	if(_gfx_check_bounds(x, y, w, h))
	{
		return 1;
	}

	image = args[4];
	bytes = (u32)2 * (u32)w * (u32)h;
	if(_memory_check_bounds(image, bytes))
	{
		return 1;
	}

	env_gfx_image_rgb565(x, y, w, h, image);
	return 0;
}

static u8 syscall_gfx_image_grayscale(u32 *args)
{
	u16 x, y, w, h;
	u32 image, bytes, rect_addr, fg, bg;
	Rectangle rect;

	rect_addr = args[0];
	if(_memory_check_bounds(rect_addr, sizeof(rect)))
	{
		return 1;
	}

	env_memory_read(rect_addr, &rect, sizeof(rect));
	x = rect.X;
	y = rect.Y;
	w = rect.W;
	h = rect.H;
	if(_gfx_check_bounds(x, y, w, h))
	{
		return 1;
	}

	image = args[1];
	bytes = (u32)w * (u32)h;
	if(_memory_check_bounds(image, bytes))
	{
		return 1;
	}

	fg = args[2];
	bg = args[3];
	env_gfx_image_grayscale(x, y, w, h, image, fg, bg);
	return 0;
}

static u8 syscall_gfx_image_1bit(u32 *args)
{
	/* TODO: Check params */
	Rectangle r;
	env_memory_read(args[0], &r, sizeof(r));
	env_gfx_image_1bit(r.X, r.Y, r.W, r.H, args[1], args[2], args[3]);
	return 0;
}

/** System call function pointer array */
static u8 (*syscalls[])(u32 *) =
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
	syscall_serial_write,
	syscall_millis
};

static u8 syscall(u32 id, u32 *args)
{
	if(id >= ARRLEN(syscalls))
	{
		return 1;
	}

	return syscalls[id](args);
}

/* --- EMULATOR --- */
static u8 emulator_next(Emulator *emu)
{
	u32 instr, opcode;
	if(memory_lw(emu->PC, &instr))
	{
		return 1;
	}

	/* r0 = Zero Register */
	emu->Registers[0] = 0;

	/* bits 7 .. 0 */
	opcode = instr & 0x7F;
	if((opcode & 0x03) != 0x03)
	{
		EMU_LOG(PSTR("INVALID"));
		return 1;
	}

	opcode >>= 2;
	switch(opcode)
	{
		case OPCODE_ECALL:
			if(syscall(emu->Registers[17], &emu->Registers[10]))
			{
				EMU_LOG(PSTR("SYSCALL ERROR"));
				return 1;
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
					if(memory_lb(emu->Registers[rs1] + offset, &emu->Registers[rd]))
					{
						return 1;
					}
					break;

				case 1:
					/* LH */
					EMU_LOG(PSTR("lh r%"PRIu8", r%"PRIu8"%+"PRIi32), rd, rs1, offset);
					if(memory_lh(emu->Registers[rs1] + offset, &emu->Registers[rd]))
					{
						return 1;
					}
					break;

				case 2:
					/* LW */
					EMU_LOG(PSTR("lw r%"PRIu8", r%"PRIu8"%+"PRIi32), rd, rs1, offset);
					if(memory_lw(emu->Registers[rs1] + offset, &emu->Registers[rd]))
					{
						return 1;
					}
					break;

				case 4:
					/* LBU */
					EMU_LOG(PSTR("lbu r%"PRIu8", r%"PRIu8"%+"PRIi32), rd, rs1, offset);
					if(memory_lbu(emu->Registers[rs1] + offset, &emu->Registers[rd]))
					{
						return 1;
					}
					break;

				case 5:
					/* LHU */
					EMU_LOG(PSTR("lhu r%"PRIu8", r%"PRIu8"%+"PRIi32), rd, rs1, offset);
					if(memory_lhu(emu->Registers[rs1] + offset, &emu->Registers[rd]))
					{
						return 1;
					}
					break;

				default:
					EMU_LOG(PSTR("INVALID"));
					return 1;
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
					EMU_LOG(PSTR("sb r%"PRIu8"%+"PRIi32", r%"PRIu8), rs1, offset, rs2);
					if(memory_sb(emu->Registers[rs1] + offset, emu->Registers[rs2]))
					{
						return 1;
					}
					break;

				case 1:
					/* SH */
					EMU_LOG(PSTR("sh r%"PRIu8"%+"PRIi32", r%"PRIu8), rs1, offset, rs2);
					if(memory_sh(emu->Registers[rs1] + offset, emu->Registers[rs2]))
					{
						return 1;
					}
					break;

				case 2:
					/* SW */
					EMU_LOG(PSTR("sw r%"PRIu8"%+"PRIi32", r%"PRIu8), rs1, offset, rs2);
					if(memory_sw(emu->Registers[rs1] + offset, emu->Registers[rs2]))
					{
						return 1;
					}
					break;

				default:
					EMU_LOG(PSTR("INVALID"));
					return 1;
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
					return 1;
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
			return 1;
	}

	emu->PC += 4;
	return 0;
}

u8 emulator_call(Emulator *emu, u32 addr, u32 *args, u8 num, u32 sp)
{
	u32 i;

	/* Initialize Stack Pointer at end of memory */
	emu->Registers[2] = sp - 4;
	emu->PC = addr;
	memcpy(&emu->Registers[10], args, num * sizeof(*emu->Registers));
	i = 0;
	_finished = false;
	while(!_finished)
	{
		++i;
		if(emulator_next(emu))
		{
			return 1;
		}
	}

	EMU_LOG(PSTR("%"PRIu32" Cycles"), i);
	return 0;
}
