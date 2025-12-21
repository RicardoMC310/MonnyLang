#include "mgc.h"

void mark(object_t *obj)
{
    if (!obj)
        return;
    if (obj->marked)
        return;
    obj->marked = 0b1;
    mark(obj->child);
}

void markAll(state_t *state)
{
    for (uint32_t i = 0; i < state->stackCount; i++)
    {
        mark(state->stack[i]);
    }
}

void sweep(state_t *state)
{
    object_t **obj = &state->heap;
    while (*obj)
    {
        if (!(*obj)->marked)
        {
            object_t *garbage = *obj;
            *obj = garbage->next;
            deleteObject(garbage);
        }
        else
        {
            (*obj)->marked = 0;
            obj = &(*obj)->next;
        }
    }
}

void gc(state_t *state)
{
    markAll(state);
    sweep(state);
}
