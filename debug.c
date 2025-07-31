#include "debug.h"
#include "value.h"
#include <stdio.h>
#include "object.h"

/// @brief       disassembles the chunk of instructions one instruction at a time
/// @param chunk the instruction chunk
/// @param name  the name of the chunk
void disassembleChunk(Chunk* chunk, const char* name)
{
    printf("== %s ==\n", name);

    for (int offset = 0; offset < chunk->count;)
    {
        offset = disassembleInstruction(chunk, offset);
    }
}

/// the function decodes the line from the RLE compression
/// @param chunk  the bytecode chunk
/// @param offset the index of the offending instruction
/// @return       returns the line of the offending instruction
int getLine(Chunk* chunk, int offset)
{
    int index = 0;
    int tempLine = chunk->lines[index];

    //iterates through the lines and decreases the saved line value until we pass all the instructions in a line
    //then passes on to the next line and repeats the process up to the offending instruction
    for (int i = 0; i < offset; i++)
    {
        //checks if arrived at the last instruction (i.e. the first instruction added to the line)
        //and if so, moves on to the next line
        if (tempLine % 100 == 0)
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
/// @param name   name of the opcode
/// @param offset the instruction index
/// @return       the new instruction index
static int simpleInstruction(const char* name, int offset)
{
    printf("%s\n", name);
    return offset + 1;
}

/// gets a byte instruction and prints a debug log
/// @param name   the instruction name
/// @param chunk  the disassembled chunk
/// @param offset the bytecode array index
/// @return       the new offset for the bytecode
static int byteInstruction(const char* name, Chunk* chunk, int offset)
{
    uint8_t slot = chunk->code[offset + 1];
    printf("%-16s %4d\n", name, slot);
    return offset + 2;
}

/// the function gets a jump instruction and prints a debug log
/// @param name   the instruction name
/// @param sign   if the jump was executed or not
/// @param chunk  the chunk being disassembled
/// @param offset the current bytecode index
/// @return       the new offset after the jump
static int jumpInstruction(const char* name, int sign, Chunk* chunk, int offset)
{
    uint16_t jump = (uint16_t)(chunk->code[offset + 1] << 8);
    jump |= chunk->code[offset + 2];
    printf("%-16s %4d -> %d", name, offset, offset + 3 + sign * jump);
    return offset + 3;
}

/// prints the debug for a 1 byte constant instruction
/// @param name   name of the opcode
/// @param chunk  a pointer to the bytecode chunk
/// @param offset the instruction index
/// @return       the new instruction index
static int constantInstruction(const char* name, Chunk* chunk, int offset)
{
    uint8_t constant = chunk->code[offset + 1];
    printf("%-16s %4d '", name, constant);
    printValue(chunk->constants.values[constant]);
    printf("'\n");
    return offset + 2;
}

/// prints the debug for a 3 byte constant instruction
/// @param name   name of the operation
/// @param chunk  the bytecode chunk
/// @param offset the index of the bytecode chunk array
/// @return       the new offset after teh operation execution
static int constantLongInstruction(const char* name, Chunk* chunk, int offset)
{
    uint32_t constant = (uint32_t)chunk->code[offset + 3] << 16 | (uint16_t)chunk->code[offset + 2] << 8 | chunk->code[
        offset + 1];
    printf("%-16s %4d '", name, constant);
    printValue(chunk->constants.values[constant]);
    printf("'\n");
    return offset + 4;
}

///
/// @param name
/// @param chunk
/// @param offset
/// @return
static int invokeInstruction(const char* name, Chunk* chunk, int offset)
{
    uint8_t constant = chunk->code[offset + 1];
    uint8_t argCount = chunk->code[offset + 2];
    printf("-%16d (%d args) %4d '", name, argCount, constant);
    printValue(chunk->constants.values[constant]);
    printf("'\n");
    return offset + 3;
}

/// @brief        disassembles the instruction from the bytecode chunk
/// @param chunk  an uint8_t array that contains the bytecode for the instructions
/// @param offset the current instruction index
/// @return       the index for the next instruction
int disassembleInstruction(Chunk* chunk, int offset)
{
    //prints the instruction number
    printf("%04d ", offset);

    //prints the line
    if (offset > 0 && getLine(chunk, offset) == getLine(chunk, offset - 1))
        printf("    | ");
    else
        printf("%4d ", getLine(chunk, offset));

    //saves the bytecode for the current instruction and switches it to the appropriate debug function
    uint8_t instruction = chunk->code[offset];
    switch (instruction)
    {
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
    case OP_NOT:
        return simpleInstruction("OP_NOT", offset);
    case OP_NIL:
        return simpleInstruction("OP_NIL", offset);
    case OP_TRUE:
        return simpleInstruction("OP_TRUE", offset);
    case OP_FALSE:
        return simpleInstruction("OP_FALSE", offset);
    case OP_POP:
        return simpleInstruction("OP_POP", offset);
    case OP_EQUAL:
        return simpleInstruction("OP_EQUAL", offset);
    case OP_GREATER:
        return simpleInstruction("OP_GREATER", offset);
    case OP_LESS:
        return simpleInstruction("OP_LESS", offset);
    case OP_PRINT:
        return simpleInstruction("OP_PRINT", offset);
    case OP_GET_GLOBAL:
        return constantInstruction("OP_GET_GLOBAL", chunk, offset);
    case OP_DEFINE_GLOBAL:
        return constantInstruction("OP_DEFINE_GLOBAL", chunk, offset);
    case OP_SET_GLOBAL:
        return constantInstruction("OP_SET_GLOBAL", chunk, offset);
    case OP_GET_LOCAL:
        return byteInstruction("OP_GET_LOCAL", chunk, offset);
    case OP_SET_LOCAL:
        return byteInstruction("OP_SET_LOCAL", chunk, offset);
    case OP_JUMP:
        return jumpInstruction("OP_JUMP", 1, chunk, offset);
    case OP_JUMP_IF_FALSE:
        return jumpInstruction("OP_JUMP_IF_FALSE", 1, chunk, offset);
    case OP_LOOP:
        return jumpInstruction("OP_LOOP", -1, chunk, offset);
    case OP_CALL:
        return byteInstruction("OP_CALL", chunk, offset);
    case OP_GET_UPVALUE:
        return byteInstruction("OP_GET_UPVALUE", chunk, offset);
    case OP_SET_UPVALUE:
        return byteInstruction("OP_SET_UPVALUE", chunk, offset);
    case OP_CLOSURE:
        // Move to the next byte to read the constant index.
        offset++;
        uint8_t constant = chunk->code[offset++];

        // Print the OP_CLOSURE instruction with its associated constant index.
        printf("%-16s %4d ", "OP_CLOSURE", constant);

        // Print the function object associated with the constant index.
        printValue(chunk->constants.values[constant]);
        printf("\n");
        ObjFunction* function = AS_FUNCTION(chunk->constants.values[constant]);

        // Iterate over the function's upvalues and print their details.
        for (int j = 0; j < function->upvalueCount; j++)
        {
            int isLocal = chunk->code[offset++];
            int index = chunk->code[offset++];
            printf("%04d     |                     %s %d\n", offset - 2, isLocal ? "local" : "upvalue", index);
        }
        return offset;
    case OP_CLOSE_UPVALUE:
        return simpleInstruction("OP_CLOSE_UPVALUE", offset);
    case OP_CLASS:
        return constantInstruction("OP_CLASS", chunk, offset);
    case OP_GET_PROPERTY:
        return constantInstruction("OP_GET_PROPERTY", chunk, offset);
    case OP_SET_PROPERTY:
        return constantInstruction("OP_SET_PROPERTY", chunk, offset);
    case OP_METHOD:
        return constantInstruction("OP_METHOD", chunk, offset);
    case OP_INVOKE:
        return invokeInstruction("OP_INVOKE", chunk, offset);
    case OP_INHERIT:
        return simpleInstruction("OP_INHERIT", offset);
    case OP_GET_SUPER:
        return constantInstruction("OP_GET_SUPER", chunk, offset);
    case OP_SUPER_INVOKE:
        return invokeInstruction("OP_SUPER_INVOKE", chunk, offset);
    default:
        printf("Unknown opcode %d\n", instruction);
        return offset + 1;
    }
}