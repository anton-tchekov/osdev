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

/**
 * @brief This function must be implemented by the user and is called by the
 *        driver when a byte is received.
 *
 * @param byte The byte that was received from the PS/2 keyboard
 */
void ps2_event(u8 byte);

#endif /* __PS2_H__ */
