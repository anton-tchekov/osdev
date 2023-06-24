#include <stdint.h>

typedef uint8_t u8;
typedef int8_t i8;
typedef int16_t i16;

#define CB_SIZE_POT 5
#define CB_SIZE      (1 << CB_SIZE_POT)
#define CB_SIZE_MASK (CB_SIZE - 1)

typedef struct
{
	u8 WP, RP, Buf[CB_SIZE];
} CB;

i8 cb_write(CB *cb, u8 byte)
{
	if(cb->WP == cb->RP) { return -1; }
	cb->Buf[cb->WP++] = byte;
	cb->WP &= CB_SIZE_MASK;
	return 0;
}

i16 cb_read(CB *cb)
{
	u8 byte;
	if(cb->RP == cb->WP) { return -1; }
	byte = cb->Buf[cb->RP++];
	cb->RP &= CB_SIZE_MASK;
	return byte;
}

int main(void)
{
	for(;;)
	{

	}

	return 0;
}
