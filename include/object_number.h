#pragma once

#include "object_base.h"

typedef struct 
{
    object_base_t base;

    double value;
} object_number_t;

void *number_create(double number);