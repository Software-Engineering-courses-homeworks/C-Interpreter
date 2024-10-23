#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "compiler.h"
#include "scanner.h"

typedef struct {
    Token current;
    Token previous;
    bool hadError;
    bool panicMode;
} Parser;

Parser parser;
Chunk* compilingChunk;

/// a standard getter function
/// @return a pointer to the current chunk being compiled
static Chunk* currentChunk() {
    return compilingChunk;
}

/// prints the error message and toggles the flag
/// @param token - the offending token
/// @param message - the error message
static void errorAt(Token* token, const char* message) {
    if(parser.panicMode) return;

    parser.panicMode = true;

    fprintf(stderr, "[line %d].Error", token->line);

    if (token->type == TOKEN_EOF) {
        fprintf(stderr, " at end");
    } else if (token->type == TOKEN_ERROR) {
        // Nothing.
    } else {
        fprintf(stderr, " at '%.*s'", token->length, token->start);
    }

    fprintf(stderr, ": %s\n", message);
    parser.hadError = true;

    fprintf(stderr, ": %s\n", message);
    parser.hadError = true;
}

/// throws an error at the last token consumed
/// @param msg the error message
static void error(const char *msg) {
    errorAt(&parser.previous, msg);
}

/// throws out an error at the current token being consumed
/// @param msg the error message
static void errorAtCurrent(const char* msg) {
    errorAt(&parser.current, msg);
}

/// primes the scanner for scanning the source code
static void advance() {
    //moves the parser one token ahead
    parser.previous = parser.current;

    //iterates through all the initial errors and prints them
    for(;;) {
        parser.current = scanToken();
        if(parser.current.type != TOKEN_ERROR) break;

        errorAtCurrent(parser.current.start);
    }
}

/// The function reads the next token, and checks if matches to the type that was received
/// if not, reported the error
/// @param type
/// @param message
static void consume(TokenType type, const char* message) {
    if (parser.current.type == type) {
        advance();
        return;
    }

    errorAtCurrent(message);
}

/// appends the given instruction byte to the chunk with the relevant line
/// @param byte - the byte that needs appending
static void emitByte(uint8_t byte) {
    writeChunk(currentChunk(), byte, parser.previous.line);
}

/// a function that writes 2 bytes at once for ease of implementation
/// @param byte1 - the opcode byte
/// @param byte2 - the operand byte
static void emitBytes(uint8_t byte1, uint8_t byte2) {
    emitByte(byte1);
    emitByte(byte2);
}

/// emits a return instruction to the bytecode
static void emitReturn() {
    emitByte(OP_RETURN);
}

/// ends the compilation of the chunk
static void endCompiler() {
    emitReturn();
}

/// compiles the source code
/// @param source - the source code that needs to be compiled
/// @param chunk - the bytecode chunk we feed teh tokenized source code to
/// @return returns whether the code had error in it
bool compile(const char *source, Chunk* chunk) {
    //initializes the scanner with the source string
    ///TO BE RECORDED
    initScanner(source);

    //initializes the compiling chunk
    compilingChunk = chunk;

    //initializes the parser
    parser.hadError = false;
    parser.panicMode = false;

    advance();
    expression();
    consume(TOKEN_EOF,"Expect end of expression");

    endCompiler();
    return !parser.hadError;
}