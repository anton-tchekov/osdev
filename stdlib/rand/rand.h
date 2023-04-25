/**
 * @file    rand.h
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    23.04.2023
 * @brief   (Pseudo-) Random number generation
 */

#ifndef __RAND_H__
#define __RAND_H__

#include <types.h>

/**
 * @brief Returns a pseudo-random integer.
 *        Generator is seeded with a hardware noise source
 */
i32 rand(void);

#endif /* __RAND_H__ */
