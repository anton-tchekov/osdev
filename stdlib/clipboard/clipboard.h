/**
 * @file    clipboard.h
 * @author  Tim Gabrikowski, Anton Tchekov
 * @version 0.1
 * @date    14.05.2023
 * @brief   Implement a OS-wide Clipboard
 */

#ifndef __CLIPBOARD_H__
#define __CLIPBOARD_H__

#include <types.h>

/**
 * @brief Save Text to the clipboard when the length is known
 *
 * @param text The text to copy
 * @param len The length of the text
 */
void clipboard_save_text_len(const char *text, i32 len);

/**
 * @brief Get textual data from clipboard
 *
 * @return Pointer to clipboard text data
 */
const char *clipboard_get_text(void);

/**
 * @brief Get the length of the text data in the clipboard
 *
 * @return Clipboard size in bytes
 */
i32 clipboard_get_text_len(void);

#endif /* __CLIPBOARD_H__ */