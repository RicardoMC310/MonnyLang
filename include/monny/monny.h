#ifndef MONNY_H
#define MONNY_H

#define MONNY_OK 0
#define MONNY_ERROR 1

typedef struct monny_state_t monny_state_t;

monny_state_t *monny_create_state();
void monny_free_state(monny_state_t *state);

int monny_load_file(monny_state_t *state, char * path);

#endif // MONNY_H