#ifndef CLOX_VM_H
#define CLOX_VM_H

#include "chunk.h"
#include "value.h"

#define STACK_MAX 256

typedef struct {
    Chunk* chunk;
    uint8_t* ip;
    Value stack[STACK_MAX];
    Value* stackTop;
} VM;

typedef enum {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
} InterpretResult;

/// initializes the VM
void initVM();

/// frees the VM
void freeVM();

/// interprets a given chunk to the VM and returns the interpreted result
/// @param chunk
/// @return the interpreted result of the given chunk(need to change)
InterpretResult interpret(const char* source);

/// pushes a value onto the VM stack
/// @param val the value that needs to be pushed
void push(Value val);

/// pops the top value from the VM stack
/// @return the top value in the stack
Value pop();
#endif //CLOX_VM_H
