#ifndef SCANNER_H
#define SCANNER_H

#include <stddef.h>
#include <token.h>

typedef struct Scanner Scanner;

Scanner *createScanner(const char *source, size_t size);
void destroyScanner(Scanner *scanner);

int scanTokens(Scanner *scanner);
char *getLastErrorScanner(Scanner *scanner);

Token *getTokens(Scanner *scanner);
size_t getTokensCount(Scanner *scanner);

#endif // SCANNER_H