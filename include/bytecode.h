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
    OP_SET_VAR,
    OP_GET_VAR,
    OP_PRINT,
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
    VAL_NIL,
    VAL_IDENTIFIER
} ValueType;

typedef struct
{
    ValueType type;
    Value value;
} TaggedValue;

typedef struct
{
    const char *name;
    TaggedValue value;
} Variable;

typedef struct BytecodeChunck BytecodeChunck;

BytecodeChunck *createChunck();
void destroyChunck(BytecodeChunck *chunck);

void generateCode(BytecodeChunck *chunck, ASTNode *node);
void writeByte(BytecodeChunck *chunck, uint8_t byte);
uint8_t *getCodeChunck(BytecodeChunck *chunck);
TaggedValue *getConstantsChunck(BytecodeChunck *chunck);
Variable *getVariablesChunck(BytecodeChunck *chunck);
size_t getVariablesCount(BytecodeChunck *chunck);
int addVar(BytecodeChunck *chunck, Variable var);

#endif // BYTECODE_H