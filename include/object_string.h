#pragma once

#include "object_base.h"

typedef struct object_string_t
{
	object_base_t base;

	char *string;
} object_string_t;

void *string_create(char *string);
