#ifndef __CLIPBOARD_H__
#define __CLIPBOARD_H__

#include <types.h>

typedef enum {
    CLIPBOARD_TYPE_TEXT
} ClipboardType;

void clipboard_save_text(const char *text);

void clipboard_save_text_len(const char *text, i32 len);

const void *clipboard_get(void);

i32 clipboard_len(void);

ClipboardType clipboard_type(void);

#endif /* __CLIPBOARD_H__ */