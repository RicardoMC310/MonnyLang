#include "object_string.h"
#include "tagged_value.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void string_destroy(object_base_t *self)
{
	object_string_t *object = (object_string_t *)self;
	memset(object->string, 0, strlen(object->string) + 1);
	free(object->string);
	object->string = NULL;
	memset(object, 0, sizeof(object_string_t));
	free(object);
	object = NULL;
}

tagged_value_t string_get_value(object_base_t *self)
{
	object_string_t *object = (object_string_t *)self;
	tagged_value_t t_val = {.type = TV_STRING};
	t_val.as.str = object->string;

	return t_val;
}

void string_display(object_base_t *self)
{
	object_string_t *object = (object_string_t *)self;
	printf("%s\n", object->string);
}

void string_update_value(object_base_t *self, tagged_value_t value)
{
	object_string_t *object = (object_string_t *)self;

	if (value.type != TV_STRING)
	{
		printf("valor passado obrigatóriamente precisa ser uma string\n");
		exit(1);
	}

	free(object->string);
	
	object->string = strdup(AS_STRING(value));
}

static ObjectVTable string_vtable = {
	.destroy = string_destroy,
	.get_value = string_get_value,
	.display = string_display,
	.update_value = string_update_value,
};

void *string_create(char *string)
{
	object_string_t *object = (object_string_t *)malloc(sizeof(object_string_t));
	
	object->base.vtable = &string_vtable;
	object->base.type = TV_STRING;
	object->string = strdup(string);
	
	return object;
}
