#pragma once

#include "tagged_value.h"

typedef struct object_base_t object_base_t;

typedef struct 
{
    void (*destroy)(object_base_t *self);
    tagged_value_t (*get_value)(object_base_t *self);
} ObjectVTable;

struct object_base_t
{
    ObjectVTable *vtable;
};

void object_destroy(void *rawObject);
tagged_value_t object_get_value(void *rawObject);