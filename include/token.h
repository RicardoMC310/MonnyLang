#ifndef TOKEN_H
#define TOKEN_H

#include <stdbool.h>
#include <string.h>

typedef enum
{
    TK_DISPLAY,
    TK_TRUE,
    TK_FALSE,
    TK_NIL,
    TK_SET,

    TK_LEFT_PAREN,
    TK_RIGHT_PAREN,
    TK_SEMICOLON,

    TK_IDENTIFIER,
    TK_STRING,
    TK_NUMBER,

    TK_EOF,
} TokenType;

typedef union
{
    double number;
    bool boolean;
    const char *string;
    void *ptr;
} Value;

typedef struct
{
    TokenType type;
    const char *lexeme;
    Value literal;
    int line;
} Token;

#endif // TOKEN_H