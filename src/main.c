#include "mobject.h"
#include "mstate.h"
#include "mgc.h"

#include <stdio.h>
#include <string.h>

int main(void)
{
    state_t* state = newState();

    object_t *number = newNumberObject(state, 23);
    object_t *str = newStringObject(state, "Teste de uso");
    object_t *str2 = newStringObject(state, "Teste de um outro texto");

    number->child = str;

    stack_push(state, number);

    gc(state);

    printf("%2.2f\n", AS_NUMBER(number));
    printf("%s\n", AS_STRING(str));
    printf("%s\n", AS_STRING(str2));

    closeState(state);

    return 0;
}
