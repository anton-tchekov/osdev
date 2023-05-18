/**
 * @file    spi.h
 * @author  Anton Tchekov
 * @version 0.1
 * @date    16.05.2023
 * @brief   SPI interface driver
 */

#ifndef __SPI_H__
#define __SPI_H__

#include <types.h>
#include <avr/io.h>

#define SPI_DIR   DDRB
#define SPI_MOSI  3
#define SPI_MISO  4
#define SPI_SCK   5

/**
 * @brief Initialize the SPI interface
 */
void spi_init(void);

/**
 * @brief Send and receive one byte from the SPI interface
 *
 * @param byte The byte to send
 * @return The byte that was received
 */
u8 spi_xchg(u8 byte);

#endif /* __SPI_H__ */
