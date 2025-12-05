#include "monny/monny.h"

int main()
{
    monny_state_t *state = monny_create_state();

    monny_free_state(state);
    return 0;
}
