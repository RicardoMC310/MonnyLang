#include "mobject.h"

#include <string.h>

#define DEFINE_NEW_OBJECT(funcName, enumType, fieldName, valueType) \
    object_t *funcName(state_t *state, valueType value)             \
    {                                                               \
        object_t *obj = malloc(sizeof(object_t));                   \
        obj->type = enumType;                                       \
        obj->marked = 0;                                            \
        obj->child = NULL;                                          \
        obj->next = state->heap;                                    \
        state->heap = obj;                                          \
        obj->as.fieldName = value;                                  \
        return obj;                                                 \
    }

DEFINE_NEW_OBJECT(newNumberObject, OBJ_NUMBER, number, double)
DEFINE_NEW_OBJECT(newBooleanObject, OBJ_BOOLEAN, boolean, bool)

object_t *newStringObject(state_t *state, char *str)
{
    object_t *obj = malloc(sizeof(object_t));
    obj->type = OBJ_STRING;
    obj->marked = 0;
    obj->child = NULL;
    obj->next = state->heap;
    state->heap = obj;
    obj->as.str = strdup(str);
    return obj;
}

object_t *newNullObject(state_t *state)
{
    object_t *obj = malloc(sizeof(object_t));
    obj->type = OBJ_NULL;
    obj->marked = 0;
    obj->child = NULL;
    obj->next = state->heap;
    state->heap = obj;
    obj->as.null = NULL;
    return obj;
}

void deleteObject(object_t *obj)
{
    if (obj->type == OBJ_STRING && obj->as.str)
        free(obj->as.str);

    free(obj);
}