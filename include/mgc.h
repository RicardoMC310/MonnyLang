#pragma once

#include "mobject.h"
#include "mstate.h"

void sweep(state_t *state);
void mark(object_t *obj);
void markAll(state_t *state);

void gc(state_t *state);