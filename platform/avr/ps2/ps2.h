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

/* --- CLOCK: Arduino pin 2 --- */
/** Clock pin - Data direction register */
#define PS2_CLOCK_PORT_DIR  DDRB

/** Clock pin - Input register */
#define PS2_CLOCK_PORT_IN   PINB

/** Clock pin - Output register */
#define PS2_CLOCK_PORT_OUT  PORTB

/** Clock pin */
#define PS2_CLOCK_PIN       2

/** Clock pin - External interrupt */
#define PS2_EXT_INTERRUPT   INT0

/* --- DATA: Arduino pin 3 --- */
/** Data pin - Data direction register */
#define PS2_DATA_PORT_DIR   DDRB

/** Data pin - Input register */
#define PS2_DATA_PORT_IN    PINB

/** Data pin - Output register */
#define PS2_DATA_PORT_OUT   PORTB

/** Data pin */
#define PS2_DATA_PIN        3

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
