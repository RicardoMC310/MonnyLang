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
} Value;

typedef enum
{
    TY_STRING,
    TY_NIL,
    TY_NUMBER
} ValueType;

typedef struct
{
    Value as;
    ValueType type;
} TaggedValue;

#define IS_NUMBER(value) ((value).type == TY_NUMBER)
#define IS_STRING(value) ((value).type == TY_STRING)
#define IS_NIL(value) ((value).type == TY_NIL)

#define AS_NUMBER(value) ((value).as.number)
#define AS_STRING(value) ((value).as.string)
#define AS_NIL(value) ((value).as.ptr)

#define MAKE_NUMBER(value) ((TaggedValue){.as.number = (value), .type = TY_NUMBER})
#define MAKE_STRING(str) ((TaggedValue){.as.string = (str), .type = TY_STRING})
#define MAKE_NIL() ((TaggedValue){.as.ptr = NULL, .type = TY_NIL})

#endif // VALUES_H