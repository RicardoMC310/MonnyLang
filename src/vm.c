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
        free((void*)vm->stack);
        vm->stack = NULL;
        vm->stackCapacity = 0;
        vm->stackSize = 0;
    }
    
    free(vm);
    vm = NULL;
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
