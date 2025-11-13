#ifndef BYTECODE_H
#define BYTECODE_H

#include <token.h>

#include <stdbool.h>
#include <stdint.h>

typedef enum 
{
    OP_CONSTANT,
    OP_PRINT_NUMBER,
    OP_PRINT_STRING,
    OP_PRINT_BOOL,
    OP_POP,
    OP_RETURN,
    OP_TRUE,
    OP_FALSE,
    OP_NIL,
} OpCodes;

typedef enum
{
    VAL_NUMBER,
    VAL_STRING,
    VAL_BOOLEAN,
    VAL_OBJ
} ValueType;

typedef struct
{
    Value value;
    ValueType type;
} TaggedValue;

typedef struct BytecodeChunck BytecodeChunck;

#endif // BYTECODE_H