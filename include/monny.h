#ifndef _MONNY_H_
#define _MONNY_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct MonnyState MonnyState;

MonnyState* monny_new(void);
void monny_close(MonnyState *M);

int monny_load_file(MonnyState *M, const char *filename);
char* monny_get_error(MonnyState *M);

#endif // _MONNY_H_