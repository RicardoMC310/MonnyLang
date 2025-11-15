#include <bytecode.h>
#include <vm.h>

#include <stdio.h>

struct VM
{
    BytecodeChunck *chunck;
    uint8_t *ip;

    TaggedValue *stack;
    size_t stackSize;
    size_t stackCapacity;
};

VM *createVM()
{
    VM *vm = malloc(sizeof(VM));
    if (vm == NULL)
        return NULL;

    vm->chunck = NULL;
    vm->ip = NULL;
    vm->stack = NULL;
    vm->stackSize = 0;
    vm->stackCapacity = 0;

    return vm;
}

void destroyVM(VM *vm)
{

    if (vm->stack != NULL)
    {
        free((void *)vm->stack);
        vm->stack = NULL;
        vm->stackCapacity = 0;
        vm->stackSize = 0;
    }

    free(vm);
    vm = NULL;
}

void setVariable(VM *vm, const char *name, TaggedValue value)
{
    if (vm == NULL || name == NULL || vm->chunck == NULL)
    {
        return;
    }

    Variable *variables = getVariablesChunck(vm->chunck);
    size_t varCount = getVariablesCount(vm->chunck);

    for (size_t i = 0; i < varCount; i++)
    {
        if (variables[i].name != NULL)
        {
            if (strcmp(variables[i].name, name) == 0)
            {
                variables[i].value = value;
                return;
            }
        }
    }

    Variable newVar;
    newVar.name = strdup(name);
    newVar.value = value;
    addVar(vm->chunck, newVar);
}

TaggedValue getVariable(VM *vm, const char *name)
{
    if (vm == NULL || name == NULL || vm->chunck == NULL)
    {
        return (TaggedValue){VAL_NIL, {.number = 0}};
    }

    Variable *variables = getVariablesChunck(vm->chunck);
    size_t varCount = getVariablesCount(vm->chunck);

    for (size_t i = 0; i < varCount; i++)
    {
        if (variables[i].name != NULL && strcmp(variables[i].name, name) == 0)
        {
            return variables[i].value;
        }
    }

    return (TaggedValue){VAL_NIL, {.number = 0}};
}

void push(VM *vm, TaggedValue value)
{
    if (vm->stackSize >= vm->stackCapacity)
    {
        vm->stackCapacity = vm->stackCapacity == 0 ? 8 : vm->stackCapacity * 2;
        vm->stack = realloc(vm->stack, sizeof(TaggedValue) * vm->stackCapacity);
    }

    vm->stack[vm->stackSize++] = value;
}

TaggedValue pop(VM *vm)
{
    if (vm->stackSize == 0)
    {
        return (TaggedValue){VAL_NIL, {.ptr = NULL}};
    }
    return vm->stack[--vm->stackSize];
}

InterpretResult execute(VM *vm, BytecodeChunck *chunck)
{
    vm->chunck = chunck;
    vm->ip = getCodeChunck(chunck);

    while (1)
    {
        uint8_t instruction = *vm->ip++;

        switch (instruction)
        {
        case OP_CONSTANT:
        {
            uint8_t constant_index = *vm->ip++;
            TaggedValue constant = (getConstantsChunck(chunck))[constant_index];
            push(vm, constant);
            break;
        }

        case OP_PRINT:
        {
            TaggedValue value = pop(vm);
            switch (value.type)
            {
            case VAL_NUMBER:
                printf("%g\n", value.value.number);
                break;
            case VAL_STRING:
                printf("%s\n", value.value.string);
                break;
            case VAL_BOOLEAN:
                printf("%s\n", value.value.boolean ? "true" : "false");
                break;
            case VAL_NIL:
                printf("nil\n");
                break;
            case VAL_IDENTIFIER:
            {
                printf("Print com identifier\n");
                break;
            }
            default:
                printf("Erro: tipo desconhecido.\n");
                return ITP_RUNT_ERROR;
            }

            break;
        }

        case OP_SET_VAR:
        {
            uint8_t constantIndex = *vm->ip++;
            TaggedValue TVvarName = getConstantsChunck(chunck)[constantIndex];

            if (TVvarName.type != VAL_STRING)
            {
                continue;
            }

            TaggedValue value = vm->stack[--vm->stackSize];

            setVariable(vm, TVvarName.value.string, value);

            break;
        }

        case OP_GET_VAR:
        {
            uint8_t constantIndex = *vm->ip++;
            TaggedValue TVvarName = getConstantsChunck(chunck)[constantIndex];

            if (TVvarName.type != VAL_STRING)
            {
                continue;
            }

            TaggedValue value = getVariable(vm, TVvarName.value.string);

            push(vm, value);

            break;
        }

        case OP_TRUE:
            push(vm, (TaggedValue){VAL_BOOLEAN, {.boolean = true}});
            break;
        case OP_FALSE:
            push(vm, (TaggedValue){VAL_BOOLEAN, {.boolean = false}});
            break;
        case OP_NIL:
            push(vm, (TaggedValue){VAL_NIL, {.ptr = NULL}});
            break;
        case OP_POP:
            pop(vm);
            break;
        case OP_RETURN:
            return ITP_OK;
        default:
            printf("Unknown opcode: %d\n", instruction);
            return ITP_RUNT_ERROR;
        }
    }
}
