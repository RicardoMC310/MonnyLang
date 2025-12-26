#include "object_boolean.h"
#include "tagged_value.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void boolean_destroy(object_base_t *self)
{
	object_boolean_t *object = (object_boolean_t *)self;
	free(object);
}

tagged_value_t boolean_get_value(object_base_t *self)
{
	object_boolean_t *object = (object_boolean_t *)self;
	tagged_value_t t_val = {.type = TV_BOOLEAN};
	t_val.as.boolean = object->boolean;

	return t_val;
}

void boolean_display(object_base_t *self)
{
	object_boolean_t *object = (object_boolean_t *)self;

	printf("%s\n", object->boolean ? "true" : "false" );
}

void boolean_update_value(object_base_t *self, tagged_value_t value)
{
	object_boolean_t *object = (object_boolean_t *)self;

	if (value.type != TV_BOOLEAN)
	{
		printf("valor passado obrigatóriamente precisa ser um boolean\n");
		exit(1);
	}

	object->boolean = AS_BOOLEAN(value);
}

static ObjectVTable boolean_vtable = {
	.destroy = boolean_destroy,
	.get_value = boolean_get_value,
	.display = boolean_display,
	.update_value = boolean_update_value
};

void *boolean_create(bool boolean)
{
	object_boolean_t *object = (object_boolean_t *)malloc(sizeof(object_boolean_t));

	object->base.vtable = &boolean_vtable;
	object->base.type = TV_BOOLEAN;
	object->boolean = boolean;

	return object;
}
