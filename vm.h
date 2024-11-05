#ifndef CLOX_VM_H
#define CLOX_VM_H

#include "chunk.h"
#include "table.h"
#include "value.h"

#define STACK_MAX 256

typedef struct {
    Chunk* chunk;
    uint8_t* ip;
    Value stack[STACK_MAX];
    Value* stackTop;
    Table strings;
    Obj* objects;
} VM;

typedef enum {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
} InterpretResult;

extern VM vm;

/// initializes the VM
void initVM();

/// frees the VM
void freeVM();

/// interprets a given chunk to the VM and returns the interpreted result
/// @return the interpreted result of the given chunk(need to change)
InterpretResult interpret(const char* source);

/// pushes a value onto the VM stack
/// @param val the value that needs to be pushed
void push(Value val);

/// pops the top value from the VM stack
/// @return the top value in the stack
Value pop();
#endif //CLOX_VM_H
