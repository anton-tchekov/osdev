/**
 * @file    sd.c
 * @author  Anton Tchekov
 * @version 0.2
 * @date    20.05.2023
 */

#include <sd.h>
#include <spi.h>
#include <logger.h>
#include <gpio.h>
#include <string.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#define CMD_GO_IDLE_STATE      0x00
#define CMD_SEND_OP_COND       0x01
#define CMD_SEND_IF_COND       0x08
#define CMD_SEND_CSD           0x09
#define CMD_SEND_CID           0x0A
#define CMD_SET_BLOCKLEN       0x10
#define CMD_READ_SINGLE_BLOCK  0x11
#define CMD_WRITE_SINGLE_BLOCK 0x18
#define CMD_SD_SEND_OP_COND    0x29
#define CMD_APP                0x37
#define CMD_READ_OCR           0x3A

#define IDLE_STATE             (1 << 0)
#define ILLEGAL_CMD            (1 << 2)

#define BLOCK_SIZE             512

#define SD_1                   (1 << 0)
#define SD_2                   (1 << 1)
#define SD_HC                  (1 << 2)

/* SD card properties */
static u32
	_serial,
	_capacity;

static u8
	_oem[3],
	_product[6],
	_card_type,
	_manufacturer,
	_revision,
	_manufacturing_year,
	_manufacturing_month,
	_flag_copy,
	_flag_write_protect,
	_flag_write_protect_temp,
	_format;

static u8 _sd_command(u8 cmd, u32 arg)
{
	u8 i, response;
	spi_xchg(0xFF);
	spi_xchg(0x40 | cmd);
	spi_xchg((arg >> 24) & 0xFF);
	spi_xchg((arg >> 16) & 0xFF);
	spi_xchg((arg >> 8) & 0xFF);
	spi_xchg((arg >> 0) & 0xFF);
	switch(cmd)
	{
	case CMD_GO_IDLE_STATE:
		spi_xchg(0x95);
		break;

	case CMD_SEND_IF_COND:
		spi_xchg(0x87);
		break;

	default:
		spi_xchg(0xFF);
		break;
	}

	for(i = 0; i < 10 && ((response = spi_xchg(0xFF)) == 0xFF); ++i) ;
	return response;
}

static void _spi_configure_slow(void)
{
	SPCR |= (1 << SPR1) | (1 << SPR0);
	SPSR &= ~(1 << SPI2X);
}

static void _sd_timeout(void)
{
	SD_DESELECT;
	panic(PSTR("SD timeout"));
}

static void _sd_error(void)
{
	SD_DESELECT;
	panic(PSTR("SD error"));
}

void sd_init(void)
{
	log_boot_P(PSTR("SD driver starting"));

	{
		u8 response;
		u16 i;
		u32 arg;

		_card_type = 0;
		_spi_configure_slow();
		SD_SELECT;
		for(i = 0; ; ++i)
		{
			if(_sd_command(CMD_GO_IDLE_STATE, 0) == IDLE_STATE)
			{
				break;
			}

			if(i >= 0x1FF)
			{
				_sd_timeout();
			}
		}

		if((_sd_command(CMD_SEND_IF_COND, 0x1AA) & ILLEGAL_CMD) == 0)
		{
			spi_xchg(0xFF);
			spi_xchg(0xFF);
			if(((spi_xchg(0xFF) & 0x01) == 0) ||
				(spi_xchg(0xFF) != 0xAA))
			{
				_sd_error();
			}

			_card_type |= SD_2;
		}
		else
		{
			_sd_command(CMD_APP, 0);
			if((_sd_command(CMD_SD_SEND_OP_COND, 0) & ILLEGAL_CMD) == 0)
			{
				_card_type |= SD_1;
			}
		}

		for(i = 0; ; ++i)
		{
			if(_card_type & (SD_1 | SD_2))
			{
				arg = 0;
				if(_card_type & SD_2)
				{
					arg = 0x40000000;
				}

				_sd_command(CMD_APP, 0);
				response = _sd_command(CMD_SD_SEND_OP_COND, arg);
			}
			else
			{
				response = _sd_command(CMD_SEND_OP_COND, 0);
			}

			if((response & IDLE_STATE) == 0)
			{
				break;
			}

			if(i >= 0x7FFF)
			{
				_sd_timeout();
			}
		}

		if(_card_type & SD_2)
		{
			_sd_command(CMD_READ_OCR, 0);
			if(spi_xchg(0xFF) & 0x40)
			{
				_card_type |= SD_HC;
			}

			spi_xchg(0xFF);
			spi_xchg(0xFF);
			spi_xchg(0xFF);
		}

		_sd_command(CMD_SET_BLOCKLEN, BLOCK_SIZE);
		SD_DESELECT;
		_delay_ms(20);
	}

	{
		u8 i, b, csd_read_bl_len, csd_c_size_mult, csd_structure;
		u16 j, csd_c_size;

		spi_fast();
		SD_SELECT;

		/* Read CID register */
		_sd_command(CMD_SEND_CID, 0);
		while(spi_xchg(0xFF) != 0xFE) {}

		for(i = 0; i < 18; ++i)
		{
			b = spi_xchg(0xFF);
			switch(i)
			{
				case 0:
					_manufacturer = b;
					break;

				case 1:
				case 2:
					_oem[i - 1] = b;
					break;

				case 3:
				case 4:
				case 5:
				case 6:
				case 7:
					_product[i - 3] = b;
					break;

				case 8:
					_revision = b;
					break;

				case 9:
				case 10:
				case 11:
				case 12:
					_serial |= (u32)b << ((12 - i) * 8);
					break;

				case 13:
					_manufacturing_year = b << 4;
					break;

				case 14:
					_manufacturing_year |= b >> 4;
					_manufacturing_month = b & 0x0f;
					break;
			}
		}

		/* Read CSD register */
		csd_read_bl_len = 0;
		csd_c_size_mult = 0;
		csd_structure = 0;
		csd_c_size = 0;

		_sd_command(CMD_SEND_CSD, 0);
		j = 0;
		while(spi_xchg(0xFF) != 0xFE)
		{
			if(++j > 0x7FFF)
			{
				//_sd_timeout();
			}
		}

		for(i = 0; i < 18; ++i)
		{
			b = spi_xchg(0xFF);
			if(i == 0)
			{
				csd_structure = b >> 6;
			}
			else if(i == 14)
			{
				if(b & 0x40)
				{
					_flag_copy = 1;
				}

				if(b & 0x20)
				{
					_flag_write_protect = 1;
				}

				if(b & 0x10)
				{
					_flag_write_protect_temp = 1;
				}

				_format = (b & 0x0C) >> 2;
			}
			else
			{
				if(csd_structure == 0x01)
				{
					switch(i)
					{
						case 7:
							b &= 0x3f;

						case 8:
						case 9:
							csd_c_size <<= 8;
							csd_c_size |= b;
							++csd_c_size;
							_capacity = (u32)csd_c_size << 10;
					}
				}
				else if(csd_structure == 0x00)
				{
					switch(i)
					{
						case 5:
							csd_read_bl_len = b & 0x0F;
							break;

						case 6:
							csd_c_size = b & 0x03;
							csd_c_size <<= 8;
							break;

						case 7:
							csd_c_size |= b;
							csd_c_size <<= 2;
							break;

						case 8:
							csd_c_size |= b >> 6;
							++csd_c_size;
							break;

						case 9:
							csd_c_size_mult = b & 0x03;
							csd_c_size_mult <<= 1;
							break;

						case 10:
							csd_c_size_mult |= b >> 7;
							_capacity = ((u32)csd_c_size <<
								(csd_c_size_mult + csd_read_bl_len + 2)) >> 9;
							break;
					}
				}
			}
		}

		SD_DESELECT;
	}

	log_boot_P(PSTR("SD card detected and initialized"));

	/* Print disk info */
	log_boot_P(PSTR("Card Type          : %S"),
		_card_type & SD_HC ? PSTR("SDHC") : PSTR("SD"));

	log_boot_P(PSTR("Block Size         : %d"),
		BLOCK_SIZE);

	log_boot_P(PSTR("Manufacturer ID    : %02X"),
		_manufacturer);

	log_boot_P(PSTR("OEM                : %s"),
		_oem);

	log_boot_P(PSTR("Product Name       : %s"),
		_product);

	log_boot_P(PSTR("Revision           : %c.%c"),
		(_revision >> 4) + '0', (_revision & 0x0F) + '0');

	log_boot_P(PSTR("Serial Number      : 0x%08lX"),
		_serial);

	log_boot_P(PSTR("Manufacture Date   : %02d-%d"),
		_manufacturing_month, 2000 + _manufacturing_year);

	log_boot_P(PSTR("Capacity           : %ld bytes"),
		_capacity);

	log_boot_P(PSTR("Format             : 0x%02X"),
		_format);

	log_boot_P(PSTR("Flags              : %S%S"),
		_flag_copy ? PSTR("Copy, ") : PSTR("Original, "),
		_flag_write_protect_temp ? PSTR("Temporarily Write Protected") :
			(_flag_write_protect ? PSTR("Write Protected") :
			PSTR("Rewritable")));
}








#ifdef COMMENT

static Status _sd_try_command(u8 cmd, u32 arg, u8 *out)
{
	/* TODO */
	u8 i, response;
	RETURN_IF(spi_tx_try(0xFF));
	RETURN_IF(spi_tx_try(0x40 | cmd));
	RETURN_IF(spi_tx_try(arg >> 24));
	RETURN_IF(spi_tx_try(arg >> 16));
	RETURN_IF(spi_tx_try(arg >> 8));
	RETURN_IF(spi_tx_try(arg));
	switch(cmd)
	{
	case CMD_GO_IDLE_STATE:
		RETURN_IF(spi_tx_try(0x95));
		break;

	case CMD_SEND_IF_COND:
		RETURN_IF(spi_tx_try(0x87));
		break;

	default:
		RETURN_IF(spi_tx_try(0xFF));
		break;
	}

	for(i = 0; ; ++i)
	{
		if(i >= 10)
		{
			return STATUS_TIMEOUT;
		}

		RETURN_IF(spi_rx_try(&response));
		if(response != 0xFF)
		{
			break;
		}
	}

	*out = response;
	return STATUS_OK;
}

static Status _block_access(u8 cmd, u32 block)
{
	u8 ret;
	_sd_select();
	if(_command(cmd, _card_type & SD_HC ? block : (block >> 9), &ret))
	{
		SD_DESELECT;
		return STATUS_FAIL;
	}

	return STATUS_OK;
}

Status sd_read(u32 block, void *data)
{
	u16 i;
	u8 out;
	u8 *buf8;

	buf8 = data;
	RETURN_IF(_block_access(CMD_READ_SINGLE_BLOCK, block));
	for(i = 0; ; ++i)
	{
		RETURN_IF(spi_rx_try(&out));
		if(out == 0xFE)
		{
			break;
		}

		if(i == 0xFFFF)
		{
			SD_DESELECT;
			return STATUS_TIMEOUT;
		}
	}

	for(i = 0; i < BLOCK_SIZE; ++i)
	{
		RETURN_IF(spi_rx_try(&out));
		*buf8++ = out;
	}

	RETURN_IF(spi_tx_try(0xFF));
	RETURN_IF(spi_tx_try(0xFF));
	SD_DESELECT;
	/* This seems unnecessary since after deselect? */
	/* RETURN_IF(spi_tx_try(0xFF, &out)); */
	return STATUS_OK;
}

Status sd_write(u32 block, const void *data)
{
	u16 i;
	u8 out;
	const u8 *buf8;

	buf8 = data;
	RETURN_IF(_block_access(CMD_WRITE_SINGLE_BLOCK, block));
	RETURN_IF(spi_tx_try(0xFE));
	for(i = 0; i < BLOCK_SIZE; ++i)
	{
		RETURN_IF(spi_tx_try(*buf8++));
	}

	RETURN_IF(spi_tx_try(0xFF));
	RETURN_IF(spi_tx_try(0xFF));
	do
	{
		RETURN_IF(spi_rx_try(&out));
		/*TIMEOUT11!*/
	}
	while(out != 0xFF);

	RETURN_IF(spi_tx_try(0xFF));
	SD_DESELECT;
	return STATUS_OK;
}

#endif
