#include "monny/tokenizer/scanner.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

struct monny_scanner_t
{
    uint32_t *tokens;
    int tokens_quantity, tokens_capacity;
};

void monny_scanner_resize(monny_scanner_t *scanner)
{
    size_t old_capacity = scanner->tokens_capacity;
    size_t new_capacity = scanner->tokens_capacity == 0 ? 5 : scanner->tokens_capacity * 2;

    uint32_t *new_ptr = (uint32_t *)realloc(scanner->tokens, sizeof(monny_scanner_t) * new_capacity);
    if (!new_ptr)
    {
        perror("resize monny scanner");
        return;
    }

    scanner->tokens = new_ptr;

    memset(
        scanner->tokens + old_capacity,
        0,
        (new_capacity - old_capacity) * sizeof(monny_scanner_t));

    scanner->tokens_capacity = new_capacity;
}

monny_scanner_t *monny_create_scanner()
{
    monny_scanner_t *scanner = (monny_scanner_t *)calloc(1, sizeof(monny_scanner_t));
    if (!scanner)
    {
        perror("monny scanner");
        return NULL;
    }

    scanner->tokens_quantity = 0;
    scanner->tokens_capacity = 5;
    scanner->tokens = (uint32_t *)calloc(scanner->tokens_capacity, sizeof(uint32_t));
    if (!scanner->tokens)
    {
        perror("monny tokens");
        free(scanner);
        return NULL;
    }

    return scanner;
}

void monny_free_scanner(monny_scanner_t *scanner)
{
    if (!scanner)
        return;

    if (scanner->tokens)
        free(scanner->tokens);

    free(scanner);
    scanner = NULL;
}