/**
 * @file    initrd.h
 * @author  Anton Tchekov
 * @version 0.1
 * @date    21.05.2023
 * @brief   Load INIT program and STDLIB from reserved area on disk.
 *          This module contains a very rudimentary ATFS driver to do that.
 *          It is kind of a hack to avoid a chicken-egg problem because
 *          we want the filesystem driver to be in userspace but we can't load
 *          the FS driver without already having an FS driver.
 */
#ifndef __INITRD_H__
#define __INITRD_H__

/**
 * @brief Load the INIT program and Standard Shared Library from special area
 *        on disk. The full featured filesystem driver is in userspace.
 */
void initrd_load(void);

#endif /* __INITRD_H__ */
