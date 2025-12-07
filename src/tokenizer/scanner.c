#include "monny/tokenizer/scanner.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#ifndef MONNY_DEBUG_TOKENS
#endif

struct monny_scanner_t
{
    monny_tokens_t *tokens;
    int tokens_quantity;
    int tokens_capacity;

    char *buffer_chars;
    int current;
    int start;
    int buffer_length;
};

static void monny_tokens_resize(monny_scanner_t *scanner)
{
    int old = scanner->tokens_capacity;
    int newcap = (old == 0) ? 16 : old * 2;

    if (newcap > 1 << 20)
    {
        newcap = old + 4096;
    }

    monny_tokens_t *new_ptr = (monny_tokens_t *)realloc(scanner->tokens, sizeof(monny_tokens_t) * newcap);
    if (!new_ptr)
    {
        perror("monny: realloc tokens");
        return;
    }

    if (newcap > old)
    {
        memset(new_ptr + old, 0, (newcap - old) * sizeof(monny_tokens_t));
    }

    scanner->tokens = new_ptr;
    scanner->tokens_capacity = newcap;
}

monny_scanner_t *monny_create_scanner(void)
{
    monny_scanner_t *scanner = (monny_scanner_t *)calloc(1, sizeof(monny_scanner_t));
    if (!scanner)
        return NULL;

    scanner->tokens_quantity = 0;
    scanner->tokens_capacity = 16;
    scanner->tokens = (monny_tokens_t *)calloc(scanner->tokens_capacity, sizeof(monny_tokens_t));
    if (!scanner->tokens)
    {
        free(scanner);
        return NULL;
    }

    scanner->buffer_chars = NULL;
    scanner->current = 0;
    scanner->start = 0;
    scanner->buffer_length = 0;

    return scanner;
}

void monny_free_scanner(monny_scanner_t *scanner)
{
    if (!scanner)
        return;

    if (scanner->tokens)
    {

        free(scanner->tokens);
    }

    free(scanner);
}

void monny_add_token(monny_scanner_t *scanner, monny_tokens_e type, int start, int length)
{
    if (scanner->tokens_quantity >= scanner->tokens_capacity)
        monny_tokens_resize(scanner);

    monny_tokens_t t;
    t.type = type;
    t.start = start;
    t.length = length;
    scanner->tokens[scanner->tokens_quantity++] = t;
}

static inline bool monny_is_at_end_char(monny_scanner_t *scanner)
{
    return scanner->current >= scanner->buffer_length;
}

static inline char monny_advance_char(monny_scanner_t *scanner)
{
    if (monny_is_at_end_char(scanner))
        return '\0';
    return scanner->buffer_chars[scanner->current++];
}

static inline char monny_peek_character(monny_scanner_t *scanner)
{
    if (monny_is_at_end_char(scanner))
        return '\0';
    return scanner->buffer_chars[scanner->current];
}

static inline char monny_peek_next_character(monny_scanner_t *scanner)
{
    if (scanner->current + 1 >= scanner->buffer_length)
        return '\0';
    return scanner->buffer_chars[scanner->current + 1];
}

static inline bool monny_is_alpha_character(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

static inline bool monny_is_digit_character(char c)
{
    return (c >= '0' && c <= '9');
}

static inline bool monny_is_alpha_numeric_character(char c)
{
    return monny_is_alpha_character(c) || monny_is_digit_character(c);
}

static void monny_digit_character(monny_scanner_t *scanner)
{
    while (monny_is_digit_character(monny_peek_character(scanner)))
        monny_advance_char(scanner);

    if (monny_peek_character(scanner) == '.' && monny_is_digit_character(monny_peek_next_character(scanner)))
    {
        monny_advance_char(scanner);
        while (monny_is_digit_character(monny_peek_character(scanner)))
            monny_advance_char(scanner);
    }

    int start = scanner->start;
    int length = scanner->current - scanner->start;

    monny_add_token(scanner, MONNY_TK_NUMBER, start, length);
}

static void monny_string_character(monny_scanner_t *scanner)
{
    char quote = scanner->buffer_chars[scanner->start];

    while (monny_peek_character(scanner) != quote && !monny_is_at_end_char(scanner))
    {
        if (monny_peek_character(scanner) == '\\')
        {
            monny_advance_char(scanner);
            if (!monny_is_at_end_char(scanner))
                monny_advance_char(scanner);
            continue;
        }
        monny_advance_char(scanner);
    }

    if (monny_is_at_end_char(scanner))
    {
        int s = scanner->start + 1;
        int len = scanner->current - scanner->start - 1;
        monny_add_token(scanner, MONNY_TK_STRING, s, len);
        return;
    }

    monny_advance_char(scanner);

    int s = scanner->start + 1;
    int len = scanner->current - scanner->start - 2;
    monny_add_token(scanner, MONNY_TK_STRING, s, len);
}

static void monny_identifier_character(monny_scanner_t *scanner)
{
    while (monny_is_alpha_numeric_character(monny_peek_character(scanner)))
        monny_advance_char(scanner);

    int start = scanner->start;
    int length = scanner->current - scanner->start;

    struct keyword_t
    {
        const char *name;
        monny_tokens_e token;
    };
    static const struct keyword_t keywords[] = {
        {"display", MONNY_TK_DISPLAY},
        {NULL, (monny_tokens_e)0}};

    for (int i = 0; keywords[i].name != NULL; ++i)
    {
        size_t klen = strlen(keywords[i].name);
        if ((int)klen == length &&
            strncmp(scanner->buffer_chars + start, keywords[i].name, klen) == 0)
        {
            monny_add_token(scanner, keywords[i].token, start, length);
            return;
        }
    }

    monny_add_token(scanner, MONNY_TK_IDENTIFIER, start, length);
}

static void monny_scan_token(monny_scanner_t *scanner)
{
    char c = monny_advance_char(scanner);
    switch (c)
    {
    case ' ':
    case '\t':
    case '\r':
    case '\n':
        return;
    case '(':
        monny_add_token(scanner, MONNY_TK_LEFTPAREN, scanner->start, 1);
        return;
    case ')':
        monny_add_token(scanner, MONNY_TK_RIGHTPAREN, scanner->start, 1);
        return;
    case ';':
        monny_add_token(scanner, MONNY_TK_SEMICOLON, scanner->start, 1);
        return;
    case ',':
        monny_add_token(scanner, MONNY_TK_COMMA, scanner->start, 1);
        return;
    case '\'':
    case '\"':
        monny_string_character(scanner);
        return;
    default:
        if (monny_is_digit_character(c))
        {
            monny_digit_character(scanner);
            return;
        }
        else if (monny_is_alpha_character(c))
        {
            monny_identifier_character(scanner);
            return;
        }
        else
        {
            return;
        }
    }
}

const char *monny_token_name(monny_tokens_e type)
{
    switch (type)
    {
    case MONNY_TK_EOF:
        return "EOF";
    case MONNY_TK_NUMBER:
        return "NUMBER";
    case MONNY_TK_STRING:
        return "STRING";
    case MONNY_TK_IDENTIFIER:
        return "IDENTIFIER";
    case MONNY_TK_NIL:
        return "NIL";
    case MONNY_TK_DISPLAY:
        return "DISPLAY";
    case MONNY_TK_LEFTPAREN:
        return "LEFT_PAREN";
    case MONNY_TK_RIGHTPAREN:
        return "RIGHT_PAREN";
    case MONNY_TK_SEMICOLON:
        return "SEMICOLON";
    case MONNY_TK_COMMA:
        return "COMMA";
    default:
        return "UNKNOWN";
    }
}

monny_tokens_t *monny_scan_tokens(monny_scanner_t *scanner, char *buffer_chars, int buffer_len)
{
    if (!scanner || !buffer_chars)
        return NULL;

    scanner->buffer_chars = buffer_chars;
    scanner->buffer_length = buffer_len >= 0 ? buffer_len : (int)strlen(buffer_chars);
    scanner->current = 0;
    scanner->start = 0;
    scanner->tokens_quantity = 0;

    while (!monny_is_at_end_char(scanner))
    {
        scanner->start = scanner->current;
        monny_scan_token(scanner);
    }

    monny_add_token(scanner, MONNY_TK_EOF, scanner->current, 0);

#ifdef MONNY_DEBUG_TOKENS
    for (int i = 0; i < scanner->tokens_quantity; ++i)
    {
        monny_tokens_t *t = &scanner->tokens[i];
        const char *lexptr = "(null)";
        int lexlen = 0;
        if (t->length > 0 && t->start >= 0)
        {
            lexptr = scanner->buffer_chars + t->start;
            lexlen = t->length;
        }

        printf("[%02d] %-12s | %-.*s\n", i, monny_token_name(t->type), lexlen, lexptr);
    }
#endif

    return scanner->tokens;
}
