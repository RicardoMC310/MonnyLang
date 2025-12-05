#ifndef TOKENS_H
#define TOKENS_H

#include <stdint.h>

enum : uint32_t {
    MONNY_TK_EOF,
    MONNY_TK_NUMBER,
    MONNY_TK_STRING,
    MONNY_TK_NIL,

    MONNY_TK_DISPLAY,

    MONNY_TK_LEFTPAREN,
    MONNY_TK_RIGHTPAREN,
    MONNY_TK_SEMICOLON,
    MONNY_TK_COMMA,
};

#endif // TOKENS_H
