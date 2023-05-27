#ifndef __KEYWORD_H__
#define __KEYWORD_H__

void keyword_init(void);

bool keyword_detect(const char *str, u32 len);

#endif /* __KEYWORD_H__ */