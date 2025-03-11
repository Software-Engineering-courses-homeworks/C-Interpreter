#ifndef CLOX_VM_H
#define CLOX_VM_H

#include "chunk.h"
#include "table.h"
#include "value.h"
#include "object.h"

#define FRAMES_MAX 64
#define STACK_MAX (FRAMES_MAX*UINT8_COUNT)

typedef struct
{
    ObjClosure* closure;
    uint8_t* ip;
    Value* slots;
} CallFrame;

typedef struct
{
    CallFrame frames[FRAMES_MAX];
    int frameCount;
    Value stack[STACK_MAX];
    Value* stackTop;
    Table strings;
    Table globals;
    ObjString* initString;
    ObjUpvalue* openUpvalues;
    size_t bytesAllocated;
    size_t nextGC;
    Obj* objects;
    int grayCount;
    int grayCapacity;
    Obj** grayStack;
} VM;

typedef enum
{
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