#include <types.h>
#include <platform.h>
#include <emulator.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>
#include <util.h>

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
 * @brief Sign extend
 *
 * @param bits
 * @param value
 * @return u32
 */
static inline u32 sext(u32 bits, u32 value)
{
	u32 m = 1 << (bits - 1);
	return (value ^ m) - m;
}

static bool _finished;
static Emulator _cur, *_emu = &_cur;
static bool _keys[KEY_COUNT];
static u32 _memory_size;

void kernel_init(void)
{
	_memory_size = memory_size();
}

void process_setup(void)
{
	emulator_call(_emu, 0, NULL, 0, _memory_size, 1000000);
}

void os_update(void)
{
	u32 addr = _emu->Events[EVENT_LOOP];
	if(addr)
	{
		emulator_call(_emu, addr, NULL, 0, _memory_size, 1000000);
	}
}

u32 syscall_event_register(u32 *args)
{
	u32 type = args[0];
	u32 addr = args[1];
	if(type < EVENT_COUNT)
	{
		_emu->Events[type] = addr;
	}

	return 0;
}

void keyboard_event(Key key, i32 chr, KeyState down)
{
	u32 addr = _emu->Events[EVENT_KEY];
	if(addr)
	{
		u32 args[] = { key, chr, down };
		emulator_call(_emu, addr, args, ARRLEN(args), _memory_size, 1000000);
	}

	key = key_mod_remove(key);
	if(key >= KEY_COUNT)
	{
		return;
	}

	_keys[key] = down;
}

static bool keyboard_is_key_pressed(Key key)
{
	if(key >= KEY_COUNT)
	{
		return false;
	}

	return _keys[key];
}

/* TODO: Paging/MProt */

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

u32 syscall_exit(u32 *args)
{
	/* Remove process from scheduler */

	/* If init process, crash system */

	/* TODO: We only have one process right now */
	exit(args[0]);
	return 0;
}

static u32 syscall_finish(u32 *args)
{
	_finished = true;
	return 0;
	(void)args;
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

u32 syscall_rand(u32 *args)
{
	return random_get();
	(void)args;
}

u32 syscall_keyboard_is_key_pressed(u32 *args)
{
	return keyboard_is_key_pressed(args[0]);
}

u32 syscall_serial_write(u32 *args)
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
#ifdef DEBUG

void emulator_dump_registers(Emulator *emu)
{
	printf("PC  0x%08X %10d\n", emu->PC, emu->PC);
	for(i32 i = 0; i < 32; ++i)
	{
		printf("R%d%s 0x%08X %10d\n", i, i < 10 ? " " : "",
			emu->Registers[i], emu->Registers[i]);
	}

	printf("\n");
}

#endif

static i32 emulator_next(Emulator *emu)
{
#ifdef DEBUG
	clock_t begin, end;
	double time_spent;

	begin = clock();
#endif

	u32 instr, opcode;
	instr = memory_lw(emu->PC);

	/* r0 = Zero Register */
	emu->Registers[0] = 0;

	/* bits 7 .. 0 */
	opcode = instr & 0x7F;
	if((opcode & 0x03) != 0x03)
	{
		/* INVALID */
#ifdef DEBUG
		printf("INVALID\n");
#endif
		return -1;
	}

	opcode >>= 2;
	switch(opcode)
	{
		case OPCODE_ECALL:
			if(syscall(emu->Registers[17], &emu->Registers[10]))
			{
#ifdef DEBUG
				printf("SYSCALL ERROR\n");
#endif
				return -1;
			}
			break;

		case OPCODE_LOAD:
		{
			i32 offset;
			u32 rd, rs1, funct3, imm;
			rd = (instr >> 7) & 0x1F;
			funct3 = (instr >> 12) & 0x07;
			rs1 = (instr >> 15) & 0x1F;
			imm = (instr >> 20);
			offset = sext(12, imm);
			switch(funct3)
			{
				case 0:
					/* LB */
#ifdef DEBUG
					printf("lb r%d, r%d%+d\n", rd, rs1, offset);
#endif
					emu->Registers[rd] = memory_lb(emu->Registers[rs1] + offset);
					break;

				case 1:
					/* LH */
#ifdef DEBUG
					printf("lh r%d, r%d%+d\n", rd, rs1, offset);
#endif
					emu->Registers[rd] = memory_lh(emu->Registers[rs1] + offset);
					break;

				case 2:
					/* LW */
#ifdef DEBUG
					printf("lw r%d, r%d%+d\n", rd, rs1, offset);
#endif
					emu->Registers[rd] = memory_lw(emu->Registers[rs1] + offset);
					break;

				case 4:
					/* LBU */
#ifdef DEBUG
					printf("lbu r%d, r%d%+d\n", rd, rs1, offset);
#endif
					emu->Registers[rd] = memory_lbu(emu->Registers[rs1] + offset);
					break;

				case 5:
					/* LHU */
#ifdef DEBUG
					printf("lhu r%d, r%d%+d\n", rd, rs1, offset);
#endif
					emu->Registers[rd] = memory_lhu(emu->Registers[rs1] + offset);
					break;

				default:
					/* INVALID */
#ifdef DEBUG
					printf("INVALID\n");
#endif
					return -1;
			}
			break;
		}

		case OPCODE_IMM:
		{
			u32 rd, rs1, funct3, imm;
			rd = (instr >> 7) & 0x1F;
			funct3 = (instr >> 12) & 0x07;
			rs1 = (instr >> 15) & 0x1F;
			imm = (instr >> 20);
			switch(funct3)
			{
				case 0:
					/* ADDI */
					imm = sext(12, imm);
#ifdef DEBUG
					printf("addi r%d, r%d, %d\n", rd, rs1, imm);
#endif
					emu->Registers[rd] = emu->Registers[rs1] + imm;
					break;

				case 1:
					/* SLLI */
					imm &= 0x1F;
#ifdef DEBUG
					printf("slli r%d, r%d, %d\n", rd, rs1, imm);
#endif
					emu->Registers[rd] = emu->Registers[rs1] << imm;
					break;

				case 2:
					/* SLTI */
					imm = sext(12, imm);
#ifdef DEBUG
					printf("slti r%d, r%d, %d\n", rd, rs1, imm);
#endif
					emu->Registers[rd] = (i32)emu->Registers[rs1] < (i32)imm;
					break;

				case 3:
					/* SLTIU */
#ifdef DEBUG
					printf("sltiu r%d, r%d, %d\n", rd, rs1, imm);
#endif
					emu->Registers[rd] = emu->Registers[rs1] < imm;
					break;

				case 4:
					/* XORI */
					imm = sext(12, imm);
#ifdef DEBUG
					printf("xori r%d, r%d, 0x%08X\n", rd, rs1, imm);
#endif
					emu->Registers[rd] = emu->Registers[rs1] ^ imm;
					break;

				case 5:
					if(imm >> 5)
					{
						/* SRAI */
						imm &= 0x1F;
#ifdef DEBUG
						printf("srai r%d, r%d, %d\n", rd, rs1, imm);
#endif
						emu->Registers[rd] = (i32)emu->Registers[rs1] >> imm;
					}
					else
					{
						/* SRLI */
						imm &= 0x1F;
#ifdef DEBUG
						printf("srli r%d, r%d, %d\n", rd, rs1, imm);
#endif
						emu->Registers[rd] = emu->Registers[rs1] >> imm;
					}
					break;

				case 6:
					/* ORI */
					imm = sext(12, imm);
#ifdef DEBUG
					printf("ori r%d, r%d, 0x%08X\n", rd, rs1, imm);
#endif
					emu->Registers[rd] = emu->Registers[rs1] | imm;
					break;

				case 7:
					/* ANDI */
					imm = sext(12, imm);
#ifdef DEBUG
					printf("andi r%d, r%d, 0x%08X\n", rd, rs1, imm);
#endif
					emu->Registers[rd] = emu->Registers[rs1] & imm;
					break;
			}
			break;
		}

		case OPCODE_AUIPC:
		{
			u32 rd, imm;
			rd = (instr >> 7) & 0x1F;
			imm = (instr >> 12);
#ifdef DEBUG
			printf("auipc r%d, 0x%08X\n", rd, imm);
#endif
			emu->Registers[rd] = emu->PC + (imm << 12);
			break;
		}

		case OPCODE_STORE:
		{
			i32 offset;
			u32 imm, rs1, rs2, funct3;
			funct3 = (instr >> 12) & 0x07;
			rs1 = (instr >> 15) & 0x1F;
			rs2 = (instr >> 20) & 0x1F;
			imm = ((instr & 0xF80) >> 7) | ((instr & 0xFE000000) >> 20);
			offset = sext(12, imm);
			switch(funct3)
			{
				case 0:
					/* SB */
#ifdef DEBUG
					printf("sb r%d%+d, r%d\n", rs1, offset, rs2);
#endif
					memory_sb(emu->Registers[rs1] + offset, emu->Registers[rs2]);
					break;

				case 1:
					/* SH */
#ifdef DEBUG
					printf("sh r%d%+d, r%d\n", rs1, offset, rs2);
#endif
					memory_sh(emu->Registers[rs1] + offset, emu->Registers[rs2]);
					break;

				case 2:
					/* SW */
#ifdef DEBUG
					printf("sw r%d%+d, r%d\n", rs1, offset, rs2);
#endif
					memory_sw(emu->Registers[rs1] + offset, emu->Registers[rs2]);
					break;

				default:
					/* INVALID */
#ifdef DEBUG
					printf("INVALID\n");
#endif
					return -1;
			}
			break;
		}

		case OPCODE_REG:
		{
			u32 rd, rs1, rs2, funct3, funct7;
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
#ifdef DEBUG
						printf("mul r%d, r%d, r%d\n", rd, rs1, rs2);
#endif
						emu->Registers[rd] = (i32)emu->Registers[rs1] * (i32)emu->Registers[rs2];
						break;

					case 1:
						/* MULH */
#ifdef DEBUG
						printf("mulh r%d, r%d, r%d\n", rd, rs1, rs2);
#endif
						emu->Registers[rd] = ((i64)emu->Registers[rs1] * (i64)emu->Registers[rs2]) >> 32;
						break;

					case 2:
						/* MULHSU */
#ifdef DEBUG
						printf("mulhsu r%d, r%d, r%d\n", rd, rs1, rs2);
#endif
						emu->Registers[rd] = ((i64)emu->Registers[rs1] * (u64)emu->Registers[rs2]) >> 32;
						break;

					case 3:
						/* MULHU */
#ifdef DEBUG
						printf("mulhu r%d, r%d, r%d\n", rd, rs1, rs2);
#endif
						emu->Registers[rd] = ((u64)emu->Registers[rs1] * (u64)emu->Registers[rs2]) >> 32;
						break;

					case 4:
						/* DIV */
#ifdef DEBUG
						printf("div r%d, r%d, r%d\n", rd, rs1, rs2);
#endif
						emu->Registers[rd] = (i32)emu->Registers[rs1] / (i32)emu->Registers[rs2];
						break;

					case 5:
						/* DIVU */
#ifdef DEBUG
						printf("divu r%d, r%d, r%d\n", rd, rs1, rs2);
#endif
						emu->Registers[rd] = emu->Registers[rs1] / emu->Registers[rs2];
						break;

					case 6:
						/* REM */
#ifdef DEBUG
						printf("rem r%d, r%d, r%d\n", rd, rs1, rs2);
#endif
						emu->Registers[rd] = (i32)emu->Registers[rs1] % (i32)emu->Registers[rs2];
						break;

					case 7:
						/* REMU */
#ifdef DEBUG
						printf("remu r%d, r%d, r%d\n", rd, rs1, rs2);
#endif
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
#ifdef DEBUG
							printf("sub r%d, r%d, r%d\n", rd, rs1, rs2);
#endif
							emu->Registers[rd] = emu->Registers[rs1] - emu->Registers[rs2];
						}
						else
						{
							/* ADD */
#ifdef DEBUG
							printf("add r%d, r%d, r%d\n", rd, rs1, rs2);
#endif
							emu->Registers[rd] = emu->Registers[rs1] + emu->Registers[rs2];
						}
						break;

					case 1:
						/* SLL */
#ifdef DEBUG
						printf("sll r%d, r%d, r%d\n", rd, rs1, rs2);
#endif
						emu->Registers[rd] = emu->Registers[rs1] << (emu->Registers[rs2] & 0x1F);
						break;

					case 2:
						/* SLT */
#ifdef DEBUG
						printf("slt r%d, r%d, r%d\n", rd, rs1, rs2);
#endif
						emu->Registers[rd] = (i32)emu->Registers[rs1] < (i32)emu->Registers[rs2];
						break;

					case 3:
						/* SLTU */
#ifdef DEBUG
						printf("sltu r%d, r%d, r%d\n", rd, rs1, rs2);
#endif
						emu->Registers[rd] = emu->Registers[rs1] < emu->Registers[rs2];
						break;

					case 4:
						/* XOR */
#ifdef DEBUG
						printf("xor r%d, r%d, r%d\n", rd, rs1, rs2);
#endif
						emu->Registers[rd] = emu->Registers[rs1] ^ emu->Registers[rs2];
						break;

					case 5:
						if(funct7)
						{
							/* SRA */
#ifdef DEBUG
							printf("srl r%d, r%d, r%d\n", rd, rs1, rs2);
#endif
							emu->Registers[rd] = (i32)emu->Registers[rs1] >> (emu->Registers[rs2] & 0x1F);
						}
						else
						{
							/* SRL */
#ifdef DEBUG
							printf("sra r%d, r%d, r%d\n", rd, rs1, rs2);
#endif
							emu->Registers[rd] = emu->Registers[rs1] >> (emu->Registers[rs2] & 0x1F);
						}
						break;

					case 6:
						/* OR */
#ifdef DEBUG
						printf("or r%d, r%d, r%d\n", rd, rs1, rs2);
#endif
						emu->Registers[rd] = emu->Registers[rs1] | emu->Registers[rs2];
						break;

					case 7:
						/* AND */
#ifdef DEBUG
						printf("and r%d, r%d, r%d\n", rd, rs1, rs2);
#endif
						emu->Registers[rd] = emu->Registers[rs1] & emu->Registers[rs2];
						break;
				}
			}
			break;
		}

		case OPCODE_LUI:
		{
			u32 rd, imm;
			rd = (instr >> 7) & 0x1F;
			imm = (instr >> 12);
#ifdef DEBUG
			printf("lui r%d, 0x%08X\n", rd, imm);
#endif
			emu->Registers[rd] = (imm << 12);
			break;
		}

		case OPCODE_BRANCH:
		{
			i32 offset;
			u32 imm, rs1, rs2, funct3;

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
#ifdef DEBUG
					printf("beq r%d, r%d, pc%+d\n", rs1, rs2, offset);
#endif
					if(emu->Registers[rs1] == emu->Registers[rs2])
					{
						emu->PC += offset;
						return 0;
					}
					break;

				case 1:
					/* BNE */
#ifdef DEBUG
					printf("bne r%d, r%d, pc%+d\n", rs1, rs2, offset);
#endif
					if(emu->Registers[rs1] != emu->Registers[rs2])
					{
						emu->PC += offset;
						return 0;
					}
					break;

				case 4:
					/* BLT */
#ifdef DEBUG
					printf("blt r%d, r%d, pc%+d\n", rs1, rs2, offset);
#endif
					if((i32)emu->Registers[rs1] < (i32)emu->Registers[rs2])
					{
						emu->PC += offset;
						return 0;
					}
					break;

				case 5:
					/* BGE */
#ifdef DEBUG
					printf("bge r%d, r%d, pc%+d\n", rs1, rs2, offset);
#endif
					if((i32)emu->Registers[rs1] >= (i32)emu->Registers[rs2])
					{
						emu->PC += offset;
						return 0;
					}
					break;

				case 6:
					/* BLTU */
#ifdef DEBUG
					printf("bltu r%d, r%d, pc%+d\n", rs1, rs2, offset);
#endif
					if(emu->Registers[rs1] < emu->Registers[rs2])
					{
						emu->PC += offset;
						return 0;
					}
					break;

				case 7:
					/* BGEU */
#ifdef DEBUG
					printf("bgeu r%d, r%d, pc%+d\n", rs1, rs2, offset);
#endif
					if(emu->Registers[rs1] >= emu->Registers[rs2])
					{
						emu->PC += offset;
						return 0;
					}
					break;

				default:
					/* INVALID */
#ifdef DEBUG
					printf("INVALID\n");
#endif
					return -1;
			}
			break;
		}

		case OPCODE_JAL:
		{
			i32 offset;
			u32 rd, imm;
			rd = (instr >> 7) & 0x1F;

			imm = ((instr & 0x80000000) >> 12) |
					(instr & 0xFF000) |
					((instr & 0x100000) >> 9) |
					((instr & 0x7FE00000) >> 20);

			offset = sext(20, imm);
#ifdef DEBUG
			printf("jal r%d, pc%+d\n", rd, offset);
#endif
			emu->Registers[rd] = emu->PC + 4;
			emu->PC += offset;
			return 0;
		}

		case OPCODE_JALR:
		{
			i32 offset;
			u32 rd, rs1, imm;
			rd = (instr >> 7) & 0x1F;
			rs1 = (instr >> 15) & 0x1F;
			imm = (instr >> 20) & ~1;

			offset = sext(12, imm);
#ifdef DEBUG
			printf("jalr r%d, r%d%+d\n", rd, rs1, offset);
#endif
			emu->Registers[rd] = emu->PC + 4;
			emu->PC = emu->Registers[rs1] + offset;
			return 0;
		}

		default:
			/* INVALID */
#ifdef DEBUG
			printf("INVALID\n");
#endif
			return -1;
	}

#ifdef DEBUG
	end = clock();
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("time spent = %f\n", time_spent);
#endif

	emu->PC += 4;
	return 0;
}

#define BOLDRED   "\033[1m\033[31m"
#define RESET     "\033[0m"

i32 emulator_call(Emulator *emu, u32 addr, u32 *args, u32 num, u32 sp, u32 max_instr)
{
	u32 i;

	/* Initialize Stack Pointer at end of memory */
	emu->Registers[2] = sp;

	emu->PC = addr;
	memcpy(&emu->Registers[10], args, num * sizeof(*emu->Registers));

	_finished = false;
	for(i = 0; i < max_instr; ++i)
	{
		emulator_next(emu);
		if(_finished)
		{
			return 0;
		}
	}

	/* TODO */
	fprintf(stderr, BOLDRED "INSTRUCTION LIMIT EXCEEDED!\n" RESET);
	exit(1);
	return 1;
}
