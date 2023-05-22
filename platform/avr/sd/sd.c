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

/** SD go to idle state command */
#define CMD_GO_IDLE_STATE      0x00

/** SD send IF cond */
#define CMD_SEND_IF_COND       0x08

/** SD read card-specific data register */
#define CMD_SEND_CSD           0x09

/** SD read card identification number */
#define CMD_SEND_CID           0x0A

/** SD set block length command */
#define CMD_SET_BLOCKLEN       0x10

/** SD read single block command */
#define CMD_READ_SINGLE_BLOCK  0x11

/** SD write single block command */
#define CMD_WRITE_SINGLE_BLOCK 0x18

/** SDHC set OP cond */
#define CMD_SEND_OP_COND       0x01

/** SD set OP cond */
#define CMD_SD_SEND_OP_COND    0x29

/** SD send app command */
#define CMD_APP                0x37

/** SD read operation conditions register */
#define CMD_READ_OCR           0x3A

/** SD idle */
#define IDLE_STATE             (1 << 0)

/** SD illegal command */
#define ILLEGAL_CMD            (1 << 2)

/** SD card type 1 */
#define SD_1                   (1 << 0)

/** SD card type 2 */
#define SD_2                   (1 << 1)

/** SDHC card type */
#define SD_HC                  (1 << 2)

/* SD card properties */

/** SD card serial number */
static u32 _serial;

/** SD card capacity in 512-byte blocks */
static u32 _capacity;

/** SD card OEM name: 2 characters + null-terminator */
static u8 _oem[3];

/** SD card product name: 5 characters + null-terminator */
static u8 _product[6];

/** SD card type: SD1 / SD2 / SDHC */
static u8 _card_type;

/** SD manufacturer ID */
static u8 _manufacturer;

/** SD card revision number as packed BCD */
static u8 _revision;

/** SD card manufacturing year after 2000 */
static u8 _manufacturing_year;

/** SD card manufacturing month number */
static u8 _manufacturing_month;

/** SD card copy/original flag */
static u8 _flag_copy;

/** SD card write protected flag */
static u8 _flag_write_protect;

/** SD card temporarily write protected flag */
static u8 _flag_write_protect_temp;

/** SD card format ID */
static u8 _format;

/* --- PRIVATE --- */

/**
 * @brief Put SPI into slow mode for initialization
 */
static void _spi_configure_slow(void)
{
	SPCR |= (1 << SPR1) | (1 << SPR0);
	SPSR &= ~(1 << SPI2X);
}

/**
 * @brief Trigger an SD timeout panic
 */
static void _sd_timeout(void)
{
	SD_DESELECT;
	panic(PSTR("SD timeout"));
}

/**
 * @brief Trigger an SD error panic
 */
static void _sd_error(void)
{
	SD_DESELECT;
	panic(PSTR("SD error"));
}

/**
 * @brief Execute an SD command (panic if failed)
 *
 * @param cmd Command number
 * @param arg Address parameter
 * @return Output byte
 */
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

/**
 * @brief Try to execute an SD command
 *
 * @param cmd Command number
 * @param arg Address parameter
 * @param out Output byte
 * @return Status response
 */
static Status _sd_command_try(u8 cmd, u32 arg, u8 *out)
{
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
		RETURN_IF(spi_rx_try(&response));
		if(response != 0xFF)
		{
			break;
		}

		if(i >= 10)
		{
			return STATUS_TIMEOUT;
		}
	}

	*out = response;
	return STATUS_OK;
}

/**
 * @brief Convert block number to internal SD address
 *        since SD uses byte address but SDHC uses block number
 *
 * @param block Block number
 * @return Internal Address
 */
static u32 _sd_block_addr(u32 block)
{
	return _card_type & SD_HC ? block : (block << BLOCK_SIZE_POT);
}

/**
 * @brief Print SD card info to serial port
 */
static void _sd_info_print(void)
{
	log_boot_P(LOG_DEBUG, PSTR("Card Type        : %S"),
		_card_type & SD_HC ? PSTR("SDHC") : PSTR("SD"));

	log_boot_P(LOG_DEBUG, PSTR("Block Size       : %d"),
		BLOCK_SIZE);

	log_boot_P(LOG_DEBUG, PSTR("Manufacturer ID  : %02X"),
		_manufacturer);

	log_boot_P(LOG_DEBUG, PSTR("OEM              : %s"),
		_oem);

	log_boot_P(LOG_DEBUG, PSTR("Product Name     : %s"),
		_product);

	log_boot_P(LOG_DEBUG, PSTR("Revision         : %c.%c"),
		(_revision >> 4) + '0', (_revision & 0x0F) + '0');

	log_boot_P(LOG_DEBUG, PSTR("Serial Number    : 0x%08"PRIx32),
		_serial);

	log_boot_P(LOG_DEBUG, PSTR("Manufacture Date : %02d-%d"),
		_manufacturing_month, 2000 + _manufacturing_year);

	log_boot_P(LOG_DEBUG, PSTR("Capacity         : %"PRIu32" blocks"),
		_capacity);

	log_boot_P(LOG_OFFSET, PSTR("                   (%"PRIu32" bytes)"),
		_capacity * BLOCK_SIZE);

	log_boot_P(LOG_DEBUG, PSTR("Format           : 0x%02X"),
		_format);

	log_boot_P(LOG_DEBUG, PSTR("Flags            : %S%S"),
		_flag_copy ? PSTR("Copy, ") : PSTR("Original, "),
		_flag_write_protect_temp ? PSTR("Temporarily Write Protected") :
			(_flag_write_protect ? PSTR("Write Protected") :
			PSTR("Rewritable")));
}

/* --- PUBLIC --- */
void sd_init(void)
{
	log_boot_P(LOG_INIT, PSTR("SD driver starting"));

	{
		u8 response;
		u16 i;
		u32 arg;

		SD_DESELECT;
		_spi_configure_slow();
		_card_type = 0;
		for(i = 0; i < 10; ++i)
		{
			spi_xchg(0xFF);
		}

		SD_SELECT;
		for(i = 0; ; ++i)
		{
			if(_sd_command(CMD_GO_IDLE_STATE, 0) == IDLE_STATE)
			{
				break;
			}

			if(i == 0x1FF)
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

			if(i == 0x7FFF)
			{
				_sd_timeout();
			}
		}

		if(_card_type & SD_2)
		{
			if(_sd_command(CMD_READ_OCR, 0))
			{
				_sd_error();
			}

			if(spi_xchg(0xFF) & 0x40)
			{
				_card_type |= SD_HC;
			}

			spi_xchg(0xFF);
			spi_xchg(0xFF);
			spi_xchg(0xFF);
		}

		if(_sd_command(CMD_SET_BLOCKLEN, 512))
		{
			_sd_error();
		}

		SD_DESELECT;
		spi_fast();
		_delay_ms(20);
	}

	log_boot_P(LOG_INIT, PSTR("SD card initialized"));
	log_boot_P(LOG_INIT, PSTR("Reading SD card parameters"));

	{
		u8 i, b, csd_read_bl_len, csd_c_size_mult, csd_structure;
		u16 j, csd_c_size;

		spi_fast();
		SD_SELECT;

		/* Read CID register */
		_sd_command(CMD_SEND_CID, 0);
		j = 0;
		while(spi_xchg(0xFF) != 0xFE)
		{
			if(++j > 0x7FFF)
			{
				_sd_timeout();
			}
		}

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
				_sd_timeout();
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

	/* Print disk info */
	_sd_info_print();
}

u32 sd_size(void)
{
	return _capacity;
}

Status sd_read(u32 block, void *data)
{
	u16 i;
	u8 *data8, response;

	data8 = data;

	SD_SELECT;

	/* Start read */
	RETURN_IF(_sd_command_try(CMD_READ_SINGLE_BLOCK,
		_sd_block_addr(block), &response));
	if(response)
	{
		SD_DESELECT;
		return STATUS_FAIL;
	}

	/* Wait for ready */
	for(i = 0; ; ++i)
	{
		RETURN_IF(spi_xchg_try(0xFF, &response));
		if(response == 0xFE)
		{
			break;
		}

		if(i == 0xFFFF)
		{
			SD_DESELECT;
			return STATUS_TIMEOUT;
		}
	}

	/* Read data */
	for(i = 0; i < BLOCK_SIZE; ++i)
	{
		if(spi_rx_try(&data8[i]))
		{
			return STATUS_TIMEOUT;
		}
	}

	RETURN_IF(spi_tx_try(0xFF));
	RETURN_IF(spi_tx_try(0xFF));
	RETURN_IF(spi_tx_try(0xFF));
	SD_DESELECT;
	return STATUS_OK;
}
