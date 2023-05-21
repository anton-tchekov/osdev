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
 *        Panics on timeout
 *
 * @param byte The byte to send
 * @return The byte that was received
 */
u8 spi_xchg(u8 byte);

/**
 * @brief Try to send and receive one byte from SPI
 *
 * @param in Input byte
 * @param out Output byte
 * @return Status
 */
Status spi_xchg_try(u8 in, u8 *out);

/**
 * @brief Try to send one byte to the SPI interface
 *
 * @param in The byte to send
 * @return Status
 */
Status spi_tx_try(u8 in);

/**
 * @brief Try to receive one byte from the SPI interface
 *
 * @param out Output byte
 * @return Status
 */
Status spi_rx_try(u8 *out);

#endif /* __SPI_H__ */
