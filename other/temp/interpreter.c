/**
 * @file    interpreter.c
 * @author  Anton Tchekov
 * @version 0.2
 * @date    17.05.2023
 * @brief   NanoC Interpreter
 */

/** Capacity of the interpreter operator stack */
#define INTERPRETER_OP_STACK_SIZE 32

// #define EMU_32

#ifdef EMU_32

typedef i32 EmuInt;

#define EMU_INT_PRINT     PRIi32
#define EMU_INT_WRITE     xmem_w32
#define EMU_INT_READ      xmem_r32

#else

typedef i16 EmuInt;

#define EMU_INT_PRINT     PRIi16
#define EMU_INT_WRITE     xmem_w16
#define EMU_INT_READ      xmem_r16

#endif

#define EMU_INT_SIZE      sizeof(EmuInt)

typedef enum
{
	INTERPRETER_OK,
	INTERPRETER_HALT,
	INTERPRETER_INV_INSTR,
	INTERPRETER_STACK_OVERFLOW,
	INTERPRETER_STACK_UNDERFLOW,
	INTERPRETER_INV_BUILTIN
} InterpreterStatus;

/** Interpreter struct */
typedef struct
{
	u8 Bank;
	EmuInt *OSP;
	u16 CSP;
	u16 FP;
	u16 IP;
	EmuInt OP_Stack[INTERPRETER_OP_STACK_SIZE];
} Interpreter;

/** Instruction Set */
typedef enum
{
	INSTR_HALT,
	INSTR_PUSHI8,
	INSTR_PUSHI16,
	INSTR_PUSHL,
	INSTR_POPL,
	INSTR_PUSHA8,
	INSTR_PUSHA16,
	INSTR_POPA8,
	INSTR_POPA16,
	INSTR_JZ,
	INSTR_JNZ,
	INSTR_JMP,
	INSTR_CALL,
	INSTR_RET,
	INSTR_DSP,
	INSTR_DUP,
	INSTR_POP,
	INSTR_U_MINUS,
	INSTR_L_NOT,
	INSTR_B_NOT,
	INSTR_L_OR,
	INSTR_L_AND,
	INSTR_B_OR,
	INSTR_B_XOR,
	INSTR_B_AND,
	INSTR_EQ,
	INSTR_NE,
	INSTR_LT,
	INSTR_GT,
	INSTR_LE,
	INSTR_GE,
	INSTR_B_SHL,
	INSTR_B_SHR,
	INSTR_ADD,
	INSTR_SUB,
	INSTR_MUL,
	INSTR_DIV,
	INSTR_MOD
} Instruction;

/** Marker value for invalid/not running task */
#define TASK_INVALID         0xFF

/** Maximum number of scheduler bursts until task switch */
#define SCHEDULER_BURSTS   32

/** Initial stack pointer value */
#define STACK_TOP            0xFFFE

/** Unsafe Mode: Disable Overflow and underflow checks for speed */
#define UNSAFE

#ifdef UNSAFE

/** Do nothing */
#define CHECK_UNDERFLOW(N)

/** Do nothing */
#define CHECK_OVERFLOW(N)

#else /* UNSAFE */

/** Check for operator stack underflow */
#define CHECK_UNDERFLOW(N) \
	if(i->OSP < op_stack) \
	{ \
		return INTERPRETER_STACK_UNDERFLOW; \
	}

/** Check for operator stack overflow */
#define CHECK_OVERFLOW(N) \
	if(i->OSP > INTERPRETER_OP_STACK_SIZE - (N)) \
	{ \
		return INTERPRETER_STACK_OVERFLOW; \
	}

#endif /* UNSAFE */

#define BINARY_OP(C) \
	{ \
		EmuInt *P, A, B; \
		CHECK_UNDERFLOW(2); \
		--i->OSP; \
		P = i->OSP - 1; \
		A = *P; \
		B = *i->OSP; \
		*P = (C); \
		++i->IP; \
	}

#define UNARY_OP(C) \
	{ \
		EmuInt *P, A; \
		CHECK_UNDERFLOW(1); \
		P = i->OSP - 1; \
		A = *P; \
		*P = (C); \
		++i->IP; \
	}

/** Current task */
static u8 _cur_task;

/** Task interpreters */
static Interpreter _tasks[XMEM_BANK_COUNT];

static EmuInt _buitlin_print(EmuInt *arg)
{
	log_boot_P(LOG_EXT, PSTR("PRINT NUMBER = %"EMU_INT_PRINT"\n"), arg[0]);
	return 0;
}

static EmuInt (*builtin[])(EmuInt *) =
{
	_buitlin_print,

	/*_builtin_random,
	_buitlin_millis,
	_buitlin_secs,*/

	/* Serial */
	/*_builtin_serial_lock,
	_builtin_serial_unlock,

	_buitlin_serial_tx,
	_buitlin_serial_tx_data,
	_buitlin_serial_tx_str,

	_buitlin_serial_rx,
	_buitlin_serial_rx_data,*/

	/* Graphics */
	/*_builtin_lcd_lock,
	_builtin_lcd_unlock,

	_builtin_lcd_string,
	_builtin_lcd_rect,*/

	/* Processes */
	/*_builtin_find_bank,
	_builtin_task_run,
	_builtin_task_end,*/
};

static void interpreter_init(Interpreter *i, u8 bank)
{
	i->CSP = STACK_TOP;
	i->FP = STACK_TOP;
	i->IP = 0;
	i->OSP = i->OP_Stack;
	i->Bank = bank;
}

static InterpreterStatus interpreter_burst(Interpreter *i)
{
	u8 cnt = 0xFF, cnt2 = SCHEDULER_BURSTS;
	do { do { switch(xmem_r8(i->IP))
	{
	case INSTR_HALT:
		return INTERPRETER_HALT;

	case INSTR_PUSHI8:
		CHECK_OVERFLOW(1);
		++i->IP;
		*i->OSP = xmem_r8(i->IP);
		++i->OSP;
		++i->IP;
		break;

	case INSTR_PUSHI16:
		CHECK_OVERFLOW(1);
		++i->IP;
		*i->OSP = xmem_r16(i->IP);
		++i->OSP;
		i->IP += 2;
		break;

	case INSTR_PUSHL:
	{
		u8 local;
		CHECK_OVERFLOW(1);
		++i->IP;
		local = xmem_r8(i->IP);
		*i->OSP = EMU_INT_READ(i->FP - EMU_INT_SIZE * local);
		++i->OSP;
		++i->IP;
		break;
	}

	case INSTR_POPL:
	{
		u8 local;
		CHECK_UNDERFLOW(1);
		++i->IP;
		local = xmem_r8(i->IP);
		--i->OSP;
		EMU_INT_WRITE(i->FP - EMU_INT_SIZE * local, *i->OSP);
		++i->IP;
		break;
	}

	case INSTR_PUSHA8:
	{
		EmuInt *p;
		CHECK_UNDERFLOW(1);
		++i->IP;
		p = i->OSP - 1;
		*p = xmem_r8(*p);
		break;
	}

	case INSTR_PUSHA16:
	{
		EmuInt *p;
		CHECK_UNDERFLOW(1);
		++i->IP;
		p = i->OSP - 1;
		*p = xmem_r16(*p);
		break;
	}

	case INSTR_POPA8:
		CHECK_UNDERFLOW(2);
		++i->IP;
		--i->OSP;
		xmem_w8(*(i->OSP - 1), *i->OSP);
		break;

	case INSTR_POPA16:
		CHECK_UNDERFLOW(2);
		++i->IP;
		--i->OSP;
		xmem_w16(*(i->OSP - 1), *i->OSP);
		break;

	case INSTR_DUP:
		CHECK_UNDERFLOW(1);
		CHECK_OVERFLOW(1);
		++i->IP;
		*i->OSP = *(i->OSP - 1);
		++i->OSP;
		break;

	case INSTR_JZ:
		CHECK_UNDERFLOW(1);
		--i->OSP;
		if(!*i->OSP)
		{
			i->IP = xmem_r16(i->IP + 1);
			break;
		}
		i->IP += 3;
		break;

	case INSTR_JNZ:
		CHECK_UNDERFLOW(1);
		--i->OSP;
		if(*i->OSP)
		{
			i->IP = xmem_r16(i->IP + 1);
			break;
		}
		i->IP += 3;
		break;

	case INSTR_JMP:
		i->IP = xmem_r16(i->IP + 1);
		break;

	case INSTR_CALL:
	{
		i16 addr;
		u8 args;
		u8 j;

		++i->IP;
		args = xmem_r8(i->IP);
		CHECK_UNDERFLOW(args);
		++i->IP;
		addr = xmem_r16(i->IP);
		i->IP += 2;
		i->OSP -= args;
		if(addr < 0)
		{
			u8 fn = -addr - 1;
			if(fn >= ARRLEN(builtin))
			{
				return INTERPRETER_INV_BUILTIN;
			}

			CHECK_OVERFLOW(1);
			*i->OSP = builtin[fn](i->OSP);
			++i->OSP;
			break;
		}

		for(j = 0; j < args; ++j)
		{
			xmem_w16(i->CSP - 4 - EMU_INT_SIZE * j, *(i->OSP + j));
		}

		xmem_w16(i->CSP, i->IP);
		i->CSP -= 2;
		xmem_w16(i->CSP, i->FP);
		i->CSP -= 2;
		i->FP = i->CSP;
		i->IP = addr;
		break;
	}

	case INSTR_RET:
		i->CSP = i->FP + 2;
		i->FP = xmem_r16(i->CSP);
		i->CSP += 2;
		i->IP = xmem_r16(i->CSP);
		break;

	case INSTR_DSP:
		++i->IP;
		i->CSP -= EMU_INT_SIZE * xmem_r8(i->IP);
		++i->IP;
		break;

	case INSTR_POP:
		CHECK_UNDERFLOW(1);
		++i->IP;
		--i->OSP;
		break;

	case INSTR_U_MINUS: UNARY_OP(-A);       break;
	case INSTR_L_NOT:   UNARY_OP(!A);       break;
	case INSTR_B_NOT:   UNARY_OP(~A);       break;
	case INSTR_L_OR:    BINARY_OP(A || B);  break;
	case INSTR_L_AND:   BINARY_OP(A && B);  break;
	case INSTR_B_OR:    BINARY_OP(A | B);   break;
	case INSTR_B_XOR:   BINARY_OP(A ^ B);   break;
	case INSTR_B_AND:   BINARY_OP(A & B);   break;
	case INSTR_EQ:      BINARY_OP(A == B);  break;
	case INSTR_NE:      BINARY_OP(A != B);  break;
	case INSTR_LT:      BINARY_OP(A < B);   break;
	case INSTR_GT:      BINARY_OP(A > B);   break;
	case INSTR_LE:      BINARY_OP(A <= B);  break;
	case INSTR_GE:      BINARY_OP(A >= B);  break;
	case INSTR_B_SHL:   BINARY_OP(A << B);  break;
	case INSTR_B_SHR:   BINARY_OP(A >> B);  break;
	case INSTR_ADD:     BINARY_OP(A + B);   break;
	case INSTR_SUB:     BINARY_OP(A - B);   break;
	case INSTR_MUL:     BINARY_OP(A * B);   break;
	case INSTR_DIV:     BINARY_OP(A / B);   break;
	case INSTR_MOD:     BINARY_OP(A % B);   break;
	default:
		return INTERPRETER_INV_INSTR;
	}

	--cnt; } while(cnt); --cnt2; } while(cnt2);

	return INTERPRETER_OK;
}

static u8 _bank_alloc(void)
{
	u8 i;
	for(i = 0; i < XMEM_BANK_COUNT; ++i)
	{
		if(_tasks[i].Bank == TASK_INVALID)
		{
			return i;
		}
	}

	return TASK_INVALID;
}

static u8 task_start(u32 sector, u8 count)
{
	u8 buf[SD_BLOCK_SIZE];
	u32 last;
	u16 addr;
	u8 bank;

	if((bank = _bank_alloc()) == TASK_INVALID)
	{
		return TASK_INVALID;
	}

	addr = 0;
	last = sector + count;
	while(sector < last)
	{
		if(sd_read(sector, buf))
		{
			return TASK_INVALID;
		}

		xmem_write(bank, addr, buf, SD_BLOCK_SIZE);
		addr += SD_BLOCK_SIZE;
		++sector;
	}

	interpreter_init(&_tasks[bank], bank);
	return bank;
}

static void task_dump(u8 bank, u8 sectors)
{
	u8 buf[SD_BLOCK_SIZE];
	u8 i;
	u16 addr;
	for(i = 0; i < sectors; ++i)
	{
		addr = SD_BLOCK_SIZE * i;
		xmem_read(bank, addr, buf, SD_BLOCK_SIZE);
		memory_dump(addr, buf, SD_BLOCK_SIZE);
	}
}

static void task_end(u8 bank)
{
	_tasks[bank].Bank = TASK_INVALID;
}

static u8 _task_find(void)
{
	u8 i;
	for(i = _cur_task + 1; i < XMEM_BANK_COUNT; ++i)
	{
		if(_tasks[i].Bank != TASK_INVALID)
		{
			return i;
		}
	}

	for(i = 0; i <= _cur_task; ++i)
	{
		if(_tasks[i].Bank != TASK_INVALID)
		{
			return i;
		}
	}

	return TASK_INVALID;
}

static void kernel_init(void)
{
	u8 i;
	for(i = 0; i < XMEM_BANK_COUNT; ++i)
	{
		_tasks[i].Bank = TASK_INVALID;
	}
}

static void kernel_update(void)
{
	u8 bank;
	Interpreter *interpreter;

	/* Round-robin scheduler */
	if((bank = _task_find()) == TASK_INVALID)
	{
		panic(PSTR("No tasks running"));
	}

	_cur_task = bank;
	_cache_init(bank);
	interpreter = &_tasks[bank];
	if(interpreter_burst(interpreter))
	{
		task_end(bank);
	}
}
