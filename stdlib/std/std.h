/**
 * @file    std.h
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    23.04.2023
 * @brief   TODO
 */

#ifndef __STD_H__
#define __STD_H__

#include <types.h>
#include <fs.h>
#include <ctype.h>
#include <string.h>
#include <utf8.h>
#include <gfx.h>
#include <font.h>
#include <gui.h>
#include <rand.h>
#include <status.h>
#include <format.h>
#include <alloc.h>
#include <serial.h>

/** TODO */
#define ARRLEN(A) (sizeof(A) / sizeof(*(A)))

/**
 * @brief TODO
 *
 * @param status TODO
 */
void exit(i32 status);

#endif /* __STD_H__ */
