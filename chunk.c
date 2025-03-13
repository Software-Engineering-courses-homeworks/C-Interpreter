#include <stdlib.h>
#include "chunk.h"
#include "memory.h"
#include "vm.h"

/// initializes a chunk
/// @param chunk    a pointer to a chunk struct
void initChunk(Chunk* chunk)
{
    //initializes the struct values to be those of an empty, uninitialized array
    chunk->count = 0;
    chunk->capacity = 0;
    chunk->code = NULL;
    chunk->lineCount = 0;
    chunk->lineCapacity = 0;
    chunk->lines = NULL;
    initValueArray(&chunk->constants);
}

/// @brief writes a value to a chunk
/// @param chunk    a pointer to a chunk struct
/// @param byte     a byte that gets appended to the end of the chunk
/// @param line     represents the instruction's line
void writeChunk(Chunk* chunk, uint8_t byte, int line)
{
    //checks if the current array already has capacity for the new byte
    //in case there isn't enough capacity, we grow the array to make room
    if (chunk->capacity < chunk->count + 1)
    {
        int oldCapacity = chunk->capacity;
        chunk->capacity = GROW_CAPACITY(oldCapacity);
        chunk->code = GROW_ARRAY(uint8_t, chunk->code, oldCapacity, chunk->capacity);
        chunk->lines = GROW_ARRAY(int, chunk->lines, oldCapacity, chunk->capacity);
    }

    chunk->code[chunk->count] = byte;
    chunk->lines[chunk->count] = line;
    chunk->count++;
}

/// @brief frees the previous array and reinitialize the chunk
/// @param chunk    a pointer to the chunk that needs to be freed
void freeChunk(Chunk* chunk)
{
    FREE_ARRAY(uint8_t, chunk->code, chunk->count);
    FREE_ARRAY(int, chunk->lines, chunk->capacity);
    freeValueArray(&chunk->constants);
    initChunk(chunk);
}

/// adds a constant to the constant pool via discrete function for orderly operation
/// @param chunk    a pointer to the chunk
/// @param value    the value that needs to be appended
/// @returns        the index of the appended constant in the pool
int addConstant(Chunk* chunk, Value value)
{
    push(value);
    writeValueArray(&chunk->constants, value);
    pop();
    return chunk->constants.count - 1;
}