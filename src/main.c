#include "object_base.h"
#include "object_number.h"
#include "object_string.h"
#include "object_boolean.h"
#include "tagged_value.h"

int main(void)
{
    object_base_t *number = number_create(3.14159);
    object_base_t *string = string_create("Ricardo Matos Costa");
	object_base_t *boolean1 = boolean_create(true);

    object_display(number);
    object_display(string);
    
    object_display(boolean1);
    if (!object_is_type(boolean1, TV_BOOLEAN))
    {
    	return -1;
    }

    object_update_value(boolean1, MAKE_BOOLEAN(false));
    object_display(boolean1);

    if (!object_is_type(number, TV_NUMBER))
    {
    	return -1;
    }

    object_update_value(number, MAKE_NUMBER(12));
    object_display(number);

    if (!object_is_type(string, TV_STRING))
    {
    	return -1;
    }

    object_update_value(string, MAKE_STRING("Paulão Do Brás"));
    object_display(string);

    object_destroy(number);
    object_destroy(string);
    object_destroy(boolean1);

    return 0;
}
