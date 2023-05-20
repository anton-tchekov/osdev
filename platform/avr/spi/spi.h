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
#include <status.h>
#include <avr/io.h>

/**
 * @brief Initialize the SPI interface
 */
void spi_init(void);

/**
 * @brief Enable fastest SPI mode
 */
void spi_fast(void);

/**
 * @brief Send and receive one byte from the SPI interface
 *
 * @param byte The byte to send
 * @return The byte that was received
 */
u8 spi_xchg(u8 byte);

/**
 * @brief TODO
 *
 * @param in
 * @param out
 * @return Status
 */
Status spi_xchg_try(u8 in, u8 *out);

/**
 * @brief TODO
 *
 * @param in
 * @return Status
 */
Status spi_tx_try(u8 in);

/**
 * @brief TODO
 *
 * @param out
 * @return Status
 */
Status spi_rx_try(u8 *out);

#endif /* __SPI_H__ */
