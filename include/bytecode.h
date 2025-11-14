#ifndef BYTECODE_H
#define BYTECODE_H

#include <token.h>
#include <parser.h>

#include <stdlib.h>
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
    VAL_OBJ,
    VAL_NIL
} ValueType;

typedef struct
{
    ValueType type;
    Value value;
} TaggedValue;

typedef struct BytecodeChunck BytecodeChunck;

BytecodeChunck *createChunck();
void destroyChunck(BytecodeChunck *chunck);

BytecodeChunck *generateCode(ASTNode *node);
uint8_t *getCodeChunck(BytecodeChunck *chunck);
TaggedValue *getConstantsChunck(BytecodeChunck *chunck);

#endif // BYTECODE_H