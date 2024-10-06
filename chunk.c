#include <stdlib.h>
#include "chunk.h"
#include "memory.h"

/// @brief initializes a chunk
/// @param chunk - a pointer to a chunk struct
void initChunk(Chunk* chunk)
{
    //initializes the struct values to be thowse of an empty, uninitialized array
    chunk->count = 0;
    chunk->capacity = 0;
    chunk->code = NULL;
}

/// @brief writes a value to a chunk
/// @param chunk - a pointer to a chunk struct
/// @param byte  - a byte that get appended to the end of the chunk
void writeChunk(Chunk* chunk, uint8_t byte)
{
    //checks if the current array already has capacity for the new byte
    //incase there isn't enough capacity, we grow the array to make room
    if(chunk->capacity <chunk->count+1)
    {
        int oldCapacity = chunk->capacity;
        chunk->capacity = GROW_CAPACITY(oldCapacity);
        chunk->code = GROW_ARRAY(unit8_t, chunk->code, oldCapacity, chunk->capacity);
    }

    chunk->code[chunk->count] = byte;
    chunk->count++;
}