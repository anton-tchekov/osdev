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

/** Clipboard content type */
typedef enum
{
	/** Textual data */
	CLIPBOARD_TYPE_TEXT
} ClipboardType;

/**
 * @brief Save Text to clipboard
 *
 * @param text The text to copy
*/
void clipboard_save_text(const char *text);

/**
 * @brief Save Text to the clipboard when the length is known
 *
 * @param text The text to copy
 * @param len The length of the text
 */
void clipboard_save_text_len(const char *text, i32 len);

/**
 * @brief Get data from clipboard
 *
 * @return Pointer to raw clipboard data
 */
const void *clipboard_get(void);

/**
 * @brief Get the length of the data in the clipboard
 *
 * @return Clipboard size in bytes
 */
i32 clipboard_len(void);

/**
 * @brief   Get the stored Datatype from the clipboard
 */
ClipboardType clipboard_type(void);

#endif /* __CLIPBOARD_H__ */