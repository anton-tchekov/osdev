
/**
 * @brief Calculate which bank an address falls onto
 *
 * @param addr Full memory address
 * @return Bank number
 */
static u8 _addr_to_bank(u32 addr)
{
	return addr >> BANK_SIZE_POT;
}

/**
 * @brief Calculate the offset from the start of a memory bank for an address
 *
 * @param addr Full memory address
 * @return Bank address
 */
static u16 _addr_bank_offset(u32 addr)
{
	return addr & (BANK_SIZE - 1);
}

/** Internal struct to handle reads and writes across bank boundaries */
typedef struct
{
	/** First bank to access */
	u8 BankFirst;

	/** Second bank to access */
	u8 BankSecond;

	/** Start address of the first part */
	u16 AddrFirst;

	/** Size of the first part */
	u16 SizeFirst;

	/** Size of the second part */
	u16 SizeSecond;
} AddrHelper;

/**
 * @brief Calculate which bank will be affected by a memory operation
 *
 * @param addr Start address
 * @param size Number of bytes affected
 * @param h Output information
 */
static void _xmem_overlap(u32 addr, u16 size, AddrHelper *h)
{
	u32 addr_end;

	h->AddrFirst = _addr_bank_offset(addr);
	addr_end = addr + size;

	h->BankFirst = _addr_to_bank(addr);
	h->BankSecond = _addr_to_bank(addr_end);
	if(h->BankFirst == h->BankSecond)
	{
		h->SizeFirst = size;
	}
	else
	{
		h->SizeSecond = _addr_bank_offset(addr_end);
		h->SizeFirst = size - h->SizeSecond;
	}
}

u32 xmem_size(void)
{
	return BANK_COUNT * BANK_SIZE;
}


void xmem_read(u32 addr, void *data, u16 size)
{
	AddrHelper h;
	_xmem_overlap(addr, size, &h);
	_xmem_read(h.BankFirst, h.AddrFirst, data, h.SizeFirst);
	if(h.BankSecond != h.BankFirst)
	{
		_xmem_read(h.BankSecond, 0, (u8 *)data + h.SizeFirst, h.SizeSecond);
	}
}

void xmem_write(u32 addr, const void *data, u16 size)
{
	AddrHelper h;
	_xmem_overlap(addr, size, &h);
	_xmem_write(h.BankFirst, h.AddrFirst, data, h.SizeFirst);
	if(h.BankSecond != h.BankFirst)
	{
		_xmem_write(h.BankSecond, 0, (const u8 *)data + h.SizeFirst,
			h.SizeSecond);
	}
}

void xmem_set(u32 addr, u8 value, u16 size)
{
	AddrHelper h;
	_xmem_overlap(addr, size, &h);
	_xmem_set(h.BankFirst, h.AddrFirst, value, h.SizeFirst);
	if(h.BankSecond != h.BankFirst)
	{
		_xmem_set(h.BankSecond, 0, value, h.SizeSecond);
	}
}


/**
 * @brief Read the specified number of bytes from the external memory
 *
 * @param addr Start address
 * @param data Destination buffer
 * @param size Number of bytes to read
 */
void xmem_read(u32 addr, void *data, u16 size);

/**
 * @brief Write the specified number of bytes to the external memory
 *
 * @param addr Start address
 * @param data Source buffer
 * @param size Number of bytes to write
 */
void xmem_write(u32 addr, const void *data, u16 size);

/**
 * @brief Set a specified number of bytes in the external memory to a value
 *
 * @param addr Start address
 * @param value The value to write
 * @param size Number of bytes to set
 */
void xmem_set(u32 addr, u8 value, u16 size);

/**
 * @brief Get external memory size
 *
 * @return Size of the external memory in bytes
 */
u32 xmem_size(void);


/** The size of each memory bank as a power of two */
#define XMEM_CHIP_SIZE_POT   17
