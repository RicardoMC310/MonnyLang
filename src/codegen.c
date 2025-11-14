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

    return chunck;
}

void destroyChunck(BytecodeChunck *chunck)
{
    if (chunck->code)
        free(chunck->code);

    for (size_t i = 0; i < chunck->constants_count; i++)
    {
        if (chunck->constants[i].type == VAL_STRING &&
            chunck->constants[i].value.string != NULL)
        {
            free((void *)chunck->constants[i].value.string);
        }
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
        char *cpyStr = strdup(getValueAST(node));
        TaggedValue constant = {VAL_STRING, {.string = cpyStr}};
        int constant_index = addConstants(chunck, constant);
        writeByte(chunck, OP_CONSTANT);
        writeByte(chunck, constant_index);
    }
    else if (getTypeAST(node) == ND_IDENTIFIER)
    {
        printf("Aviso: Identificador ainda não foi implementado.");
    }
}

void generatePrintSTMT(BytecodeChunck *chunck, ASTNode *node)
{
    if (node == NULL)
        return;

    ASTNode *leftPrint = getLeftExpr(node);

    generateExpr(chunck, leftPrint);

    if (getTypeAST(leftPrint) == ND_NUMBER)
    {
        writeByte(chunck, OP_PRINT_NUMBER);
    }
    else if (getTypeAST(leftPrint) == ND_STRING)
    {
        writeByte(chunck, OP_PRINT_STRING);
    }
    else
    {
        writeByte(chunck, OP_PRINT_NUMBER);
    }

    writeByte(chunck, OP_POP);
}

BytecodeChunck *generateCode(ASTNode *node)
{
    BytecodeChunck *chunck = createChunck();
    if (chunck == NULL)
        return NULL;

    if (node == NULL)
        writeByte(chunck, OP_RETURN);

    if (getTypeAST(node) == ND_PRINT)
    {
        generatePrintSTMT(chunck, node);
    }

    writeByte(chunck, OP_RETURN);

    return chunck;
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