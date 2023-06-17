/**
 * @file    emulator.c
 * @author  Anton Tchekov
 * @version 0.1
 * @date    24.05.2023
 */

#define TASK_GUI       0
#define TASK_BG0       1
#define TASK_BG1       2

#define TASK_STOPPED   0
#define TASK_SETUP     1
#define TASK_IDLE      2
#define TASK_RUNNING   3

/** Emulated process */
typedef struct
{
	u8 Status;

	/** Event handler addresses */
	u32 Events[EVENT_COUNT];

	/** Program counter */
	u32 PC;

	/** CPU Registers */
	u32 Registers[32];
} Emulator;

/* --- LOGGING --- */
#ifdef DEBUG

/** Print to serial/LCD */
#define EMU_LOG(...) log_boot_P(LOG_EXT, __VA_ARGS__)

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

/** Empty logger */
#define EMU_LOG(...)

#endif /* DEBUG */

/* --- OPCODES --- */

/** Maximum number of tasks */
#define MAX_TASKS        3

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

/** Current task bank */
static u8 _cur_task;

/** List of currently running tasks */
static Emulator _tasks[MAX_TASKS];

/** Currently active task */
static Emulator *_emu = &_tasks[0];

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
	return (x >= LCD_WIDTH) ||
		(y >= LCD_HEIGHT) ||
		(w > LCD_WIDTH) ||
		(h > LCD_HEIGHT) ||
		(x + w > LCD_WIDTH) ||
		(y + h > LCD_HEIGHT);
}

/* --- Store --- */

/**
 * @brief Store byte
 *
 * @param addr The address at which to store the value
 * @param value The value to store
 */
static void memory_sb(u32 addr, u32 value)
{
	i8 value8 = (i8)value;
	xmem_write_chip(_cur_task, addr & XMEM_CHIP_MASK, &value8, sizeof(value8));
}

/**
 * @brief Store 16-bit half-word
 *
 * @param addr The address at which to store the value
 * @param value The value to store
 */
static void memory_sh(u32 addr, u32 value)
{
	i16 value16 = (i16)value;
	xmem_write_chip(_cur_task, addr & XMEM_CHIP_MASK, &value16, sizeof(value16));
}

/**
 * @brief Store 32-bit word
 *
 * @param addr The address at which to store the value
 * @param value The value to store
 */
static void memory_sw(u32 addr, u32 value)
{
	xmem_write_chip(_cur_task, addr & XMEM_CHIP_MASK, &value, sizeof(value));
}

/* --- Load --- */
/**
 * @brief Load a signed byte and perform sign-extension
 *
 * @param addr The address from which to load the value
 * @return Loaded value
 */
static u32 memory_lb(u32 addr)
{
	i8 value8;
	xmem_read_chip(_cur_task, addr & XMEM_CHIP_MASK, &value8, sizeof(value8));
	return (i32)value8;
}

/**
 * @brief Load a 16-bit signed half-word and perform sign extension
 *
 * @param addr The address from which to load the value
 * @return Loaded value
 */
static u32 memory_lh(u32 addr)
{
	i16 value16;
	xmem_read_chip(_cur_task, addr & XMEM_CHIP_MASK, &value16, sizeof(value16));
	return (i32)value16;
}

/**
 * @brief Load a 32-bit word
 *
 * @param addr The address from which to load the value
 * @return Loaded value
 */
static u32 memory_lw(u32 addr)
{
	u32 value;
	xmem_read_chip(_cur_task, addr & XMEM_CHIP_MASK, &value, sizeof(value));
	return value;
}

/**
 * @brief Load an unsigned byte
 *
 * @param addr The address from which to load the value
 * @return Loaded value
 */
static u32 memory_lbu(u32 addr)
{
	u8 value8;
	xmem_read_chip(_cur_task, addr & XMEM_CHIP_MASK, &value8, sizeof(value8));
	return value8;
}

/**
 * @brief Load an unsigned 16-bit half-word
 *
 * @param addr The address from which to load the value
 * @return Loaded value
 */
static u32 memory_lhu(u32 addr)
{
	u16 value16;
	xmem_read_chip(_cur_task, addr & XMEM_CHIP_MASK, &value16, sizeof(value16));
	return value16;
}

/* --- Syscalls --- */

/**
 * @brief Exit program
 *
 * @param args args[0]: Exit code
 * @return Non-zero on error
 */
static u8 syscall_exit(u32 *args)
{
	_emu->Status = TASK_STOPPED;
	return 0;
	(void)args;
}

/**
 * @brief Yield function to pass control to the scheduler
 *
 * @param args Unused
 * @return Non-zero on error
 */
static u8 syscall_finish(u32 *args)
{
	_emu->Status = TASK_IDLE;
	return 0;
	(void)args;
}

/**
 * @brief Register an event handler
 *
 * @param args args[0]: Event Type,
 *             args[1]: Event Handler Address
 * @return Non-zero on error
 */
static u8 syscall_event_register(u32 *args)
{
	u32 type, addr;

	type = args[0];
	addr = args[1];
	if(type >= EVENT_COUNT)
	{
		return 1;
	}

	EMU_LOG(PSTR("Registered Event Type: %"PRIu32" - Addr: %"PRIu32), type, addr);
	_emu->Events[type] = addr;
	return 0;
}

/**
 * @brief System call to get the number of milliseconds since startup
 *
 * @param args args[0]: Return value (milliseconds)
 * @return Non-zero on error
 */
static u8 syscall_millis(u32 *args)
{
	args[0] = millis();
	return 0;
}

/**
 * @brief System call to get a random number
 *
 * @param args args[0]: Return value (random number)
 * @return Non-zero on error
 */
static u8 syscall_rand(u32 *args)
{
	args[0] = random_get();
	return 0;
}

/**
 * @brief System call to send data over the serial interface
 *
 * @param args args[0]: Pointer to string to send
 *             args[1]: Number of bytes to send
 * @return Non-zero on error
 */
static u8 syscall_serial_write(u32 *args)
{
	u8 buf[128];
	u8 cur;
	u32 ptr, len;

	ptr = args[0];
	len = args[1];
	if(ptr > XMEM_CHIP_SIZE - len)
	{
		return 1;
	}

	while(len)
	{
		cur = len > sizeof(buf) ? sizeof(buf) : len;
		xmem_read_chip(_cur_task, ptr, buf, cur);
		serial_tx_data(buf, cur);
		len -= cur;
		ptr += cur;
	}

	return 0;
}

/**
 * @brief System call to draw a rectangle
 *
 * @param args args[0]: X-Coordinate
 *             args[1]: Y-Coordinate
 *             args[2]: Width
 *             args[3]: Height
 *             args[4]: ABGR Color
 * @return Non-zero on error
 */
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
	lcd_rect(x, y, w, h, color);
	return 0;
}

/** System call function pointer array */
static u8 (*syscalls[])(u32 *) =
{
	syscall_exit,
	syscall_finish,
	syscall_event_register,

	syscall_gfx_rect,
	//syscall_gfx_string,

	syscall_rand,
	syscall_serial_write,
	syscall_millis
};

/**
 * @brief Perform a system call
 *
 * @param id System call ID
 * @param args System call arguments, return value is placed in args[0]
 * @return Non-zero on failure
 */
static u8 syscall(u32 id, u32 *args)
{
	EMU_LOG(PSTR("ecall %"PRIu32""), id);
	if(id >= ARRLEN(syscalls))
	{
		return 1;
	}

	return syscalls[id](args);
}

/* --- EMULATOR --- */
static void emulator_init(Emulator *emu)
{
	emu->Status = TASK_SETUP;
	emu->Registers[2] = XMEM_CHIP_SIZE - 4;
}

static void emulator_run(Emulator *emu)
{
	u16 i = 0xFFFF;
	do
	{
	u32 instr, opcode;
	instr = memory_lw(emu->PC);

	/* r0 = Zero Register */
	emu->Registers[0] = 0;

	/* bits 7 .. 0 */
	opcode = instr & 0x7F;
	if((opcode & 0x03) != 0x03)
	{
		goto error;
	}

	opcode >>= 2;
	switch(opcode)
	{
		case OPCODE_ECALL:
			if(syscall(emu->Registers[17], &emu->Registers[10]))
			{
				EMU_LOG(PSTR("SYSCALL ERROR"));
				goto error;
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
					EMU_LOG(PSTR("lh r%"PRIu8", r%"PRIu8"%+"PRIi32), rd, rs1, offset);
					emu->Registers[rd] = memory_lh(emu->Registers[rs1] + offset);
					break;

				case 2:
					/* LW */
					EMU_LOG(PSTR("lw r%"PRIu8", r%"PRIu8"%+"PRIi32), rd, rs1, offset);
					emu->Registers[rd] = memory_lw(emu->Registers[rs1] + offset);
					break;

				case 4:
					/* LBU */
					EMU_LOG(PSTR("lbu r%"PRIu8", r%"PRIu8"%+"PRIi32), rd, rs1, offset);
					emu->Registers[rd] = memory_lbu(emu->Registers[rs1] + offset);
					break;

				case 5:
					/* LHU */
					EMU_LOG(PSTR("lhu r%"PRIu8", r%"PRIu8"%+"PRIi32), rd, rs1, offset);
					emu->Registers[rd] = memory_lhu(emu->Registers[rs1] + offset);
					break;

				default:
					goto error;
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
					memory_sb(emu->Registers[rs1] + offset, emu->Registers[rs2]);
					break;

				case 1:
					/* SH */
					EMU_LOG(PSTR("sh r%"PRIu8"%+"PRIi32", r%"PRIu8), rs1, offset, rs2);
					memory_sh(emu->Registers[rs1] + offset, emu->Registers[rs2]);
					break;

				case 2:
					/* SW */
					EMU_LOG(PSTR("sw r%"PRIu8"%+"PRIi32", r%"PRIu8), rs1, offset, rs2);
					memory_sw(emu->Registers[rs1] + offset, emu->Registers[rs2]);
					break;

				default:
					goto error;
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
						goto next;
					}
					break;

				case 1:
					/* BNE */
					EMU_LOG(PSTR("bne r%"PRIu8", r%"PRIu8", pc%+"PRIi32), rs1, rs2, offset);
					if(emu->Registers[rs1] != emu->Registers[rs2])
					{
						emu->PC += offset;
						goto next;
					}
					break;

				case 4:
					/* BLT */
					EMU_LOG(PSTR("blt r%"PRIu8", r%"PRIu8", pc%+"PRIi32), rs1, rs2, offset);
					if((i32)emu->Registers[rs1] < (i32)emu->Registers[rs2])
					{
						emu->PC += offset;
						goto next;
					}
					break;

				case 5:
					/* BGE */
					EMU_LOG(PSTR("bge r%"PRIu8", r%"PRIu8", pc%+"PRIi32), rs1, rs2, offset);
					if((i32)emu->Registers[rs1] >= (i32)emu->Registers[rs2])
					{
						emu->PC += offset;
						goto next;
					}
					break;

				case 6:
					/* BLTU */
					EMU_LOG(PSTR("bltu r%"PRIu8", r%"PRIu8", pc%+"PRIi32), rs1, rs2, offset);
					if(emu->Registers[rs1] < emu->Registers[rs2])
					{
						emu->PC += offset;
						goto next;
					}
					break;

				case 7:
					/* BGEU */
					EMU_LOG(PSTR("bgeu r%"PRIu8", r%"PRIu8", pc%+"PRIi32), rs1, rs2, offset);
					if(emu->Registers[rs1] >= emu->Registers[rs2])
					{
						emu->PC += offset;
						goto next;
					}
					break;

				default:
					goto error;
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
			goto next;
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
			goto next;
		}

		default:
			goto error;
	}

	emu->PC += 4;
next:
	} while(--i);
	return;

error:
	EMU_LOG(PSTR("INVALID INSTRUCTION"));
	_emu->Status = TASK_STOPPED;
}

static void emulator_call(Emulator *emu, u32 addr, u32 *args, u8 num)
{
	emu->PC = addr;
	emu->Status = TASK_RUNNING;
	memcpy(&emu->Registers[10], args, num * sizeof(*emu->Registers));
	emulator_run(emu);
}

static Status task_start(u8 chip, u32 sector, u8 count)
{
	u8 buf[SD_BLOCK_SIZE];
	u32 last;
	u32 addr;

	addr = 0;
	last = sector + count;
	while(sector < last)
	{
		RETURN_IF(sd_read(sector, buf));
		xmem_write(chip, addr, buf, SD_BLOCK_SIZE);
		addr += SD_BLOCK_SIZE;
		++sector;
	}

	emulator_init(&_tasks[chip]);
	return STATUS_OK;
}

static void task_end(u8 chip)
{
	_tasks[chip].Status = TASK_STOPPED;
}

static u8 _task_find(void)
{
	u8 i;
	for(i = _cur_task + 1; i < XMEM_CHIP_COUNT; ++i)
	{
		if(_tasks[i].Status)
		{
			return i + 1;
		}
	}

	for(i = 0; i <= _cur_task; ++i)
	{
		if(_tasks[i].Status)
		{
			return i + 1;
		}
	}

	return 0;
}

static void kernel_update(void)
{
	Emulator *emu;
	u8 chip;

	/* Round-robin scheduler */
	if(!(chip = _task_find()))
	{
		panic(PSTR("No tasks running"));
	}

	_cur_task = chip - 1;
	emu = &_tasks[_cur_task];
	if(emu->Status == TASK_SETUP)
	{
		emulator_call(_emu, 0, NULL, 0);
	}
	else if(emu->Status == TASK_RUNNING)
	{
		emulator_run(_emu);
	}
	else if(emu->Status == TASK_IDLE)
	{
		u32 addr;
		KeyEvent e;
		if(_cur_task == TASK_GUI && ps2_read(&e))
		{
			if((addr = _emu->Events[EVENT_KEY]))
			{
				u32 args[] = { e.Key, e.ASCII, e.State };
				emulator_call(_emu, addr, args, ARRLEN(args));
			}
		}
		else
		{
			if((addr = _emu->Events[EVENT_LOOP]))
			{
				emulator_call(_emu, addr, NULL, 0);
			}
		}
	}
}
