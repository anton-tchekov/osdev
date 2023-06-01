/**
 * @file    ps2.h
 * @author  Haron Nazari, Anton Tchekov
 * @version 0.1
 * @date    16.05.2023
 * @brief   PS/2 Keyboard Driver with Interrupts
 */

#ifndef __PS2_H__
#define __PS2_H__

#include <types.h>
#include <avr/io.h>
#include <avr/interrupt.h>

/**
 * @brief Initialize PS/2 keyboard driver and interrupt, Interrupts must be
 *        enabled afterwards using `sei()`.
 */
void ps2_init(void);

#endif /* __PS2_H__ */
