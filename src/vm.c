#include <bytecode.h>
#include <vm.h>

#include <stdio.h>

typedef struct
{
    char *name;
    TaggedValue value;
} Variable;

struct VM
{
    BytecodeChunck *chunck;
    uint8_t *ip;

    TaggedValue *stack;
    size_t stackSize;
    size_t stackCapacity;

    Variable *variables;
    size_t varCount;
    size_t varCapacity;
};

VM *createVM()
{
    VM *vm = malloc(sizeof(VM));
    if (vm == NULL)
        return NULL;

    vm->chunck = NULL;
    vm->ip = NULL;
    vm->stack = NULL;
    vm->variables = NULL;
    vm->stackSize = 0;
    vm->stackCapacity = 0;
    vm->varCapacity = 0;
    vm->varCount = 0;

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

    if (vm->variables != NULL)
    {
        for (size_t i = 0; i < vm->varCount; i++)
        {
            // LIBERA o nome da variável (alocado com strdup)
            if (vm->variables[i].name != NULL)
            {
                free(vm->variables[i].name);
                vm->variables[i].name = NULL;
            }

            // Se o valor for uma string, também precisa liberar
            if (vm->variables[i].value.type == VAL_STRING &&
                vm->variables[i].value.value.string != NULL)
            {
                free((void *)vm->variables[i].value.value.string);
                vm->variables[i].value.value.string = NULL;
            }
        }
        free((void *)vm->variables);
        vm->variables = NULL;
        vm->varCapacity = 0;
        vm->varCount = 0;
    }

    free(vm);
    vm = NULL;
}

void setVariable(VM *vm, const char *name, TaggedValue value)
{
    for (size_t i = 0; i < vm->varCount; i++)
    {
        if (strcmp(vm->variables[i].name, name) == 0)
        {
            vm->variables[i].value = value;
            return;
        }
    }

    if (vm->varCount >= vm->varCapacity)
    {
        vm->varCapacity = vm->varCapacity == 0 ? 8 : vm->varCapacity * 2;
        vm->variables = realloc(vm->variables, sizeof(Variable) * vm->varCapacity);
    }

    vm->variables[vm->varCount].name = strdup(name);
    vm->variables[vm->varCount].value = value;
    vm->varCount++;
}

TaggedValue getVariable(VM *vm, char *name)
{
    for (size_t i = 0; i < vm->varCount; i++)
    {
        if (strcmp(vm->variables[i].name, name) == 0)
        {
            return vm->variables[i].value;
        }
    }

    // Variável não encontrada
    printf("Error: Undefined variable '%s'\n", name);
    return (TaggedValue){VAL_NUMBER, {.number = 0}};
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

        case OP_PRINT_NUMBER:
        {
            TaggedValue value = pop(vm);
            if (value.type == VAL_NUMBER)
            {
                printf("%g\n", value.value.number);
            }
            else
            {
                printf("Erro: tipo passado não foi um numero.");
                return ITP_RUNT_ERROR;
            }
            break;
        }

        case OP_PRINT_STRING:
        {
            TaggedValue value = pop(vm);
            if (value.type == VAL_STRING)
            {
                printf("%s\n", value.value.string);
            }
            else
            {
                printf("Erro: tipo passado não foi um texto.");
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
