#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "compiler.h"

#include "debug.h"
#include "scanner.h"

#ifdef DEBUG_PRINT_CODE
#include "debug.h"
#endif

typedef struct {
    Token current;
    Token previous;
    bool hadError;
    bool panicMode;
} Parser;

typedef enum {
    PREC_NONE,
    PREC_ASSIGNMENT, // =
    PREC_OR,         // or
    PREC_AND,        // and
    PREC_EQUALITY,   // == !=
    PREC_COMPARISON, // < > <= >=
    PREC_TERM,       // + -
    PREC_FACTOR,     // * /
    PREC_UNARY,      // ! -
    PREC_CALL,       // . ()
    PREC_PRIMARY
} Precedence;

typedef void (*ParseFn)();

typedef struct {
    ParseFn prefix;
    ParseFn infix;
    Precedence precedence;
}ParseRule;

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

static uint32_t makeConstant(Value value) {
    int constant = addConstant(currentChunk(), value);

    if (constant > UINT24_MAX) {
        error("Too many constants in one chunk.");
        return 0;
    }

    return (uint32_t)constant;
}

/// emits the constant byteCode to the chunk using the writeConstant function because of extended functionality
/// @param value - the value that needs appending
static void emitConstant(Value value) {
    uint32_t constant = makeConstant(value);

    if(constant <= UINT8_MAX)
    {
        emitBytes(OP_CONSTANT,constant);
    }
    //if the constant can't fit in an uint8_t, add it to the chunk using the OP_CONSTANT_LONG opcode
    else if(constant <= UINT24_MAX)
    {
        // //writes the instruction opcode to the chunk
        // //splits the constant index into 3 bytes and writes them in a little endian style
        emitBytes(OP_CONSTANT_LONG,constant);
        emitBytes(constant >> 8, constant >> 16);
    }
}

/// ends the compilation of the chunk
static void endCompiler() {
    emitReturn();

#ifdef DEBUG_PRINT_CODE
    if(!parser.hadError) {
        disassembleChunk(currentChunk(), "code");
    }
#endif
}

static void expression();
static ParseRule* getRule(TokenType type);
static void parsePrecedence(Precedence precedence);

/// converts the token value to a double that we can append to the bytecode chunk
static void number() {
    double value = strtod(parser.current.start, NULL);
    emitConstant(value);
}

/// The function receives a precedence level and parses expressions that have an operator precedence
/// //equals or higher than the provided one
/// @param precedence
static void parsePrecedence(Precedence precedence) {
    advance();
    ParseFn prefixRule = getRule(parser.previous.type)->prefix;
    if (prefixRule == NULL) {
        error("Expect expression.");
        return;
    }

    prefixRule();

    while (precedence <= getRule(parser.current.type)->precedence) {
        advance();
        ParseFn infixRule = getRule(parser.previous.type)->infix;
        infixRule();
    }
}


/// handles the rest of the arithmetic operators. emits the byte code of the instruction
static void binary() {
    TokenType operatorType = parser.previous.type;
    ParseRule* rule = getRule(operatorType);
    parsePrecedence((Precedence)rule->precedence + 1);

    switch (operatorType) {
        case TOKEN_PLUS: emitByte(OP_ADD); break;
        case TOKEN_MINUS: emitByte(OP_SUBTRACT); break;
        case TOKEN_STAR: emitByte(OP_MULTIPLY); break;
        case TOKEN_SLASH: emitByte(OP_DIVIDE); break;
        default: return; //Unreachable
    }
}

///
static void expression() {
    parsePrecedence(PREC_ASSIGNMENT);
}

/// evaluates the value in the expression and then negates it
static void unary() {
    TokenType operatorType = parser.current.type;

    //Compile the operand.
    parsePrecedence(PREC_UNARY);

    //Emit the operator instruction
    switch (operatorType) {
        case TOKEN_MINUS: emitByte(OP_NEGATE); break;
        default: return; //Unreachable.
    }
}

/// call expression to compile what inside the parentheses, and then parse the closing
static void grouping() {
    expression();
    consume(TOKEN_RIGHT_PAREN, "Expected ')' after expression.");
}

ParseRule rules[] = {
  [TOKEN_LEFT_PAREN]    = {grouping, NULL,   PREC_NONE},
  [TOKEN_RIGHT_PAREN]   = {NULL,     NULL,   PREC_NONE},
  [TOKEN_LEFT_BRACE]    = {NULL,     NULL,   PREC_NONE},
  [TOKEN_RIGHT_BRACE]   = {NULL,     NULL,   PREC_NONE},
  [TOKEN_COMMA]         = {NULL,     NULL,   PREC_NONE},
  [TOKEN_DOT]           = {NULL,     NULL,   PREC_NONE},
  [TOKEN_MINUS]         = {unary,    binary, PREC_TERM},
  [TOKEN_PLUS]          = {NULL,     binary, PREC_TERM},
  [TOKEN_SEMICOLON]     = {NULL,     NULL,   PREC_NONE},
  [TOKEN_SLASH]         = {NULL,     binary, PREC_FACTOR},
  [TOKEN_STAR]          = {NULL,     binary, PREC_FACTOR},
  [TOKEN_BANG]          = {NULL,     NULL,   PREC_NONE},
  [TOKEN_BANG_EQUAL]    = {NULL,     NULL,   PREC_NONE},
  [TOKEN_EQUAL]         = {NULL,     NULL,   PREC_NONE},
  [TOKEN_EQUAL_EQUAL]   = {NULL,     NULL,   PREC_NONE},
  [TOKEN_GREATER]       = {NULL,     NULL,   PREC_NONE},
  [TOKEN_GREATER_EQUAL] = {NULL,     NULL,   PREC_NONE},
  [TOKEN_LESS]          = {NULL,     NULL,   PREC_NONE},
  [TOKEN_LESS_EQUAL]    = {NULL,     NULL,   PREC_NONE},
  [TOKEN_IDENTIFIER]    = {NULL,     NULL,   PREC_NONE},
  [TOKEN_STRING]        = {NULL,     NULL,   PREC_NONE},
  [TOKEN_NUMBER]        = {number,   NULL,   PREC_NONE},
  [TOKEN_AND]           = {NULL,     NULL,   PREC_NONE},
  [TOKEN_CLASS]         = {NULL,     NULL,   PREC_NONE},
  [TOKEN_ELSE]          = {NULL,     NULL,   PREC_NONE},
  [TOKEN_FALSE]         = {NULL,     NULL,   PREC_NONE},
  [TOKEN_FOR]           = {NULL,     NULL,   PREC_NONE},
  [TOKEN_FUN]           = {NULL,     NULL,   PREC_NONE},
  [TOKEN_IF]            = {NULL,     NULL,   PREC_NONE},
  [TOKEN_NIL]           = {NULL,     NULL,   PREC_NONE},
  [TOKEN_OR]            = {NULL,     NULL,   PREC_NONE},
  [TOKEN_PRINT]         = {NULL,     NULL,   PREC_NONE},
  [TOKEN_RETURN]        = {NULL,     NULL,   PREC_NONE},
  [TOKEN_SUPER]         = {NULL,     NULL,   PREC_NONE},
  [TOKEN_THIS]          = {NULL,     NULL,   PREC_NONE},
  [TOKEN_TRUE]          = {NULL,     NULL,   PREC_NONE},
  [TOKEN_VAR]           = {NULL,     NULL,   PREC_NONE},
  [TOKEN_WHILE]         = {NULL,     NULL,   PREC_NONE},
  [TOKEN_ERROR]         = {NULL,     NULL,   PREC_NONE},
  [TOKEN_EOF]           = {NULL,     NULL,   PREC_NONE},
};

static ParseRule* getRule(TokenType type) {
    return &rules[type];
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