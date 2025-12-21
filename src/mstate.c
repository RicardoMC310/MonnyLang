#include "mstate.h"
#include "mobject.h"
#include "mgc.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

state_t *newState()
{
    state_t *state = malloc(sizeof(state_t));
    if (!state)
    {
        perror("state");
        return NULL;
    }

    state->heap = NULL;
    state->stackCount = 0;

    return state;
}

void closeState(state_t *state)
{
    if (!state)
        return;

    state->stackCount = 0;
    sweep(state);

    free(state);
}

void stack_push(state_t *state, object_t *obj)
{
    state->stack[state->stackCount++] = obj;
}

object_t *stack_pop(state_t *state)
{
    return state->stack[--(state->stackCount)];
}