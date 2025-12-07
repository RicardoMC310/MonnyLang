#ifndef SCANNER_H
#define SCANNER_H

#include "monny/tokenizer/tokens.h"

typedef struct monny_scanner_t monny_scanner_t;

monny_scanner_t *monny_create_scanner();
void monny_free_scanner(monny_scanner_t *scanner);

monny_tokens_t *monny_scan_tokens(monny_scanner_t *scanner, char *buffer_chars, int buffer_len);

#endif // SCANNER_H