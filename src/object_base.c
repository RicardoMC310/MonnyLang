#include "object_base.h"

void object_destroy(void *rawObject)
{
    object_base_t *object = (object_base_t *)rawObject;
    object->vtable->destroy(object);
}

tagged_value_t object_get_value(void *rawObject)
{
    object_base_t *object = (object_base_t *)rawObject;
    return object->vtable->get_value(object);
}