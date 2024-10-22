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

/// prints the error message and toggles the flag
/// @param token the offending token
/// @param message the error message
static void errorAt(Token* token, const char* message) {
    fprintf(stderr, "[line %d].Error", token->line);

    if (token->type == TOKEN_EOF) {
        fprintf(stderr, " at end");
    } else if (token->type == TOKEN_ERROR) {
        //Nothing.
    }else {
        fprintf(stderr, ": %s\n", message);
        parser.hadError = true;
    }

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

/// compiles the source code
/// @param source
/// @param chunk
/// @return returns whether the code had error in it
bool compile(const char *source, Chunk* chunk) {
    //initializes the scanner with the source string
    initScanner(source);

    //sets the line to -1
    int line = -1;

    //runs the scanner and generates tokens on the fly
    for(;;) {
        //scans the lexeme and saves the return token
        Token token = scanToken();

        //if the line is differnt then the last line printed, print is and update 'line'
        //else, print a | to indicate it's the same line of code
        if(token.line != line) {
            printf("%4d ", token.line);
            line = token.line;
        }
        else {
            printf("    | ");
        }
        //prints the operation token
        printf("%2d '%.*s'\n", token.type, token.length, token.start);

        //stop the loop when reaching EOF
        if(token.type == TOKEN_EOF)
            break;
    }
}