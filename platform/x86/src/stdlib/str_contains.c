#include "str_contains.h"
#include "ctype.h"

bool str_contains(char* s, char* t) {
    i32 string_length = strlen(s);
    i32 search_term_length = strlen(t);

    for (size_t i = 0; i <= string_length - search_term_length; i++) {
        bool match = true;

        for (size_t j = 0; j < search_term_length; j++) {
            if (tolower(s[i + j]) != tolower(t[j])) {
                match = false;
                break;
            }
        }

        if (match) {
            return true;
        }
    }

    return false;
}