#include "object_number.h"
#include "tagged_value.h"

#include <stdio.h>
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

void number_display(object_base_t *self)
{
	object_number_t *object = (object_number_t *)self;
	printf("%g\n", object->value);
}

void number_update_value(object_base_t *self, tagged_value_t value)
{
	object_number_t *object = (object_number_t *)self;

	if (value.type != TV_NUMBER)
	{
		printf("valor passado obrigatóriamente precisa ser um number\n");
		exit(1);
	}

	object->value = AS_NUMBER(value);
}

static ObjectVTable number_vtable = {
    .destroy = number_destroy,
    .get_value = number_get_value,
    .display = number_display,
	.update_value = number_update_value,
};

void *number_create(double val)
{
    object_number_t *number = malloc(sizeof(object_number_t));

    number->base.vtable = &number_vtable;
    number->base.type = TV_NUMBER;
    number->value = val;

    return number;
}
