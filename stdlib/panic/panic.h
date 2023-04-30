/**
 * @file    panic.h
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    23.04.2023
 * @brief   Panic function to stop the program when
 *          an unrecoverable error occurs
 */

#ifndef __PANIC_H__
#define __PANIC_H__

#include <types.h>

/**
 * @brief Panic function. Do not call this function directly.
 *        Use the PANIC and ASSERT macros below.
 *
 * @param msg Error message
 * @param file File in which the error occurred
 * @param line Line in which the error occurred
 */
void panic(const char *msg, const char *file, u32 line);

/**
 * @brief Crashes the program in case of an internal inconsistency.
 *        Prints the error message, line and file where the error
 *        occurred.
 *
 * @param msg Error message
 */
#define PANIC(msg) panic(msg, __FILE__, __LINE__)

/**
 * @brief If the condition is false, a PANIC is triggered.
 *
 * @param cond Boolean condition
 * @param msg Error message
 */
#define ASSERT(cond, msg) do { if(!cond) { PANIC(msg); } } while(0)

#endif /* __PANIC_H__ */
