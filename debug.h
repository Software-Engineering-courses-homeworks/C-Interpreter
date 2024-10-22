#ifndef clox_debug_h
#define clox_debug_h

#include "chunk.h"

/// @brief 
/// @param chunk 
/// @param name 
void disassembleChunk(Chunk* chunk, const char* name);

/// @brief 
/// @param chunk 
/// @param offset
/// @return 
int disassembleInstruction(Chunk* chunk, int offset);

/// the function decodes the line from the RLE compression
/// @param chunk the bytecode chunk
/// @param offset the index of the offending instruction
/// @return returns the line of the offending instruction
int getLine(Chunk* chunk, int offset);

static int simpleInstruction(const char* name, int offset);

static int constantInstruction(const char* name, Chunk* chunk, int offset);

static int constantLongInstruction(const char* name, Chunk* chunk, int offset);

#endif