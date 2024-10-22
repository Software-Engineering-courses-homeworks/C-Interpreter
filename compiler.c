#include <stdio.h>
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
    parser.previous = parser.current;

    for(;;) {
        parser.current = scanToken();
        if(parser.current.type != TOKEN_ERROR) break;

        errorAtCurrent(parser.current.start);
    }
}

/// compiles the source code
/// @param source 
/// @param chunk 
/// @return returns whether the code had error in it
bool compile(const char *source, Chunk* chunk) {
    //initializes the scanner with the source string
    initScanner(source);

    //primes the compiler
    advance();
    expression();
    consume(TOKEN_EOF, "Expected end of expression.");

    return !parser.hadError;
}