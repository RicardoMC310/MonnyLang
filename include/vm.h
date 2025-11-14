#ifndef VM_H
#define VM_H

typedef enum
{
    ITP_OK,
    ITP_COMP_ERROR,
    ITP_RUNT_ERROR,
} InterpretResult;

typedef struct VM VM;

VM *createVM();
void destroyVM(VM *vm);

InterpretResult execute(VM *vm, BytecodeChunck *chunck);

#endif