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

/// checks if the current token type in the parser is the same as the one given to the function
/// @param type the type that needs comparing
/// @return true is the type matches, false otherwise
static bool check(TokenType type) {
    return parser.current.type == type;
}

/// matches a given type to the parser current type
/// @param type the type that we want to match
/// @return true is the match was successful and the token was consumed, false otherwise
static bool match(TokenType type) {
    if(!check(type)) return false;
    advance();
    return true;
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
        emitBytes(OP_CONSTANT, constant);
    }
    //if the constant can't fit in an uint8_t, add it to the chunk using the OP_CONSTANT_LONG opcode
    else if(constant <= UINT24_MAX)
    {
        //writes the instruction opcode to the chunk
        //splits the constant index into 3 bytes and writes them in a little endian style
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
static void statement();
static void declaration();
static ParseRule* getRule(TokenType type);
static void parsePrecedence(Precedence precedence);

/// converts the token value to a double that we can append to the bytecode chunk
static void number() {
    double value = strtod(parser.previous.start, NULL);
    emitConstant(NUMBER_VAL(value));
}

/// emits the string bytecode
static void string() {
    emitConstant(OBJ_VAL(copyString(parser.previous.start + 1, parser.previous.length - 2)));
}

/// The function receives a precedence level and parses expressions that have an operator precedence
/// //equals or higher than the provided one
/// @param precedence the current precedence
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

/// creates a constant from an identifier token
/// @param name a pointer to a token that represents the identifier
/// @return the index of the created constant
static uint8_t identifierConstant(Token* name) {
    return makeConstant(OBJ_VAL(copyString(name->start, name->length)));
}

/// a helper function to get the variable value
/// @param name the name of the variable
static void namedVariable(Token name) {
    uint8_t arg = identifierConstant(&name);

    //if there is an equals sign after the identifier we compile the assige value
    if (match(TOKEN_EQUAL)) {
        expression();
        emitBytes(OP_SET_GLOBAL,arg);
    }
    else {
        emitBytes(OP_GET_GLOBAL,arg);;
    }
}

/// allows for the referencing of variables
static void variable() {
    namedVariable(parser.previous);
}

/// parse a variable identifier from the token stream
/// @param errorMessage the message we want to print if the identifier is not found
/// @return the index of the constant in the constant table
static uint8_t parseVariable(const char* errorMessage) {
    consume(TOKEN_IDENTIFIER, errorMessage);
    return identifierConstant(&parser.previous);
}

///defines a global variable in the bytecode
static void defineVariable(uint8_t global) {
    emitBytes(OP_DEFINE_GLOBAL, global);
}

/// handles the rest of the arithmetic operators. emits the byte code of the instruction
static void binary() {
    TokenType operatorType = parser.previous.type;
    ParseRule* rule = getRule(operatorType);
    parsePrecedence((Precedence)(rule->precedence + 1));

    switch (operatorType) {
        case TOKEN_PLUS: emitByte(OP_ADD); break;
        case TOKEN_MINUS: emitByte(OP_SUBTRACT); break;
        case TOKEN_STAR: emitByte(OP_MULTIPLY); break;
        case TOKEN_SLASH: emitByte(OP_DIVIDE); break;
        case TOKEN_BANG_EQUAL: emitBytes(OP_EQUAL, OP_NOT); break;
        case TOKEN_EQUAL_EQUAL: emitByte(OP_EQUAL); break;
        case TOKEN_GREATER: emitByte(OP_GREATER); break;
        case TOKEN_GREATER_EQUAL: emitBytes(OP_LESS, OP_NOT); break;
        case TOKEN_LESS: emitByte(OP_LESS); break;
        case TOKEN_LESS_EQUAL: emitBytes(OP_GREATER, OP_NOT); break;
        default: return; //Unreachable
    }
}

/// a function to load the bytecode for boolean and nil values
static void literal() {
    switch(parser.previous.type) {
        case TOKEN_FALSE: emitByte(OP_FALSE); break;
        case TOKEN_TRUE: emitByte(OP_TRUE); break;
        case TOKEN_NIL: emitByte(OP_NIL); break;
        default: return;
    }
}

///reads an expression and compiles it
static void expression() {
    parsePrecedence(PREC_ASSIGNMENT);
}

/// a function to handle the declaration of global variables
static void varDeclaration() {
    //parses the variable
    uint8_t global = parseVariable("Expect variable name");

    //applies the value to the variable if there is one applied upon declaration.
    //else, sets the value to nil
    if(match(TOKEN_EQUAL)) {
        expression();
    }
    else {
        emitByte(OP_NIL);
    }
    //consumes the semicolon token at the end of the declaration
    consume(TOKEN_SEMICOLON, "Expected ';' after variable declaration");

    //defines the global variable and it's value
    defineVariable(global);
}

/// the function executes the expression and consumes the semicolon
static void expressionStatement() {
    expression();
    consume(TOKEN_SEMICOLON, "Expect ';' expression.");
    emitByte(OP_POP);
}

/// compiles the expression, consumes the semicolon token and emits a print opcode
static void printStatement() {
    expression();
    consume(TOKEN_SEMICOLON, "Expected ';' after statement.");
    emitByte(OP_PRINT);
}

///synchronizes the program after encountering a compilation error error
static void synchronize() {
    parser.panicMode = false;

    while(parser.current.type != TOKEN_EOF) {
        if(parser.previous.type == TOKEN_SEMICOLON) return;
        switch (parser.current.type) {
            case TOKEN_CLASS:
            case TOKEN_FUN:
            case TOKEN_VAR:
            case TOKEN_FOR:
            case TOKEN_IF:
            case TOKEN_WHILE:
            case TOKEN_PRINT:
            case TOKEN_RETURN:
                return;
            default:
                ;
        }

        advance();
    }
}

/// compiles a declaration
static void declaration() {
    if(match(TOKEN_VAR)) {
        varDeclaration();
    }
    else {
        statement();
    }

    if(parser.panicMode) synchronize();
}

/// compiles a statement
static void statement() {
    if(match(TOKEN_PRINT)) {
        printStatement();
    }
    else {
        expressionStatement();
    }
}

/// evaluates the value in the expression and then negates it
static void unary() {
    TokenType operatorType = parser.previous.type;

    //Compile the operand.
    parsePrecedence(PREC_UNARY);

    //Emit the operator instruction
    switch (operatorType) {
        case TOKEN_BANG : emitByte(OP_NOT); break;
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
  [TOKEN_BANG]          = {unary,     NULL,   PREC_NONE},
  [TOKEN_BANG_EQUAL]    = {NULL,     binary,   PREC_COMPARISON},
  [TOKEN_EQUAL]         = {NULL,     NULL,   PREC_NONE},
  [TOKEN_EQUAL_EQUAL]   = {NULL,     binary,   PREC_EQUALITY},
  [TOKEN_GREATER]       = {NULL,     binary,   PREC_COMPARISON},
  [TOKEN_GREATER_EQUAL] = {NULL,     binary,   PREC_COMPARISON},
  [TOKEN_LESS]          = {NULL,     binary,   PREC_COMPARISON},
  [TOKEN_LESS_EQUAL]    = {NULL,     binary,   PREC_COMPARISON},
  [TOKEN_IDENTIFIER]    = {variable,     NULL,   PREC_NONE},
  [TOKEN_STRING]        = {string,     NULL,   PREC_NONE},
  [TOKEN_NUMBER]        = {number,   NULL,   PREC_NONE},
  [TOKEN_AND]           = {NULL,     NULL,   PREC_NONE},
  [TOKEN_CLASS]         = {NULL,     NULL,   PREC_NONE},
  [TOKEN_ELSE]          = {NULL,     NULL,   PREC_NONE},
  [TOKEN_FALSE]         = {literal,     NULL,   PREC_NONE},
  [TOKEN_FOR]           = {NULL,     NULL,   PREC_NONE},
  [TOKEN_FUN]           = {NULL,     NULL,   PREC_NONE},
  [TOKEN_IF]            = {NULL,     NULL,   PREC_NONE},
  [TOKEN_NIL]           = {literal,     NULL,   PREC_NONE},
  [TOKEN_OR]            = {NULL,     NULL,   PREC_NONE},
  [TOKEN_PRINT]         = {NULL,     NULL,   PREC_NONE},
  [TOKEN_RETURN]        = {NULL,     NULL,   PREC_NONE},
  [TOKEN_SUPER]         = {NULL,     NULL,   PREC_NONE},
  [TOKEN_THIS]          = {NULL,     NULL,   PREC_NONE},
  [TOKEN_TRUE]          = {literal,     NULL,   PREC_NONE},
  [TOKEN_VAR]           = {NULL,     NULL,   PREC_NONE},
  [TOKEN_WHILE]         = {NULL,     NULL,   PREC_NONE},
  [TOKEN_ERROR]         = {NULL,     NULL,   PREC_NONE},
  [TOKEN_EOF]           = {NULL,     NULL,   PREC_NONE},
};

/// a wrapper function that returns the precedence rule for a token type
/// @param type - the token type whose precedence needs to be checked
/// @return returns the parse rule for that token type
static ParseRule* getRule(TokenType type) {
    return &rules[type];
}

/// compiles the source code
/// @param source - the source code that needs to be compiled
/// @param chunk - the bytecode chunk we feed teh tokenized source code to
/// @return returns whether the code had error in it
bool compile(const char *source, Chunk* chunk) {
    //initializes the scanner with the source string
    initScanner(source);

    //initializes the compiling chunk
    compilingChunk = chunk;

    //initializes the parser
    parser.hadError = false;
    parser.panicMode = false;

    advance();
    while(!match(TOKEN_EOF)) {
        declaration();
    }

    endCompiler();
    return !parser.hadError;
}