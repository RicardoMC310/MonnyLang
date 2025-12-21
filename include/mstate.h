#pragma once

#include <stdint.h>

#define STACK_MAX_SIZE 512

typedef struct object_t object_t;


typedef struct state_t
{
    object_t *heap;
    object_t *stack[STACK_MAX_SIZE];
    uint32_t stackCount;
} state_t;

state_t *newState();
void closeState(state_t *state);

void stack_push(state_t *state, object_t *obj);
object_t *stack_pop(state_t *state);
