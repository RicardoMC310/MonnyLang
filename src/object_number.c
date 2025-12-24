#include "object_number.h"
#include "tagged_value.h"

#include <stdlib.h>
#include <string.h>

void number_destroy(object_base_t *self)
{
    object_number_t *object = (object_number_t *)self;
    memset(object, 0, sizeof(object_number_t));
    free(object);
    object = NULL;
}

tagged_value_t number_get_value(object_base_t *self)
{
    object_number_t *object = (object_number_t *)self;
    tagged_value_t t_val = {.type = TV_NUMBER};
    t_val.as.number = object->value;

    return t_val;
}

static ObjectVTable number_vtable = {
    .destroy = number_destroy,
    .get_value = number_get_value};

void *number_create(double val)
{
    object_number_t *number = malloc(sizeof(object_number_t));

    number->base.vtable = &number_vtable;
    number->value = val;

    return number;
}