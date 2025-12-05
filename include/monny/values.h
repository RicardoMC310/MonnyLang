#ifndef VALUES_H
#define VALUES_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

typedef union
{
    double number;
    char *string;
    void *ptr;
} monny_value_t;

typedef enum
{
    MONNY_TY_STRING,
    MONNY_TY_NIL,
    MONNY_TY_NUMBER
} monny_valuetype_t;

typedef struct
{
    monny_value_t as;
    monny_valuetype_t type;
} monny_taggedvalue_t;

#define MONNY_IS_NUMBER(value) ((value).type == MONNY_TY_NUMBER)
#define MONNY_IS_STRING(value) ((value).type == MONNY_TY_STRING)
#define MONNY_IS_NIL(value) ((value).type == MONNY_TY_NIL)

#define MONNY_AS_NUMBER(value) ((value).as.number)
#define MONNY_AS_STRING(value) ((value).as.string)
#define MONNY_AS_NIL(value) ((value).as.ptr)

#define MONNY_MAKE_NUMBER(value) ((monny_taggedvalue_t){.as.number = (value), .type = MONNY_TY_NUMBER})
#define MONNY_MAKE_STRING(str) ((monny_taggedvalue_t){.as.string = (str), .type = MONNY_TY_STRING})
#define MONNY_MAKE_NIL() ((monny_taggedvalue_t){.as.ptr = NULL, .type = MONNY_TY_NIL})

#endif // VALUES_H