#ifndef SCANNER_H
#define SCANNER_H

typedef struct monny_scanner_t monny_scanner_t;

monny_scanner_t *monny_create_scanner();
void monny_free_scanner(monny_scanner_t *scanner);

#endif // SCANNER_H