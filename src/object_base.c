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

void object_display(void *rawObject)
{
	object_base_t *object = (object_base_t *)rawObject;
	object->vtable->display(object);
}

void object_update_value(void *rawObject, tagged_value_t value)
{
	object_base_t *object = (object_base_t *)rawObject;
	object->vtable->update_value(object, value);
}

bool object_is_type(void *rawObject, int type)
{
	object_base_t *object = (object_base_t *)rawObject;
	return object->type == type;
}
