/**
 * @file    format.h
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    23.04.2023
 * @brief   String formatting
 */

#ifndef __FORMAT_H__
#define __FORMAT_H__

#include <types.h>
#include <stdarg.h>

/**
 * @brief Formats and stores a series of characters and values in the provided
 *        character array buffer, according to the specified format string.
 *
 * @param str A pointer to the character array buffer where the resulting
 *            formatted string will be stored.
 * @param format A string that specifies how subsequent arguments are
 *               converted for output
 * @param ... A variable number of additional arguments to be formatted and
 *            inserted into the resulting string, according to the format string
 * @return The total number of characters written to the buffer, not including
 *         the terminating null character.
 */
i32 sprintf(char *str, const char *format, ...);

/**
 * @brief Formats and stores a series of characters and values in the provided
 *        character array buffer, according to the specified format string and
 *        a maximum buffer size
 *
 * @param str A pointer to the character array buffer where the resulting
 *            formatted string will be stored
 * @param size The maximum number of characters to be written to the buffer,
 *             including the terminating null character
 * @param format A string that specifies how subsequent arguments are converted
 *               for output
 * @param ... A variable number of additional arguments to be formatted and
 *            inserted into the resulting string, according to the format string
 * @return The total number of characters that would have been written to the
 *         buffer, if the output had not been truncated, not including the
 *         terminating null character.
 */
i32 snprintf(char *str, u32 size, const char *format, ...);


/**
 * @brief Formats and stores a series of characters and values in the provided
 *        character array buffer, according to the specified format string and
 *        a maximum buffer size, taking a va_list argument instead of variable
 *        arguments.
 *
 * @param str A pointer to the character array buffer where the resulting
 *            formatted string will be stored.
 * @param size The maximum number of characters to be written to the buffer,
 *             including the terminating null character.
 * @param format A string that specifies how subsequent arguments are converted
 *               for output.
 * @param arg A va_list object containing the variable arguments to be formatted
 *            and inserted into the resulting string, according to the format
 *            string.
 * @return The total number of characters that would have been written to the
 *         buffer, if the output had not been truncated, not including the
 *         terminating null character.
 */
i32 vsnprintf(char *str, u32 size, const char *format, va_list arg);

#endif /* __FORMAT_H__ */
