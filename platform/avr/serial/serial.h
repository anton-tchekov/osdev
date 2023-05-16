/**
 * @file    serial.h
 * @author  Anton Tchekov
 * @version 0.1
 * @date    16.05.2023
 * @brief   Serial interface driver
 */

#ifndef __SERIAL_H__
#define __SERIAL_H__

void serial_init(void);
void serial_tx(char c);
void serial_tx_str(const char *s);
void serial_tx_str_P(const char *s);

#endif /* __SERIAL_H__ */
