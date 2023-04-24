#include "types.h"
#include "memory.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARRLEN(A) (sizeof(A) / sizeof(*A))

/* OPCODES */
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

typedef struct
{
	u32 PC;
	u32 Registers[32];
} Emulator;

static inline u32 sext(u32 bits, u32 value)
{
	u32 m = 1 << (bits - 1);
	return (value ^ m) - m;
}

static void registers_dump(Emulator *emu)
{
	printf("PC  0x%08X %10d\n", emu->PC, emu->PC);
	for(i32 i = 0; i < 32; ++i)
	{
		printf("R%d%s 0x%08X %10d\n", i, i < 10 ? " " : "",  emu->Registers[i], emu->Registers[i]);
	}

	printf("\n");
}

static u32 syscall_exit(u32 *args)
{
	exit(args[0]);
	return 0;
}

/* MEM */
static u32 syscall_memcpy(u32 *args)
{
	return (u8 *)memcpy(_memory + args[0], _memory + args[1], args[2]) - _memory;
}

static u32 syscall_memmove(u32 *args)
{
	return (u8 *)memmove(_memory + args[0], _memory + args[1], args[2]) - _memory;
}

static u32 syscall_memcmp(u32 *args)
{
	return memcmp(_memory + args[0], _memory + args[1], args[2]);
}

static u32 syscall_memchr(u32 *args)
{
	return (u8 *)memchr(_memory + args[0], args[1], args[2]) - _memory;
}

static u32 syscall_memset(u32 *args)
{
	return (u8 *)memset(_memory + args[0], args[1], args[2]) - _memory;
}

/* STR */
static u32 syscall_strcpy(u32 *args)
{
	return strcpy((char *)(_memory + args[0]), (char *)(_memory + args[1])) - (char *)_memory;
}

static u32 syscall_strncpy(u32 *args)
{
	return strncpy((char *)(_memory + args[0]), (char *)(_memory + args[1]), args[2]) - (char *)_memory;
}

static u32 syscall_strlen(u32 *args)
{
	return strlen((char *)(_memory + args[0]));
}

static u32 syscall_strcmp(u32 *args)
{
	return strcmp((char *)(_memory + args[0]), (char *)(_memory + args[1]));
}

static u32 syscall_strncmp(u32 *args)
{
	return strncmp((char *)(_memory + args[0]), (char *)(_memory + args[1]), args[2]);
}

static u32 syscall_strchr(u32 *args)
{
	return strchr((char *)(_memory + args[0]), args[1]) - (char *)_memory;
}

/* RAND */
static u32 syscall_rand(u32 *args)
{
	return rand();
	(void)args;
}

/* GFX */
static u32 syscall_gfx_rect(u32 *args)
{
	printf("DRAW RECT (X: %4d, Y: %4d, W: %4d, H: %4d, COLOR: 0x%08X)\n",
		args[0], args[1], args[2], args[3], args[4]);
	return 0;
}

static u32 syscall_gfx_image_rgba(u32 *args)
{
	/* TODO */
	return 0;
	(void)args;
}

static u32 syscall_gfx_image_rgb(u32 *args)
{
	/* TODO */
	return 0;
	(void)args;
}

static u32 syscall_gfx_image_rgb565(u32 *args)
{
	/* TODO */
	return 0;
	(void)args;
}

static u32 syscall_gfx_image_grayscale(u32 *args)
{
	/* TODO */
	return 0;
	(void)args;
}

static u32 syscall_gfx_image_1bit(u32 *args)
{
	/* TODO */
	return 0;
	(void)args;
}

/* FS */
static u32 syscall_file_open(u32 *args)
{
	/* TODO */
	return 0;
	(void)args;
}

static u32 syscall_file_read(u32 *args)
{
	/* TODO */
	return 0;
	(void)args;
}

static u32 syscall_file_write(u32 *args)
{
	/* TODO */
	return 0;
	(void)args;
}

static u32 syscall_file_close(u32 *args)
{
	/* TODO */
	return 0;
	(void)args;
}

static u32 syscall_file_size(u32 *args)
{
	/* TODO */
	return 0;
	(void)args;
}

/* KBD */
static u32 syscall_keyboard_is_key_pressed(u32 *args)
{
	/* TODO */
	return 0;
	(void)args;
}

static u32 syscall_keyboard_register_event(u32 *args)
{
	/* TODO */
	return 0;
	(void)args;
}

static u32 syscall_serial_write(u32 *args)
{
	/* TODO */
	return 0;
	(void)args;
}

static u32 syscall_serial_read(u32 *args)
{
	/* TODO */
	return 0;
	(void)args;
}

static u32 (*syscalls[])(u32 *) =
{
	syscall_exit,

	syscall_memcpy,
	syscall_memmove,
	syscall_memcmp,
	syscall_memchr,
	syscall_memset,

	syscall_strcpy,
	syscall_strncpy,
	syscall_strlen,
	syscall_strcmp,
	syscall_strncmp,
	syscall_strchr,

	syscall_rand,

	syscall_gfx_rect,
	syscall_gfx_image_rgba,
	syscall_gfx_image_rgb,
	syscall_gfx_image_rgb565,
	syscall_gfx_image_grayscale,
	syscall_gfx_image_1bit,

	syscall_file_open,
	syscall_file_read,
	syscall_file_write,
	syscall_file_close,
	syscall_file_size,

	syscall_keyboard_is_key_pressed,
	syscall_keyboard_register_event,

	syscall_serial_write,
	syscall_serial_read,
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
static i32 emulator_next(Emulator *emu)
{
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
						break;

					case 1:
						/* MULH */
#ifdef DEBUG
						printf("mulh r%d, r%d, r%d\n", rd, rs1, rs2);
#endif
						break;

					case 2:
						/* MULHSU */
#ifdef DEBUG
						printf("mulhsu r%d, r%d, r%d\n", rd, rs1, rs2);
#endif
						break;

					case 3:
						/* MULHU */
#ifdef DEBUG
						printf("mulhu r%d, r%d, r%d\n", rd, rs1, rs2);
#endif
						break;

					case 4:
						/* DIV */
#ifdef DEBUG
						printf("div r%d, r%d, r%d\n", rd, rs1, rs2);
#endif
						break;

					case 5:
						/* DIVU */
#ifdef DEBUG
						printf("divu r%d, r%d, r%d\n", rd, rs1, rs2);
#endif
						break;

					case 6:
						/* REM */
#ifdef DEBUG
						printf("rem r%d, r%d, r%d\n", rd, rs1, rs2);
#endif
						break;

					case 7:
						/* REMU */
#ifdef DEBUG
						printf("remu r%d, r%d, r%d\n", rd, rs1, rs2);
#endif
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
			u32 rd, rs1, ret, imm;
			rd = (instr >> 7) & 0x1F;
			rs1 = (instr >> 15) & 0x1F;
			imm = (instr >> 20) & ~1;

			offset = sext(12, imm);
#ifdef DEBUG
			printf("jalr r%d, r%d%+d\n", rd, rs1, offset);
#endif
			ret = emu->PC + 4;
			emu->PC = emu->Registers[rs1] + offset;
			emu->Registers[rd] = ret;
			return 0;
		}

		default:
			/* INVALID */
#ifdef DEBUG
			printf("INVALID\n");
#endif
			return -1;
	}

	emu->PC += 4;
	return 0;
}
