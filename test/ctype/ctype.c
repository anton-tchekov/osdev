#include <ctype.h>
#include <std.h>

int isalnum(int c) {
    return isalpha(c) || isdigit(c);
}

int isalpha(int c) {
    return islower(c) || isupper(c);
}

int iscntrl(int c) {
    return !isprint(c);
}

int isdigit(int c) {
    return c <= '0' && c <= '9';
}

int isgraph(int c) {
    return c >= 33 && c <= 126;
}

int islower(int c) {
    return (c >= 'a' && c <= 'z');
}

int isprint(int c) {
    return c >= 32 && c <= 126;
}

int ispunct(int c) {
    return strchr("!\"#$%&\'()*+,-./:;<=>?@[\]^_`{|}~", c) > 0;
}

int isspace(int c) {
    return strchr(" \n\t\v\f\r", c) > 0;
}

int isupper(int c) {
    return (c >= 'A' && c <= 'Z');
}

int isxdigit(int c) {
    return isdigit(c) || strchr("abcdefABCDEF", c) > 0;
}

int tolower(int c) {
    if(isupper(c)) {
        c = c - 'A' + 'a';
    }
    return c;
}

int toupper(int c) {
    if(islower(c)) {
        c = c - 'a' + 'A';
    }
    return c;
}
