

#if \
	defined(__AVR_ATmega8__) || \
	defined(__AVR_ATmega48__) || \
	defined(__AVR_ATmega48P__) || \
	defined(__AVR_ATmega88__) || \
	defined(__AVR_ATmega88P__) || \
	defined(__AVR_ATmega168__) || \
	defined(__AVR_ATmega168P__) || \
	defined(__AVR_ATmega328P__)

#define SPI_DIR                DDRB
#define SPI_OUT                PORTB
#define MOSI                   (1 << 3)
#define SCK                    (1 << 5)
#define CS                     (1 << 2)
#define MISO                   (1 << 4)

#elif \
	defined(__AVR_ATmega16__) || \
	defined(__AVR_ATmega32__)

#define SPI_DIR                DDRB
#define SPI_OUT                PORTB
#define MOSI                   (1 << 5)
#define SCK                    (1 << 7)
#define CS                     (1 << 4)
#define MISO                   (1 << 6)

#elif \
	defined(__AVR_ATmega64__) || \
	defined(__AVR_ATmega128__) || \
	defined(__AVR_ATmega169__)

#define SPI_DIR                DDRB
#define SPI_OUT                PORTB
#define MOSI                   (1 << 2)
#define SCK                    (1 << 1)
#define CS                     (1 << 0)
#define MISO                   (1 << 3)

#else
#error "Target AVR not supported: No pin mappings available."
#endif

u8 spi_xchg(u8 b)
{
	SPDR = b;
	while(!(SPSR & (1 << SPIF))) ;
	SPSR &= ~(1 << SPIF);
	return SPDR;
}
