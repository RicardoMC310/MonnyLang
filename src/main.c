#include "object_base.h"
#include "object_number.h"
#include "tagged_value.h"

#include <stdio.h>

int main(void)
{
    object_number_t *number = number_create(12);

    tagged_value_t value_number = object_get_value(number);

    if (IS_NUMBER(value_number))
    {
        printf("%2.2f\n", AS_NUMBER(value_number));
    }
    else
    {
        printf("(null)\n");
    }

    object_destroy(number);

    return 0;
}
