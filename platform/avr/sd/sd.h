#ifndef __SD_H__
#define __SD_H__

#include "types.h"

/** TODO */
#define SD_CS_DIR  DDRX

/** TODO */
#define SD_CS_OUT  PORTX

/** TODO */
#define SD_PIN    9

void sd_init(void);
void sd_read(u32 block, void *data);
void sd_write(u32 block, const void *data);

#endif /* __SD_H__ */
