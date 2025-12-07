#ifndef TOKENS_H
#define TOKENS_H

#include "monny/values.h"

#include <stdint.h>

typedef enum : uint32_t
{
    MONNY_TK_EOF,
    MONNY_TK_NUMBER,
    MONNY_TK_STRING,
    MONNY_TK_IDENTIFIER,
    MONNY_TK_NIL,

    MONNY_TK_DISPLAY,

    MONNY_TK_LEFTPAREN,
    MONNY_TK_RIGHTPAREN,
    MONNY_TK_SEMICOLON,
    MONNY_TK_COMMA,
} monny_tokens_e;

typedef struct
{
    unsigned type : 8;
    unsigned start : 24;
    unsigned length : 16;
} monny_tokens_t;

#endif // TOKENS_H
