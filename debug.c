#include "debug.h"
#include "value.h"
#include <stdio.h>

/// @brief disassembles the chunk of instructions one instruction at a time
/// @param chunk - the instruction chunk
/// @param name - the name of the chunk
void disassembleChunk(Chunk* chunk, const char* name)
{
    printf("== %s ==\n", name);

    for(int offset = 0; offset < chunk->count;)
    {
        offset = disassembleInstruction(chunk, offset);
    }
}

/// @brief disassembles the instruction from the bytecode chunk
/// @param chunk an uint8_t array that contains the bytecode for the instructions
/// @param offset the current instruction index
/// @return the index for the next instruction
int disassembleInstruction(Chunk* chunk, int offset)
{
    //prints the instruction number
    printf("%04d ", offset);

    //prints the line
    if(offset > 0 && getLine(chunk, offset) == getLine(chunk, offset - 1))
        printf("    | ");
    else
        printf("%4d ", getLine(chunk, offset));

    //saves the bytecode for the current instruction and switches it to the appropriate debug function
    uint8_t instruction = chunk->code[offset];
    switch(instruction) {
        case OP_CONSTANT:
            return constantInstruction("OP_CONSTANT", chunk, offset);
        case OP_RETURN:
            return simpleInstruction("OP_RETURN", offset);
        case OP_CONSTANT_LONG:
            return constantLongInstruction("OP_CONSTANT_LONG", chunk, offset);
        case OP_NEGATE:
            return simpleInstruction("OP_NEGATE", offset);
        case OP_ADD:
            return simpleInstruction("OP_ADD", offset);
        case OP_SUBTRACT:
            return simpleInstruction("OP_SUBTRACT", offset);
        case OP_MULTIPLY:
            return simpleInstruction("OP_MULTIPLY", offset);
        case OP_DIVIDE:
            return simpleInstruction("OP_DIVIDE", offset);
        default:
            printf("Unknown opcode %d\n", instruction);
            return offset + 1;
    }
}

/// the function decodes the line from the RLE compression
/// @param chunk the bytecode chunk
/// @param offset the index of the offending instruction
/// @return returns the line of the offending instruction
int getLine(Chunk* chunk, int offset)
{
    int index = 0;
    int tempLine = chunk->lines[index];

    //iterates through the lines and decreases the saved line value until we pass all the instructions in a line
    //then passes on to the next line and repeats the process up to the offending instruction
    for(int i = 0; i < offset; i++)
    {
        //checks if arrived at the last instruction (i.e. the first instruction added to the line)
        //and if so, moves on to the next line
        if(tempLine % 100 == 0)
        {
            index++;
            tempLine = chunk->lines[index];
        }
        //else, decrements the encoded instruction count in the line value
        else
            tempLine--;
    }
    return chunk->lines[index] / 100;
}

/// prints the debug for a simple 1 byte instruction
/// @param name name of the opcode
/// @param offset the instruction index
/// @return the new instruction index
static int simpleInstruction(const char* name, int offset)
{
    printf("%s\n",name);
    return offset + 1;
}

/// prints the debug for a 1 byte constant instruction
/// @param name name of the opcode
/// @param chunk a pointer to the bytecode chunk
/// @param offset the instruction index
/// @return the new instruction index
static int constantInstruction(const char* name, Chunk* chunk, int offset)
{
    uint8_t constant = chunk->code[offset + 1];
    printf("%-16s %4d '", name, constant);
    printValue(chunk->constants.values[constant]);
    printf("'\n");
    return offset + 2;
}

/// prints the debug for a 3 byte constant instruction
/// @param name name of the operation
/// @param chunk the bytecode chunk
/// @param offset the index of the bytecode chunk array
/// @return the new offset after teh operation execution
static int constantLongInstruction(const char* name, Chunk* chunk, int offset)
{
    uint32_t constant = (uint32_t)chunk->code[offset + 3] << 16 | (uint16_t)chunk->code[offset + 2] << 8 | chunk->code[offset + 1];
    printf("%-16s %4d '", name, constant);
    printValue(chunk->constants.values[constant]);
    printf("'\n");
    return offset + 4;
}