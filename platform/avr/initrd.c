/**
 * @file    initrd.c
 * @author  Anton Tchekov
 * @version 0.1
 * @date    21.05.2023
 * @brief   Load INIT program from reserved area on disk.
 *          This module contains a very rudimentary ATFS driver to do that.
 *          It is kind of a hack to avoid a chicken-egg problem because
 *          we want the filesystem driver to be in userspace but we can't load
 *          the FS driver without already having an FS driver.
 */

static u32 _read_32(u8 *buf)
{
	/* The cast is important for systems with small integer size */
	return (u32)(buf[0]) |
		((u32)(buf[1]) << 8) |
		((u32)(buf[2]) << 16) |
		((u32)(buf[3]) << 24);
}

static void _read_bootsector(u32 *init_start, u8 *init_sectors)
{
	u8 buf[SD_BLOCK_SIZE];
	u32 revision, init_size;

	if(sd_read(ATFS_SECTOR_BOOT, buf))
	{
		panic(PSTR("SD read error"));
	}

	/* Check signature */
	if(memcmp(buf + ATFS_OFFSET_SIGNATURE,
		_atfs_signature, sizeof(_atfs_signature)))
	{
		panic(PSTR("Wrong FS signature"));
	}

	/* Check revision */
	revision = _read_32(buf + ATFS_OFFSET_REVISION);
	if(revision != ATFS_REVISION)
	{
		panic(PSTR("Unsupported ATFS revision"));
	}

	/* Get INIT Start and Size */
	*init_start = _read_32(buf + ATFS_OFFSET_INIT_BLOCK);
	init_size = _read_32(buf + ATFS_OFFSET_INIT_SIZE);
	*init_sectors = (init_size + SD_BLOCK_SIZE - 1) >> SD_BLOCK_SIZE_POT;

	log_boot_P(LOG_INIT, PSTR("Loading Init binary from sector %"PRIu32""),
		*init_start);

	log_boot_P(LOG_INIT, PSTR("Init Size: %"PRIu32" bytes (%"PRIu8" sectors)"),
		init_size, *init_sectors);
}

/**
 * @brief Load the INIT program from special area on disk.
 *        The fully featured filesystem driver is in userspace.
 */
static void initrd_load(void)
{
	u32 init_start;
	u8 init_sectors;

	log_boot_P(LOG_INIT, PSTR("Loading file system"));
	_read_bootsector(&init_start, &init_sectors);
	if(task_start(TASK_GUI, init_start, init_sectors))
	{
		panic(PSTR("Loading Init failed"));
	}

	/* task_dump(0, init_sectors); */
}
