/**
 * @file    logger.c
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    16.05.2023
 */

#include <logger.h>
#include <serial.h>

void log_boot(const char *msg)
{
	serial_tx_str_P("[INIT] ");
	serial_tx_str_P(msg);
	serial_tx('\n');
}
