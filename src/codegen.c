#include <bytecode.h>

#include <stdio.h>

struct BytecodeChunck
{
    uint8_t *code;
    size_t capacity;
    size_t count;

    TaggedValue *constants;
    size_t constants_capacity;
    size_t constants_count;

    Variable *variables;
    size_t varCapacity;
    size_t varCount;
};

BytecodeChunck *createChunck()
{
    BytecodeChunck *chunck = malloc(sizeof(BytecodeChunck));
    if (chunck == NULL)
        return NULL;

    chunck->code = NULL;
    chunck->capacity = 0;
    chunck->count = 0;
    chunck->constants = NULL;
    chunck->constants_capacity = 0;
    chunck->constants_count = 0;
    chunck->variables = NULL;
    chunck->varCapacity = 0;
    chunck->varCount = 0;

    return chunck;
}

void destroyChunck(BytecodeChunck *chunck)
{
    if (chunck->code)
        free(chunck->code);

    if (chunck->variables)
    {
        for (size_t i = 0; i < chunck->varCount; i++)
        {
            if (chunck->variables[i].name != NULL)
            {
                free((void*)chunck->variables[i].name);
            }
        }

        free(chunck->variables);
    }

    if (chunck->constants)
        free(chunck->constants);

    free(chunck);
    chunck = NULL;
}

void writeByte(BytecodeChunck *chunck, uint8_t byte)
{
    if (chunck->capacity < chunck->count + 1)
    {
        chunck->capacity = chunck->capacity == 0 ? 8 : chunck->capacity * 2;
        chunck->code = realloc(chunck->code, sizeof(uint8_t) * chunck->capacity);
    }

    chunck->code[chunck->count++] = byte;
}

int addConstants(BytecodeChunck *chunck, TaggedValue value)
{
    if (chunck->constants_capacity < chunck->constants_count + 1)
    {
        chunck->constants_capacity = chunck->constants_capacity == 0 ? 8 : chunck->constants_capacity * 2;
        chunck->constants = realloc(chunck->constants, sizeof(TaggedValue) * chunck->constants_capacity);
    }

    chunck->constants[chunck->constants_count] = value;
    return chunck->constants_count++;
}

int addVar(BytecodeChunck *chunck, Variable var)
{
    if (chunck->varCount >= chunck->varCapacity)
    {
        chunck->varCapacity = chunck->varCapacity == 0 ? 8 : chunck->varCapacity * 2;
        chunck->variables = realloc(chunck->variables, sizeof(Variable) * chunck->varCapacity);
    }

    chunck->variables[chunck->varCount] = var;
    return chunck->varCount++;
}

void generateExpr(BytecodeChunck *chunck, ASTNode *node)
{
    if (node == NULL)
        return;

    if (getTypeAST(node) == ND_NUMBER)
    {
        double number = atof(getValueAST(node));
        TaggedValue constant = {VAL_NUMBER, {.number = number}};
        int constant_index = addConstants(chunck, constant);
        writeByte(chunck, OP_CONSTANT);
        writeByte(chunck, constant_index);
    }
    else if (getTypeAST(node) == ND_STRING)
    {
        TaggedValue constant = {VAL_STRING, {.string = getValueAST(node)}};
        int constant_index = addConstants(chunck, constant);
        writeByte(chunck, OP_CONSTANT);
        writeByte(chunck, constant_index);
    }
    else if (getTypeAST(node) == ND_IDENTIFIER)
    {
        writeByte(chunck, OP_GET_VAR);

        TaggedValue varName = {VAL_STRING, {.string = getValueAST(node)}};
        int constantIndex = addConstants(chunck, varName);
        writeByte(chunck, constantIndex);
    }
}

void generatePrintSTMT(BytecodeChunck *chunck, ASTNode *node)
{
    if (node == NULL)
        return;

    ASTNode *leftPrint = getLeftExpr(node);

    generateExpr(chunck, leftPrint);

    writeByte(chunck, OP_PRINT);

    writeByte(chunck, OP_POP);
}

void generateSetVarSTMT(BytecodeChunck *chunck, ASTNode *node)
{
    if (node == NULL)
        return;

    generateExpr(chunck, getVarValue(node));
    writeByte(chunck, OP_SET_VAR);

    // USA a string diretamente, SEM strdup
    TaggedValue varNameConstant = {VAL_STRING, {.string = getVarName(node)}};
    int constantIndex = addConstants(chunck, varNameConstant);
    writeByte(chunck, constantIndex);
}

void generateCode(BytecodeChunck *chunck, ASTNode *node)
{
    if (chunck == NULL)
        return;

    if (node == NULL)
        writeByte(chunck, OP_RETURN);

    if (getTypeAST(node) == ND_PRINT)
    {
        generatePrintSTMT(chunck, node);
    }
    else if (getTypeAST(node) == ND_SET_VAR)
    {
        generateSetVarSTMT(chunck, node);
    }
    else
    {
        printf("DEBUG: generateCode - Tipo de node desconhecido: %d\n", getTypeAST(node));
    }
}

uint8_t *getCodeChunck(BytecodeChunck *chunck)
{
    if (chunck == NULL)
        return NULL;

    return chunck->code;
}

TaggedValue *getConstantsChunck(BytecodeChunck *chunck)
{
    if (chunck == NULL)
        return NULL;

    return chunck->constants;
}

Variable *getVariablesChunck(BytecodeChunck *chunck)
{
    if (chunck == NULL)
        return NULL;

    return chunck->variables;
}

size_t getVariablesCount(BytecodeChunck *chunck)
{
    if (chunck == NULL)
        return 0;

    return chunck->varCount;
}