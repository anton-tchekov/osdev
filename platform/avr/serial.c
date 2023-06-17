/**
 * @file    serial.c
 * @author  Anton Tchekov
 * @version 0.1
 * @date    16.05.2023
 * @brief   Serial interface driver
 */

/** UART Baudrate */
#define UART_BAUD        9600

/** UART Prescaler */
#define UART_PRESCALER       (((F_CPU / (UART_BAUD * 16UL))) - 1)

/**
 * @brief Initialize the serial port
 */
static void serial_init(void)
{
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
	UBRR0 = UART_PRESCALER;
}

/**
 * @brief Print a character
 *
 * @param c The ASCII character to print
 */
static void serial_tx(char c)
{
	while(!(UCSR0A & (1 << UDRE0))) {}
	UDR0 = c;
}

/**
 * @brief Print a null-terminated string
 *
 * @param s Pointer to string
 */
static void serial_tx_str(const char *s)
{
	char c;
	while((c = *s++))
	{
		serial_tx(c);
	}
}

static void serial_tx_data(const void *data, u8 len)
{
	u8 i;
	const char *data8 = data;
	for(i = 0; i < len; ++i)
	{
		serial_tx(data8[i]);
	}
}

/**
 * @brief Print a null-terminated string from PROGMEM
 *
 * @param s Pointer to string in program memory
 */
static void serial_tx_str_P(const char *s)
{
	char c;
	while((c = pgm_read_byte(s++)))
	{
		serial_tx(c);
	}
}
