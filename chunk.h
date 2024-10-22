#ifndef clox_chunk_h
#define clox_chunk_h

#include "common.h"
#include "value.h"

//The instruction will mean “return from the current function”.
typedef enum {
  OP_RETURN,
  OP_CONSTANT,
  OP_CONSTANT_LONG,
  OP_ADD,
  OP_SUBTRACT,
  OP_MULTIPLY,
  OP_DIVIDE,
  OP_NEGATE,
} OpCode;

//wrapper around an array of bytes
typedef struct {
  int count;
  int capacity;
  int lineCount;
  int lineCapacity;

  uint8_t* code;
  int* lines;
  ValueArray constants;
} Chunk;

/// @brief 
/// @param chunk 
void initChunk(Chunk* chunk);

/// @brief 
/// @param chunk 
/// @param byte 
/// @param line
void writeChunk(Chunk* chunk, uint8_t byte, int line);

/// @brief 
/// @param chunk 
void freeChunk(Chunk* chunk);

/// adds a constant to the constant pool via discrete function for orderly operation
/// @param chunk
/// @param value
/// @returns the index of the appended constant in the pool
int addConstant(Chunk* chunk, Value value);

/// 
/// @param chunk 
/// @param value 
/// @param line 
void writeConstant(Chunk* chunk, Value value, int line);
#endif