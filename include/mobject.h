#pragma once

#include "mstate.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum
{
    OBJ_STRING,
    OBJ_NUMBER,
    OBJ_BOOLEAN,
    OBJ_NULL
} OBJECT_TYPE;

typedef struct object_t
{
    OBJECT_TYPE type;
    union
    {
        char *str;
        double number;
        bool boolean;
        void *null;
    } as;
    object_t *child;
    object_t *next;
    unsigned char marked;

} object_t;

#define IS_STRING(obj) (obj->type == OBJ_STRING)
#define IS_NUMBER(obj) (obj->type == OBJ_NUMBER)
#define IS_BOOLEAN(obj) (obj->type == OBJ_BOOLEAN)
#define IS_NULL(obj) (obj->type == OBJ_NULL)

#define AS_STRING(obj) (obj->as.str)
#define AS_NUMBER(obj) (obj->as.number)
#define AS_BOOLEAN(obj) (obj->as.boolean)
#define AS_NULL(obj) (obj->as.null)

object_t *newStringObject(state_t *state, char *str);
object_t *newNumberObject(state_t *state, double num);
object_t *newBooleanObject(state_t *state, bool b);
object_t *newNullObject(state_t *state);

void deleteObject(object_t *obj);