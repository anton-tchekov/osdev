#include "types.h"
#include "memory.c"
#include <stdio.h>

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
	for(int i = 0; i < 32; ++i)
	{
		printf("R%d%s 0x%08X %10d\n", i, i < 10 ? " " : "",  emu->Registers[i], emu->Registers[i]);
	}

	printf("\n");
}

static i32 syscall_print(u32 *args)
{
	u8 c;
	u32 addr = args[0];
	while((c = memory_lb(addr++)))
	{
		fputc(c, stdout);
	}

	args[0] = 0;
	return 0;
}

#include <stdlib.h>

static i32 syscall_exit(u32 *args)
{
	exit(0);
}

static i32 (*syscalls[])(u32 *) =
{
	syscall_print,
	syscall_exit
};

#define ARRLEN(A) (sizeof(A) / sizeof(*A))

static i32 syscall(u32 id, u32 *args)
{
	if(id >= ARRLEN(syscalls))
	{
		return -1;
	}

	return syscalls[id](args);
}

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
		printf("INVALID\n");
		return -1;
	}

	opcode >>= 2;
	switch(opcode)
	{
		case OPCODE_ECALL:
			if(syscall(emu->Registers[17], &emu->Registers[10]))
			{
				printf("SYSCALL ERROR\n");
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
					printf("lb r%d, r%d%+d\n", rd, rs1, offset);
					emu->Registers[rd] = memory_lb(emu->Registers[rs1] + offset);
					break;

				case 1:
					/* LH */
					printf("lh r%d, r%d%+d\n", rd, rs1, offset);
					emu->Registers[rd] = memory_lh(emu->Registers[rs1] + offset);
					break;

				case 2:
					/* LW */
					printf("lw r%d, r%d%+d\n", rd, rs1, offset);
					emu->Registers[rd] = memory_lw(emu->Registers[rs1] + offset);
					break;

				case 4:
					/* LBU */
					printf("lbu r%d, r%d%+d\n", rd, rs1, offset);
					emu->Registers[rd] = memory_lbu(emu->Registers[rs1] + offset);
					break;

				case 5:
					/* LHU */
					printf("lhu r%d, r%d%+d\n", rd, rs1, offset);
					emu->Registers[rd] = memory_lhu(emu->Registers[rs1] + offset);
					break;

				default:
					/* INVALID */
					printf("INVALID\n");
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
					printf("addi r%d, r%d, %d\n", rd, rs1, imm);
					emu->Registers[rd] = emu->Registers[rs1] + imm;
					break;

				case 1:
					/* SLLI */
					imm &= 0x1F;
					printf("slli r%d, r%d, %d\n", rd, rs1, imm);
					emu->Registers[rd] = emu->Registers[rs1] << imm;
					break;

				case 2:
					/* SLTI */
					imm = sext(12, imm);
					printf("slti r%d, r%d, %d\n", rd, rs1, imm);
					emu->Registers[rd] = (i32)emu->Registers[rs1] < (i32)imm;
					break;

				case 3:
					/* SLTIU */
					printf("sltiu r%d, r%d, %d\n", rd, rs1, imm);
					emu->Registers[rd] = emu->Registers[rs1] < imm;
					break;

				case 4:
					/* XORI */
					imm = sext(12, imm);
					printf("xori r%d, r%d, 0x%08X\n", rd, rs1, imm);
					emu->Registers[rd] = emu->Registers[rs1] ^ imm;
					break;

				case 5:
					if(imm >> 5)
					{
						/* SRAI */
						imm &= 0x1F;
						printf("srai r%d, r%d, %d\n", rd, rs1, imm);
						emu->Registers[rd] = (i32)emu->Registers[rs1] >> imm;
					}
					else
					{
						/* SRLI */
						imm &= 0x1F;
						printf("srli r%d, r%d, %d\n", rd, rs1, imm);
						emu->Registers[rd] = emu->Registers[rs1] >> imm;
					}
					break;

				case 6:
					/* ORI */
					imm = sext(12, imm);
					printf("ori r%d, r%d, 0x%08X\n", rd, rs1, imm);
					emu->Registers[rd] = emu->Registers[rs1] | imm;
					break;

				case 7:
					/* ANDI */
					imm = sext(12, imm);
					printf("andi r%d, r%d, 0x%08X\n", rd, rs1, imm);
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
			printf("auipc r%d, 0x%08X\n", rd, imm);
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
					printf("sb r%d%+d, r%d\n", rs1, offset, rs2);
					memory_sb(emu->Registers[rs1] + offset, emu->Registers[rs2]);
					break;

				case 1:
					/* SH */
					printf("sh r%d%+d, r%d\n", rs1, offset, rs2);
					memory_sh(emu->Registers[rs1] + offset, emu->Registers[rs2]);
					break;

				case 2:
					/* SW */
					printf("sw r%d%+d, r%d\n", rs1, offset, rs2);
					memory_sw(emu->Registers[rs1] + offset, emu->Registers[rs2]);
					break;

				default:
					/* INVALID */
					printf("INVALID\n");
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
						printf("mul r%d, r%d, r%d\n", rd, rs1, rs2);
						break;

					case 1:
						/* MULH */
						printf("mulh r%d, r%d, r%d\n", rd, rs1, rs2);
						break;

					case 2:
						/* MULHSU */
						printf("mulhsu r%d, r%d, r%d\n", rd, rs1, rs2);
						break;

					case 3:
						/* MULHU */
						printf("mulhu r%d, r%d, r%d\n", rd, rs1, rs2);
						break;

					case 4:
						/* DIV */
						printf("div r%d, r%d, r%d\n", rd, rs1, rs2);
						break;

					case 5:
						/* DIVU */
						printf("divu r%d, r%d, r%d\n", rd, rs1, rs2);
						break;

					case 6:
						/* REM */
						printf("rem r%d, r%d, r%d\n", rd, rs1, rs2);
						break;

					case 7:
						/* REMU */
						printf("remu r%d, r%d, r%d\n", rd, rs1, rs2);
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
							printf("sub r%d, r%d, r%d\n", rd, rs1, rs2);
							emu->Registers[rd] = emu->Registers[rs1] - emu->Registers[rs2];
						}
						else
						{
							/* ADD */
							printf("add r%d, r%d, r%d\n", rd, rs1, rs2);
							emu->Registers[rd] = emu->Registers[rs1] + emu->Registers[rs2];
						}
						break;

					case 1:
						/* SLL */
						printf("sll r%d, r%d, r%d\n", rd, rs1, rs2);
						emu->Registers[rd] = emu->Registers[rs1] << (emu->Registers[rs2] & 0x1F);
						break;

					case 2:
						/* SLT */
						printf("slt r%d, r%d, r%d\n", rd, rs1, rs2);
						emu->Registers[rd] = (i32)emu->Registers[rs1] < (i32)emu->Registers[rs2];
						break;

					case 3:
						/* SLTU */
						printf("sltu r%d, r%d, r%d\n", rd, rs1, rs2);
						emu->Registers[rd] = emu->Registers[rs1] < emu->Registers[rs2];
						break;

					case 4:
						/* XOR */
						printf("xor r%d, r%d, r%d\n", rd, rs1, rs2);
						emu->Registers[rd] = emu->Registers[rs1] ^ emu->Registers[rs2];
						break;

					case 5:
						if(funct7)
						{
							/* SRA */
							printf("srl r%d, r%d, r%d\n", rd, rs1, rs2);
							emu->Registers[rd] = (i32)emu->Registers[rs1] >> (emu->Registers[rs2] & 0x1F);
						}
						else
						{
							/* SRL */
							printf("sra r%d, r%d, r%d\n", rd, rs1, rs2);
							emu->Registers[rd] = emu->Registers[rs1] >> (emu->Registers[rs2] & 0x1F);
						}
						break;

					case 6:
						/* OR */
						printf("or r%d, r%d, r%d\n", rd, rs1, rs2);
						emu->Registers[rd] = emu->Registers[rs1] | emu->Registers[rs2];
						break;

					case 7:
						/* AND */
						printf("and r%d, r%d, r%d\n", rd, rs1, rs2);
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
			printf("lui r%d, 0x%08X\n", rd, imm);
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
					printf("beq r%d, r%d, pc%+d\n", rs1, rs2, offset);
					if(emu->Registers[rs1] == emu->Registers[rs2])
					{
						emu->PC += offset;
						return 0;
					}
					break;

				case 1:
					/* BNE */
					printf("bne r%d, r%d, pc%+d\n", rs1, rs2, offset);
					if(emu->Registers[rs1] != emu->Registers[rs2])
					{
						emu->PC += offset;
						return 0;
					}
					break;

				case 4:
					/* BLT */
					printf("blt r%d, r%d, pc%+d\n", rs1, rs2, offset);
					if((i32)emu->Registers[rs1] < (i32)emu->Registers[rs2])
					{
						emu->PC += offset;
						return 0;
					}
					break;

				case 5:
					/* BGE */
					printf("bge r%d, r%d, pc%+d\n", rs1, rs2, offset);
					if((i32)emu->Registers[rs1] >= (i32)emu->Registers[rs2])
					{
						emu->PC += offset;
						return 0;
					}
					break;

				case 6:
					/* BLTU */
					printf("bltu r%d, r%d, pc%+d\n", rs1, rs2, offset);
					if(emu->Registers[rs1] < emu->Registers[rs2])
					{
						emu->PC += offset;
						return 0;
					}
					break;

				case 7:
					/* BGEU */
					printf("bgeu r%d, r%d, pc%+d\n", rs1, rs2, offset);
					if(emu->Registers[rs1] >= emu->Registers[rs2])
					{
						emu->PC += offset;
						return 0;
					}
					break;

				default:
					/* INVALID */
					printf("INVALID\n");
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
			printf("jal r%d, pc%+d\n", rd, offset);
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
			printf("jalr r%d, r%d%+d\n", rd, rs1, offset);
			ret = emu->PC + 4;
			emu->PC = emu->Registers[rs1] + offset;
			emu->Registers[rd] = ret;
			return 0;
		}

		default:
			/* INVALID */
			printf("INVALID\n");
			return -1;
	}

	emu->PC += 4;
	return 0;
}

