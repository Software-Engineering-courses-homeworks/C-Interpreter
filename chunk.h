#ifndef clox_chunk_h
#define clox_chunk_h

#include "common.h"
#include "value.h"

//The instruction will mean “return from the current function”.
typedef enum
{
    OP_CALL,
    OP_RETURN,
    OP_CONSTANT,
    OP_CONSTANT_LONG,
    OP_NIL,
    OP_TRUE,
    OP_FALSE,
    OP_POP,
    OP_GET_LOCAL,
    OP_GET_GLOBAL,
    OP_DEFINE_GLOBAL,
    OP_SET_LOCAL,
    OP_SET_GLOBAL,
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_NEGATE,
    OP_NOT,
    OP_EQUAL,
    OP_GREATER,
    OP_LESS,
    OP_PRINT,
    OP_JUMP,
    OP_JUMP_IF_FALSE,
    OP_LOOP,
    OP_CLOSURE,
    OP_GET_UPVALUE,
    OP_SET_UPVALUE,
    OP_CLOSE_UPVALUE,
} OpCode;

//wrapper around an array of bytes
typedef struct
{
    int count;
    int capacity;
    int lineCount;
    int lineCapacity;

    uint8_t *code;
    int *lines;
    ValueArray constants;
} Chunk;

/// @brief 
/// @param chunk 
void initChunk(Chunk *chunk);

/// @brief 
/// @param chunk 
/// @param byte 
/// @param line
void writeChunk(Chunk *chunk, uint8_t byte, int line);

/// @brief 
/// @param chunk 
void freeChunk(Chunk *chunk);

/// adds a constant to the constant pool via discrete function for orderly operation
/// @param chunk
/// @param value
/// @returns the index of the appended constant in the pool
int addConstant(Chunk *chunk, Value value);

#endif