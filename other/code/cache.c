
#define CACHE_LINES           8
#define CACHE_LINE_SIZE      32
#define CACHE_LINE_SIZE_POT   5

static u16 _cached[CACHE_LINES];
static u8 _lines[CACHE_LINES][CACHE_LINE_SIZE];
static u8 _invcnt[CACHE_LINES];
static u8 _cache_bank = 0xFF;

static void _cache_init(u8 bank)
{
	u8 i;
	u16 addr;
	if(bank == _cache_bank)
	{
		return;
	}

	/* preload start of program */
	_cache_bank = bank;
	for(i = 0, addr = 0; i < CACHE_LINES / 2; ++i, addr += CACHE_LINE_SIZE)
	{
		_cached[i] = addr;
		_invcnt[i] = 1;
		xmem_read(_cache_bank, addr, _lines[i], CACHE_LINE_SIZE);
	}

	/* preload stack */
	for(addr = 0x10000 - CACHE_LINE_SIZE; i < CACHE_LINES;
		++i, addr -= CACHE_LINE_SIZE)
	{
		_cached[i] = addr;
		_invcnt[i] = 1;
		xmem_read(_cache_bank, addr, _lines[i], CACHE_LINE_SIZE);
	}
}

static u8 _cache_get(u16 pre)
{
	static u8 halving;
	u8 i;

	/* Periodic halving of priorities */
	if(++halving == 0xFF)
	{
		halving = 0;
		for(i = 0; i < CACHE_LINES; ++i)
		{
			_invcnt[i] >>= 1;
		}
	}

	/* Check if cached */
	for(i = 0; i < CACHE_LINES; ++i)
	{
		if(_cached[i] == pre)
		{
			++_invcnt[i];
			return i;
		}
	}

	{
		/* Cache miss */
		log_boot_P(LOG_EXT, PSTR("CACHE MISS!"));
		u8 j, min = 0xFF;
		i = 0;
		for(j = 0; j < CACHE_LINES; ++j)
		{
			if(_invcnt[j] < min)
			{
				i = j;
				min = _invcnt[j];
			}
		}

		/* Invalidate */
		_invcnt[i] = 1;
		xmem_write(_cache_bank, pre, _lines[i], CACHE_LINE_SIZE);
		_cached[i] = pre;
		xmem_read(_cache_bank, pre, _lines[i], CACHE_LINE_SIZE);
	}

	return i;
}

static u8 xmem_r8(u16 addr)
{
	u16 pre = addr & ~((u16)CACHE_LINE_SIZE - 1);
	u8 offset = addr & (CACHE_LINE_SIZE - 1);
	return _lines[_cache_get(pre)][offset];
}

static u16 xmem_r16(u16 addr)
{
	u16 pre = addr & ~((u16)CACHE_LINE_SIZE - 1);
	u8 offset = addr & (CACHE_LINE_SIZE - 1);
	u8 line = _cache_get(pre);
	if(offset > CACHE_LINE_SIZE - 2)
	{
		/* unaligned */
		u8 line2 = _cache_get(pre + CACHE_LINE_SIZE);
		return _lines[line][offset] | (_lines[line2][0] << 8);
	}

	return _lines[line][offset] | (_lines[line][offset + 1] << 8);
}

static void xmem_w8(u16 addr, u8 val)
{
	u16 pre = addr & ~((u16)CACHE_LINE_SIZE - 1);
	u8 offset = addr & (CACHE_LINE_SIZE - 1);
	_lines[_cache_get(pre)][offset] = val;
}

static void xmem_w16(u16 addr, u16 val)
{
	u16 pre = addr & ~((u16)CACHE_LINE_SIZE - 1);
	u8 offset = addr & (CACHE_LINE_SIZE - 1);
	u8 line = _cache_get(pre);
	if(offset > CACHE_LINE_SIZE - 2)
	{
		/* unaligned */
		u8 line2 = _cache_get(pre + CACHE_LINE_SIZE);
		_lines[line][offset] = val;
		_lines[line2][0] = val >> 8;
	}

	_lines[line][offset] = val;
	_lines[line][offset + 1] = val >> 8;
}

/*
static u8 xmem_r8(u8 bank, u16 addr)
{
	u8 byte;

	_xmem_start_bank(XMEM_COMMAND_READ, bank, addr);
	byte = spi_xchg_fast(SPI_DUMMY);
	_xmem_end_bank(bank);
	return byte;
}

static u16 xmem_r16(u8 bank, u16 addr)
{
	u16 val;
	_xmem_start_bank(XMEM_COMMAND_READ, bank, addr);
	val = spi_xchg_fast(SPI_DUMMY);
	val |= (u16)(spi_xchg_fast(SPI_DUMMY)) << 8;
	_xmem_end_bank(bank);
	return val;
}

static void xmem_w8(u8 bank, u16 addr, u8 val)
{
	_xmem_start_bank(XMEM_COMMAND_WRITE, bank, addr);
	spi_xchg_fast(val);
	_xmem_end_bank(bank);
}

static void xmem_w16(u8 bank, u16 addr, u16 val)
{
	_xmem_start_bank(XMEM_COMMAND_WRITE, bank, addr);
	spi_xchg_fast(val);
	spi_xchg_fast(val >> 8);
	_xmem_end_bank(bank);
}
*/
/*
static u32 xmem_r32(u8 bank, u16 addr)
{
	u16 val;
	_xmem_start_bank(XMEM_COMMAND_READ, bank, addr);
	val = spi_xchg_fast(SPI_DUMMY);
	val |= (u32)(spi_xchg_fast(SPI_DUMMY)) << 8;
	val |= (u32)(spi_xchg_fast(SPI_DUMMY)) << 16;
	val |= (u32)(spi_xchg_fast(SPI_DUMMY)) << 24;
	_xmem_end_bank(bank);
	return val;
}

static void xmem_w32(u8 bank, u16 addr, u32 val)
{
	_xmem_start_bank(XMEM_COMMAND_WRITE, bank, addr);
	spi_xchg_fast(val);
	spi_xchg_fast(val >> 8);
	spi_xchg_fast(val >> 16);
	spi_xchg_fast(val >> 24);
	_xmem_end_bank(bank);
}
*/
