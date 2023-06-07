/**
 * @file    progmem.h
 * @author  Anton Tchekov
 * @version 0.1
 * @date    07.06.2023
 * @brief   Data in Program Memory
 */
#ifndef __PROGMEM_H__
#define __PROGMEM_H__

/** PSTR mock */
#define PSTR(X) X

/** PROGMEM mock */
#define PROGMEM

/** pgm_read_byte mock */
#define pgm_read_byte(x) *(x)

#endif /* __PROGMEM_H__ */
