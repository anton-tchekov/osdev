/**
 * @file    serial.h
 * @author  Anton Tchekov
 * @version 0.1
 * @date    16.05.2023
 * @brief   Serial interface driver
 */

#ifndef __SERIAL_H__
#define __SERIAL_H__

/**
 * @brief Initialize the serial port
 */
void serial_init(void);

/**
 * @brief Print a character
 *
 * @param c The ASCII character to print
 */
void serial_tx(char c);

/**
 * @brief Print a null-terminated string
 *
 * @param s Pointer to string
 */
void serial_tx_str(const char *s);

/**
 * @brief Print a null-terminated string from PROGMEM
 *
 * @param s Pointer to string in program memory
 */
void serial_tx_str_P(const char *s);

#endif /* __SERIAL_H__ */
