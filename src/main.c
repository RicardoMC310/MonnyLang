#include "monny/monny.h"

#include <stdio.h>

int main(int argc, char **argv)
{
    monny_state_t *state = monny_create_state();

    if (argc < 2)
    {
        printf("[ERROR]: Usage %s <nome_do_arquivo>.mn", argv[0]);
        monny_free_state(state);
        return MONNY_ERROR;
    }

    if (monny_load_file(state, argv[1]) == MONNY_ERROR)
    {
        monny_free_state(state);
        return MONNY_ERROR;
    }

    monny_free_state(state);
    return MONNY_OK;
}
