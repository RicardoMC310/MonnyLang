#pragma once

#include "object_base.h"

#include <stdbool.h>

typedef struct object_boolean_t
{
	object_base_t base;

	bool boolean;
} object_boolean_t;

void *boolean_create(bool boolean);
