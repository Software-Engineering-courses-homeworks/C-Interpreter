#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "compiler.h"
#include <string.h>
#include "debug.h"
#include "memory.h"
#include "scanner.h"
#include "memory.h"

#ifdef DEBUG_PRINT_CODE

#include "debug.h"

#endif

// a struct to hold the current and previous parsed tokens
typedef struct
{
    Token current;
    Token previous;
    bool hadError;
    bool panicMode;
} Parser;

// a struct to hold the precedence of the operators
typedef enum
{
    PREC_NONE,
    PREC_ASSIGNMENT, // =
    PREC_OR, // or
    PREC_AND, // and
    PREC_EQUALITY, // == !=
    PREC_COMPARISON, // < > <= >=
    PREC_TERM, // + -
    PREC_FACTOR, // * /
    PREC_UNARY, // ! -
    PREC_CALL, // . ()
    PREC_PRIMARY
} Precedence;

// a function pointer to parse the prefix and infix expressions
typedef void (*ParseFn)(bool canAssign);

// a struct to hold the rules for parsing the expressions
typedef struct
{
    ParseFn prefix;
    ParseFn infix;
    Precedence precedence;
} ParseRule;

// a struct to hold the local variables
typedef struct
{
    Token name;
    int depth;
    bool isCaptured;
} Local;

// a struct to hold the upvalues
typedef struct
{
    uint8_t index;
    bool isLocal;
} Upvalue;

// a struct to hold the function types
typedef enum
{
    TYPE_FUNCTION,
    TYPE_SCRIPT,
    TYPE_METHOD,
    TYPE_INITIALIZER,
} FunctionType;

// a struct to hold the compilers in a linked list
typedef struct Compiler
{
    struct Compiler* enclosing;
    ObjFunction* function;
    FunctionType type;
    Local locals[UINT8_COUNT];
    int localCount;
    Upvalue upvalues[UINT8_COUNT];
    int scopeDepth;
} Compiler;

// a struct for a linked list of class compilers
typedef struct ClassCompiler
{
    struct ClassCompiler* enclosing;
    bool hasSuperClass;
} ClassCompiler;

// global variables
Parser parser;
Compiler* current = NULL;
ClassCompiler* currentClass = NULL;

/// a standard getter function
/// @return a pointer to the current chunk being compiled
static Chunk* currentChunk()
{
    return &current->function->chunk;
}

/// prints the error message and toggles the flag
/// @param token    the offending token
/// @param message  the error message
static void errorAt(Token* token, const char* message)
{
    if (parser.panicMode) return;

    parser.panicMode = true;

    fprintf(stderr, "[line %d].Error", token->line);

    if (token->type == TOKEN_EOF)
    {
        fprintf(stderr, " at end");
    }
    else if (token->type == TOKEN_ERROR)
    {
        // Nothing.
    }
    else
    {
        fprintf(stderr, " at '%.*s'", token->length, token->start);
    }

    fprintf(stderr, ": %s\n", message);
    parser.hadError = true;

    fprintf(stderr, ": %s\n", message);
    parser.hadError = true;
}

/// throws an error at the last token consumed
/// @param msg the error message
static void error(const char* msg)
{
    errorAt(&parser.previous, msg);
}

/// throws out an error at the current token being consumed
/// @param msg the error message
static void errorAtCurrent(const char* msg)
{
    errorAt(&parser.current, msg);
}

/// primes the scanner for scanning the source code
static void advance()
{
    //moves the parser one token ahead
    parser.previous = parser.current;

    //iterates through all the initial errors and prints them
    for (;;)
    {
        parser.current = scanToken();
        if (parser.current.type != TOKEN_ERROR) break;

        errorAtCurrent(parser.current.start);
    }
}

/// The function reads the next token, and checks if matches to the type that was received
/// if not, reported the error
/// @param type     the type that needs to be matched
/// @param message  the error message
static void consume(TokenType type, const char* message)
{
    if (parser.current.type == type)
    {
        advance();
        return;
    }

    errorAtCurrent(message);
}

/// checks if the current token type in the parser is the same as the one given to the function
/// @param type the type that needs comparing
/// @return     true is the type matches, false otherwise
static bool check(TokenType type)
{
    return parser.current.type == type;
}

/// matches a given type to the parser current type
/// @param type the type that we want to match
/// @return     true is the match was successful and the token was consumed, false otherwise
static bool match(TokenType type)
{
    if (!check(type)) return false;
    advance();
    return true;
}

/// appends the given instruction byte to the chunk with the relevant line
/// @param byte the byte that needs appending
static void emitByte(uint8_t byte)
{
    writeChunk(currentChunk(), byte, parser.previous.line);
}

/// a function that writes 2 bytes at once for ease of implementation
/// @param byte1 the opcode byte
/// @param byte2 the operand byte
static void emitBytes(uint8_t byte1, uint8_t byte2)
{
    emitByte(byte1);
    emitByte(byte2);
}

/// a helper function to emit the loop bytecode
/// @param loopStart the start index for the loop block
static void emitLoop(int loopStart)
{
    emitByte(OP_LOOP);

    int offset = currentChunk()->count - loopStart + 2;
    if (offset > UINT16_MAX) error("Loop body too large");

    emitByte((offset >> 8) & 0xFF);
    emitByte(offset & 0xFF);
}

/// the function emits a jump instruction to the bytecode with filler operands and returns the filler index
/// @param instruction  the jump instruction
/// @return             the index for the jump operands
static int emitJump(uint8_t instruction)
{
    emitByte(instruction);
    emitByte(0xff);
    emitByte(0xff);
    return currentChunk()->count - 2;
}

/// emits a return instruction to the bytecode
static void emitReturn()
{
    if (current->type == TYPE_INITIALIZER)
    {
        emitBytes(OP_GET_LOCAL, 0);
    }
    else
    {
        emitByte(OP_NIL);
    }
    emitByte(OP_RETURN);
}

/// a function to emit the constant value to the chunk
/// @param value the value that needs to be appended
/// @return      the index of the constant in the chunk
static uint32_t makeConstant(Value value)
{
    int constant = addConstant(currentChunk(), value);

    if (constant > UINT24_MAX)
    {
        error("Too many constants in one chunk.");
        return 0;
    }

    return (uint32_t)constant;
}

/// emits the constant byteCode to the chunk using the writeConstant function because of extended functionality
/// @param value the value that needs appending
static void emitConstant(Value value)
{
    uint32_t constant = makeConstant(value);

    if (constant <= UINT8_MAX)
    {
        emitBytes(OP_CONSTANT, constant);
    }
    //if the constant can't fit in an uint8_t, add it to the chunk using the OP_CONSTANT_LONG opcode
    else if (constant <= UINT24_MAX)
    {
        //writes the instruction opcode to the chunk
        //splits the constant index into 3 bytes and writes them in a little endian style
        emitBytes(OP_CONSTANT_LONG, (uint8_t)constant);
        emitBytes((uint8_t)(constant >> 8), (uint8_t)(constant >> 16));
    }
}

/// the function replaces the jump operand filler with the number of bytes that needs to be jumped over in the bytecode
/// @param offset the jump offset
static void patchJump(int offset)
{
    int jump = currentChunk()->count - offset - 2;

    if (jump > UINT16_MAX)
    {
        error("Too much code to jump over");
    }

    currentChunk()->code[offset] = (jump >> 8) & 0xff;
    currentChunk()->code[offset + 1] = jump & 0xff;
}

/// gets a compiler struct and initializes it
/// @param compiler the compiler scope and depth
static void initCompiler(Compiler* compiler, FunctionType type)
{
    compiler->enclosing = current;
    compiler->function = NULL;
    compiler->type = type;
    compiler->localCount = 0;
    compiler->scopeDepth = 0;
    compiler->function = newFunction();
    current = compiler;

    // if the function isn't the main function, copies it's parsed name.
    if (type != TYPE_SCRIPT)
    {
        current->function->name = copyString(parser.previous.start, parser.previous.length);
    }

    Local* local = &current->locals[current->localCount++];
    local->depth = 0;
    local->isCaptured = false;

    if (type != TYPE_FUNCTION)
    {
        local->name.start = "this";
        local->name.length = 4;
    }
    else
    {
        local->name.start = "";
        local->name.length = 0;
    }
}

/// ends the compilation of the chunk
static ObjFunction* endCompiler()
{
    emitReturn();
    ObjFunction* function = current->function;

#ifdef DEBUG_PRINT_CODE
    if (!parser.hadError)
    {
        disassembleChunk(currentChunk(), function->name != NULL ? function->name->chars : "<script>");
    }
#endif

    // pops the old compiler from the stack and replaces the current one with the enclosing one
    current = current->enclosing;
    return function;
}

/// a helper function to increase the compiler scope at the start of a block
static void beginScope()
{
    current->scopeDepth++;
}

/// a helper function to decrease teh compiler scope at the end of the block
static void endScope()
{
    current->scopeDepth--;

    //removes all the local variables from the scope that was ended
    while (current->localCount > 0 && current->locals[current->localCount - 1].depth > current->scopeDepth)
    {
        if (current->locals[current->localCount - 1].isCaptured)
        {
            emitByte(OP_CLOSE_UPVALUE);
        }
        else
        {
            emitByte(OP_POP);
        }
        current->localCount--;
    }
}

// function prototypes for the recursive descent parser
static void expression();
static void statement();
static void declaration();
static ParseRule* getRule(TokenType type);
static void parsePrecedence(Precedence precedence);

/// converts the token value to a double that we can append to the bytecode chunk
static void number(bool canAssign)
{
    double value = strtod(parser.previous.start, NULL);
    emitConstant(NUMBER_VAL(value));
}

/// a function for the "or" operator
/// @param canAssign a flag to check if the operator can be assigned
static void or_(bool canAssign)
{
    int elseJump = emitJump(OP_JUMP_IF_FALSE);
    int endJump = emitJump(OP_JUMP);

    patchJump(elseJump);
    emitByte(OP_POP);

    parsePrecedence(PREC_OR);
    patchJump(endJump);
}

/// emits the string bytecode
/// @param canAssign a flag to check if the operator can be assigned
static void string(bool canAssign)
{
    emitConstant(OBJ_VAL(copyString(parser.previous.start + 1, parser.previous.length - 2)));
}

/// The function receives a precedence level and parses expressions that have an operator precedence
/// equals or higher than the provided one
/// @param precedence the current precedence
static void parsePrecedence(Precedence precedence)
{
    advance();
    ParseFn prefixRule = getRule(parser.previous.type)->prefix;
    if (prefixRule == NULL)
    {
        error("Expect expression.");
        return;
    }
    bool canAssign = precedence <= PREC_ASSIGNMENT;
    prefixRule(canAssign);

    while (precedence <= getRule(parser.current.type)->precedence)
    {
        advance();
        ParseFn infixRule = getRule(parser.previous.type)->infix;
        infixRule(canAssign);
    }

    if (canAssign && match(TOKEN_EQUAL))
    {
        error("Invalid assignment target");
    }
}

/// creates a constant from an identifier token
/// @param name a pointer to a token that represents the identifier
/// @return     the index of the created constant
static uint8_t identifierConstant(Token* name)
{
    return makeConstant(OBJ_VAL(copyString(name->start, name->length)));
}

/// adds the variable to the local variable pool in the compiler
/// @param name the name of the variable
static void addLocal(Token name)
{
    //checks if the new variable is over the stack size limit
    if (current->localCount == UINT8_COUNT)
    {
        error("Too many local variables in function.");
        return;
    }

    Local* local = &current->locals[current->localCount++];

    local->name = name;
    local->depth = -1;
    local->isCaptured = false;
}

/// the function gets to local variable tokens and compares them
/// @param a    the first local token
/// @param b    the second local token
/// @return     true if they're equal, false otherwise
static bool identifiersEqual(Token* a, Token* b)
{
    if (a->length != b->length) return false;
    return memcmp(a->start, b->start, a->length) == 0;
}

/// checks if a variable is a global or local one
/// @param compiler the compiler with the local variables array
/// @param name     the name of the variable
/// @return         returns the index of the local variable in the local pool
static int resolveLocal(Compiler* compiler, Token* name)
{
    for (int i = compiler->localCount - 1; i >= 0; i--)
    {
        Local* local = &compiler->locals[i];
        if (identifiersEqual(name, &local->name))
        {
            if (local->depth == -1)
            {
                error("Can't read local variable in its own initializer");
            }
            return i;
        }
    }

    //if we didn't find the variable in the local pool then it's a global one.
    return -1;
}

/// adds an upvalue to the upvalue array in the function
/// @param compiler the compiler with the upvalue array
/// @param index    the index of the upvalue
/// @param isLocal  a flag to check if the upvalue is local
/// @return returns the index of the upvalue in the upvalue array
static int addUpvalue(Compiler* compiler, uint8_t index, bool isLocal)
{
    int upvalueCount = compiler->function->upvalueCount;

    for (int i = 0; i < upvalueCount; i++)
    {
        Upvalue* upvalue = &compiler->upvalues[i];

        if (upvalue->index == index && upvalue->isLocal == isLocal)
        {
            return i;
        }
    }
    // checks if there is space for more closed over upvalues
    if (upvalueCount == UINT8_COUNT)
    {
        error("Too many closure variables in function.");
        return 0;
    }

    compiler->upvalues[upvalueCount].isLocal = isLocal;
    compiler->upvalues[upvalueCount].index = index;
    return compiler->function->upvalueCount++;
}

/// resolves the upvalue in the function
/// @param compiler the compiler with the upvalue array
/// @param name     the name of the upvalue
/// @return         the index of the upvalue in the upvalue array
static int resolveUpvalue(Compiler* compiler, Token* name)
{
    if (compiler->enclosing == NULL) return -1;

    int local = resolveLocal(compiler->enclosing, name);
    if (local != -1)
    {
        compiler->enclosing->locals[local].isCaptured = true;
        return addUpvalue(compiler, (uint8_t)local, true);
    }

    int upvalue = resolveUpvalue(compiler->enclosing, name);
    if (upvalue != -1)
    {
        return addUpvalue(compiler, (uint8_t)upvalue, false);
    }

    return -1;
}

/// the function records the existence of the local variable to the compiler
static void declareVariable()
{
    //if the variable is a global, leave the function and define it using the global infrastructure
    if (current->scopeDepth == 0) return;

    //adds the local variable to the compiler
    Token* name = &parser.previous;
    //checks if there is no local identical to the new local in the current scope
    for (int i = current->localCount - 1; i >= 0; i--)
    {
        Local* local = &current->locals[i];
        if (local->depth != -1 && local->depth < current->scopeDepth)
        {
            break;
        }

        //if there is another local on the stack that's the same as the new local in the same scope, report an error
        if (identifiersEqual(name, &local->name))
        {
            error("Already a variable with this name in this scope");
        }
    }
    addLocal(*name);
}

/// a helper function to get or set a variable value
/// @param name         the name of a variable
/// @param canAssign    whether a value can be assigned to the variable
static void namedVariable(Token name, bool canAssign)
{
    //checks if the variable in question is a local or global variable and sets the opcodes accordingly
    uint8_t getOp, setOp;
    int arg = resolveLocal(current, &name);
    if (arg != -1)
    {
        getOp = OP_GET_LOCAL;
        setOp = OP_SET_LOCAL;
    }
    else if ((arg = resolveUpvalue(current, &name)) != -1)
    {
        getOp = OP_GET_UPVALUE;
        setOp = OP_SET_UPVALUE;
    }
    else
    {
        arg = identifierConstant(&name);
        getOp = OP_GET_GLOBAL;
        setOp = OP_SET_GLOBAL;
    }

    //if there is an equals sign after the identifier we compile the assigned value
    if (canAssign && match(TOKEN_EQUAL))
    {
        expression();
        emitBytes(setOp, (uint8_t)arg);
    }
    else
    {
        emitBytes(getOp, (uint8_t)arg);
    }
}

/// a function that handles the function variables and their number
static uint8_t argumentList()
{
    uint8_t argCount = 0;
    // compiles and counts the number of arguments being passed to the function
    if (!check(TOKEN_RIGHT_PAREN))
    {
        do
        {
            expression();
            if (argCount == 255)
            {
                error("Can't have more than 255 arguments.");
            }
            argCount++;
        }
        while (match(TOKEN_COMMA));
    }

    // consumes the ')' token at the end of the argument list
    consume(TOKEN_RIGHT_PAREN, "Expect ')' after arguments.");

    return argCount;
}

/// allows for the referencing of variables
static void variable(bool canAssign)
{
    namedVariable(parser.previous, canAssign);
}

/// creates a synthetic token for the tracking of superclasses and subclasses
/// @param text the name of the synthetic token
/// @return     a token of the 'text' argument type
static Token syntheticToken(const char* text)
{
    Token token;
    token.start = text;
    token.length = (int)strlen(text);
    return token;
}

/// the function allows for the compilation and parsing of the super keyword
/// @param canAssign whether the variable can be assigned (non-relevant to this function)
static void super_(bool canAssign)
{
    // checking for the existence of a class and superclass to perform the super operation
    if (currentClass == NULL)
    {
        error("Can't use 'super' outside of a class.");
    }
    else if(!currentClass->hasSuperClass)
    {
        error("Can't use 'super' in a class with no superclass");
    }

    // consumes the dot and property tokens before parsing the arguments
    consume(TOKEN_DOT,"Expect '.' after 'super'");
    consume(TOKEN_IDENTIFIER,"Expect superclass method name");
    uint8_t name = identifierConstant(&parser.previous);

    //looks up the 'this' instance, and it's corresponding 'super' class and push them to the top of the stack
    namedVariable(syntheticToken("this"), false);
    //if the call happens at the moment the super call has happened, invoke it. else, take the long route
    if (match(TOKEN_LEFT_PAREN))
    {
        uint8_t argCount = argumentList();
        namedVariable(syntheticToken("super"), false);
        emitBytes(OP_SUPER_INVOKE,name);
        emitByte(argCount);
    }
    else
    {
        namedVariable(syntheticToken("super"), false);
        emitBytes(OP_GET_SUPER, name);
    }
}

/// a function to parse the 'this' keyword
/// @param canAssign whether the variable can be assigned (non-relevant to this function)
static void this_(bool canAssign)
{
    if (currentClass == NULL)
    {
        error("Cant use 'this' outside of a class");
        return;
    }

    variable(false);
}

/// parse a variable identifier from the token stream
/// @param errorMessage the message we want to print if the identifier is not found
/// @return             the index of the constant in the constant table
static uint8_t parseVariable(const char* errorMessage)
{
    //consumes the identifier token
    consume(TOKEN_IDENTIFIER, errorMessage);

    //declares the variable and if it's not a global one leave the function to not add it to the constant array and leave it on the stack
    declareVariable();
    if (current->scopeDepth > 0) return 0;

    //add the variable to the constant array
    return identifierConstant(&parser.previous);
}

/// a helper function to mark a local variable as initialized at the end of the initialization.
static void markInitialized()
{
    //check if we're in the main function, don't mark it as initialized until it is defined.
    if (current->scopeDepth == 0) return;
    current->locals[current->localCount - 1].depth = current->scopeDepth;
}

///defines a global variable in the bytecode
static void defineVariable(uint8_t global)
{
    //if the variable is local, leave the function
    if (current->scopeDepth > 0)
    {
        markInitialized();
        return;
    }

    //emits the global variable to the bytecode
    emitBytes(OP_DEFINE_GLOBAL, global);
}

/// a parser function for and operator
/// @param canAssign if a value can be assigned (non-relevant)
static void and_(bool canAssign)
{
    int endJump = emitJump(OP_JUMP_IF_FALSE);

    emitByte(OP_POP);
    parsePrecedence(PREC_AND);

    patchJump(endJump);
}

/// handles the rest of the arithmetic operators. emits the byte code of the instruction
/// @param canAssign a flag to check if the operator can be assigned (non-relevant)
static void binary(bool canAssign)
{
    TokenType operatorType = parser.previous.type;
    ParseRule* rule = getRule(operatorType);
    parsePrecedence((Precedence)(rule->precedence + 1));

    switch (operatorType)
    {
    case TOKEN_PLUS:
        emitByte(OP_ADD);
        break;
    case TOKEN_MINUS:
        emitByte(OP_SUBTRACT);
        break;
    case TOKEN_STAR:
        emitByte(OP_MULTIPLY);
        break;
    case TOKEN_SLASH:
        emitByte(OP_DIVIDE);
        break;
    case TOKEN_BANG_EQUAL:
        emitBytes(OP_EQUAL, OP_NOT);
        break;
    case TOKEN_EQUAL_EQUAL:
        emitByte(OP_EQUAL);
        break;
    case TOKEN_GREATER:
        emitByte(OP_GREATER);
        break;
    case TOKEN_GREATER_EQUAL:
        emitBytes(OP_LESS, OP_NOT);
        break;
    case TOKEN_LESS:
        emitByte(OP_LESS);
        break;
    case TOKEN_LESS_EQUAL:
        emitBytes(OP_GREATER, OP_NOT);
        break;
    default:
        return; //Unreachable
    }
}

/// a function to handle the calling of functions
/// @param canAssign a flag to check if the operator can be assigned (non-relevant)
static void call(bool canAssign)
{
    uint8_t argCount = argumentList();
    emitBytes(OP_CALL, argCount);
}

/// accesses a class's fields and methods
/// @param canAssign determines whether the attribute being accessed is a field or a method
static void dot(bool canAssign)
{
    consume(TOKEN_IDENTIFIER, "Expect property name after '.'.");
    uint8_t name = identifierConstant(&parser.previous);

    if (canAssign && match(TOKEN_EQUAL))
    {
        expression();
        emitBytes(OP_SET_PROPERTY, name);
    }
    else if (match(TOKEN_LEFT_PAREN))
    {
        uint8_t argCount = argumentList();
        emitBytes(OP_INVOKE, name);
        emitByte(argCount);
    }
    else
    {
        emitBytes(OP_GET_PROPERTY, name);
    }
}

/// a function to load the bytecode for boolean and nil values
/// @param canAssign a flag to check if the operator can be assigned (non-relevant)
static void literal(bool canAssign)
{
    switch (parser.previous.type)
    {
    case TOKEN_FALSE:
        emitByte(OP_FALSE);
        break;
    case TOKEN_TRUE:
        emitByte(OP_TRUE);
        break;
    case TOKEN_NIL:
        emitByte(OP_NIL);
        break;
    default:
        return;
    }
}

///reads an expression and compiles it
static void expression()
{
    parsePrecedence(PREC_ASSIGNMENT);
}

/// a helper function to process blocks
static void block()
{
    //while inside a block, process the declarations inside it
    while (!check(TOKEN_RIGHT_BRACE) && !check(TOKEN_EOF))
    {
        declaration();
    }

    //consume the end of the right brace at the end of the block
    consume(TOKEN_RIGHT_BRACE, "Expect '}' after block");
}

/// compiles the body of the function
/// @param type the type of the function
static void function(FunctionType type)
{
    Compiler compiler;
    initCompiler(&compiler, type);
    beginScope();

    consume(TOKEN_LEFT_PAREN, "Expect '(' after function name.");
    if (!check(TOKEN_RIGHT_PAREN))
    {
        do
        {
            current->function->arity++;
            if (current->function->arity > 255)
            {
                errorAtCurrent("Can't have more than 255 parameters.");
            }
            uint8_t constant = parseVariable("Expect parameter name.");
            defineVariable(constant);
        }
        while (match(TOKEN_COMMA));
    }
    consume(TOKEN_RIGHT_PAREN, "Expect ')' after function parameters.");
    consume(TOKEN_LEFT_BRACE, "Expect '{' before function body.");
    block();

    ObjFunction* function = endCompiler();
    emitBytes(OP_CLOSURE, makeConstant(OBJ_VAL(function)));

    for (int i = 0; i < function->upvalueCount; i++)
    {
        emitByte(compiler.upvalues[i].isLocal ? 1 : 0);
        emitByte(compiler.upvalues[i].index);
    }
}

/// a function to handle class methods
static void method()
{
    //consume and process the method name
    consume(TOKEN_IDENTIFIER, "Expect method name");

    //create the method name constant and adds it to the constant table
    uint8_t constant = identifierConstant(&parser.previous);

    //processes the method body
    FunctionType type = TYPE_METHOD;

    if (parser.previous.length == 4 && memcmp(parser.previous.start, "init", 4) == 0)
    {
        type = TYPE_INITIALIZER;
    }
    function(type);

    //emits the method instructions
    emitBytes(OP_METHOD, constant);
}

/// a function to handle the declaration of classes
static void classDeclaration()
{
    consume(TOKEN_IDENTIFIER, "Expect class name.");
    Token className = parser.previous;
    uint8_t nameConstant = identifierConstant(&parser.previous);
    declareVariable();

    emitBytes(OP_CLASS, nameConstant);
    defineVariable(nameConstant);

    ClassCompiler classCompiler;
    classCompiler.hasSuperClass=false;
    classCompiler.enclosing = currentClass;
    currentClass = &classCompiler;

    //if an inheritance clause was detected, load the superclass and the subclass onto the stack and emit the inheritance opcode
    if (match(TOKEN_LESS))
    {
        consume(TOKEN_IDENTIFIER, "Expect superclass name.");
        variable(false);

        //checks if a class is trying to inherit itself
        if (identifiersEqual(&className, &parser.previous))
        {
            error("A class can't inherit itself.");
        }

        //begins a scope for the superclass calls
        beginScope();
        addLocal(syntheticToken("super"));
        defineVariable(0);

        //perform the inheritance operation
        namedVariable(className, false);
        emitByte(OP_INHERIT);
        classCompiler.hasSuperClass = true;
    }

    namedVariable(className, false);
    //consumes the left brace at the begining of the class
    consume(TOKEN_LEFT_BRACE, "Expect '{' after class name.");

    while (!check(TOKEN_RIGHT_BRACE) && !check(TOKEN_EOF))
    {
        method();
    }

    //consumes teh right brace at the end of the class definition and declaration
    consume(TOKEN_RIGHT_BRACE, "Expect '}' after class name.");
    emitByte(OP_POP);

    if (classCompiler.hasSuperClass)
    {
        endScope();
    }

    //remove the current class from the compiling class stack
    currentClass = currentClass->enclosing;
}

/// a function to handle the declaration of functions
static void funDeclaration()
{
    //parses the function
    uint8_t global = parseVariable("Except function name.");
    //mark the function as initialized
    markInitialized();
    function(TYPE_FUNCTION);
    defineVariable(global);
}

/// a function to handle the declaration of global variables
static void varDeclaration()
{
    //parses the variable
    uint8_t global = parseVariable("Expect variable name");

    //applies the value to the variable if there is one applied upon declaration.
    //else, sets the value to nil
    if (match(TOKEN_EQUAL))
    {
        expression();
    }
    else
    {
        emitByte(OP_NIL);
    }
    //consumes the semicolon token at the end of the declaration
    consume(TOKEN_SEMICOLON, "Expected ';' after variable declaration");

    //defines the global variable and it's value
    defineVariable(global);
}

/// the function executes the expression and consumes the semicolon
static void expressionStatement()
{
    expression();
    consume(TOKEN_SEMICOLON, "Expect ';' expression.");
    emitByte(OP_POP);
}

/// compiles the for loop syntax
static void forStatement()
{
    beginScope();

    consume(TOKEN_LEFT_PAREN, "Expect '(' after 'for'.");

    //the initializer part if present
    if (match(TOKEN_SEMICOLON))
    {
        // No initializer.
    }
    else if (match(TOKEN_VAR))
    {
        varDeclaration();
    }
    else
    {
        expressionStatement();
    }

    int loopStart = currentChunk()->count;

    int exitJump = -1;
    //the conditional part if present
    if (!match(TOKEN_SEMICOLON))
    {
        expression();
        consume(TOKEN_SEMICOLON, "Expect ';' after loop condition.");

        // Jump out of the loop if the condition is false.
        exitJump = emitJump(OP_JUMP_IF_FALSE);
        emitByte(OP_POP); // pops the condition value
    }

    //the increment part if present
    if (!match(TOKEN_RIGHT_PAREN))
    {
        int bodyJump = emitJump(OP_JUMP);
        int incrementStart = currentChunk()->count;
        expression();
        emitByte(OP_POP);
        consume(TOKEN_RIGHT_PAREN, "Expect ')' after for clause.");

        emitLoop(loopStart);
        loopStart = incrementStart;
        patchJump(bodyJump);
    }

    statement();
    emitLoop(loopStart);

    // patches the exit loop jump
    if (exitJump != -1)
    {
        patchJump(exitJump);
        emitByte(OP_POP); // Condition.
    }

    endScope();
}

/// the function executes the "if" control structure
static void ifStatement()
{
    // consumes the parenthesis and the condition expression for the 'if' statement
    consume(TOKEN_LEFT_PAREN, "Expect '(' after 'if'.");
    expression();
    consume(TOKEN_RIGHT_PAREN, "Expect ')' after 'if'.");

    //calculates the required jump in case of a skip and executes the statement
    int thenJump = emitJump(OP_JUMP_IF_FALSE);
    emitByte(OP_POP);
    statement();

    //calculates the jump in case the 'else' block needs to be skipped
    int elseJump = emitJump(OP_JUMP);

    //sets the jump address after compilation of the "then" block
    patchJump(thenJump);
    emitByte(OP_POP);

    //if an else token exists, compile it
    if (match(TOKEN_ELSE)) statement();
    //replace the jump operand filler with the size of the else block
    patchJump(elseJump);
}

/// compiles the expression, consumes the semicolon token and emits a print opcode
static void printStatement()
{
    expression();
    consume(TOKEN_SEMICOLON, "Expected ';' after statement.");
    emitByte(OP_PRINT);
}

/// compiles the return statement
static void returnStatement()
{
    if (current->type == TYPE_SCRIPT)
    {
        error("Can't return from top-level code.");
    }
    if (match(TOKEN_SEMICOLON))
    {
        emitReturn();
    }
    else
    {
        if (current->type == TYPE_INITIALIZER)
        {
            error("Can't return a value from an initializer");
        }
        expression();
        consume(TOKEN_SEMICOLON, "Expected ';' after return value.");
        emitByte(OP_RETURN);
    }
}

/// a function to compile 'while' statements
static void whileStatement()
{
    int loopStart = currentChunk()->count;
    consume(TOKEN_LEFT_PAREN, "Expect '(' after 'while'.");
    expression();
    consume(TOKEN_RIGHT_PAREN, "Expect ')' after condition.");

    int exitJump = emitJump(OP_JUMP_IF_FALSE);
    emitByte(OP_POP);
    statement();
    emitLoop(loopStart);

    patchJump(exitJump);
    emitByte(OP_POP);
}

///synchronizes the program after encountering a compilation error error
static void synchronize()
{
    parser.panicMode = false;

    while (parser.current.type != TOKEN_EOF)
    {
        if (parser.previous.type == TOKEN_SEMICOLON) return;
        switch (parser.current.type)
        {
        case TOKEN_CLASS:
        case TOKEN_FUN:
        case TOKEN_VAR:
        case TOKEN_FOR:
        case TOKEN_IF:
        case TOKEN_WHILE:
        case TOKEN_PRINT:
        case TOKEN_RETURN:
            return;
        default: ;
        }

        advance();
    }
}

/// compiles a declaration
static void declaration()
{
    if (match(TOKEN_CLASS))
    {
        classDeclaration();
    }
    else if (match(TOKEN_FUN))
    {
        funDeclaration();
    }
    else if (match(TOKEN_VAR))
    {
        varDeclaration();
    }
    else
    {
        statement();
    }

    if (parser.panicMode) synchronize();
}

/// compiles a statement
static void statement()
{
    if (match(TOKEN_PRINT))
    {
        printStatement();
    }
    else if (match(TOKEN_FOR))
    {
        forStatement();
    }
    else if (match(TOKEN_IF))
    {
        ifStatement();
    }
    else if (match(TOKEN_WHILE))
    {
        whileStatement();
    }
    else if (match(TOKEN_LEFT_BRACE))
    {
        beginScope();
        block();
        endScope();
    }
    else if (match(TOKEN_RETURN))
    {
        returnStatement();
    }
    else
    {
        expressionStatement();
    }
}

/// evaluates the value in the expression and then negates it
/// @param canAssign a boolean that determines whether a variable can be assigned (non-relevant)
static void unary(bool canAssign)
{
    TokenType operatorType = parser.previous.type;

    //Compile the operand.
    parsePrecedence(PREC_UNARY);

    //Emit the operator instruction
    switch (operatorType)
    {
    case TOKEN_BANG:
        emitByte(OP_NOT);
        break;
    case TOKEN_MINUS:
        emitByte(OP_NEGATE);
        break;
    default:
        return; //Unreachable.
    }
}

/// call expression to compile what inside the parentheses, and then parse the closing
/// @param canAssign a flag to check if the operator can be assigned (non-relevant)
static void grouping(bool canAssign)
{
    expression();
    consume(TOKEN_RIGHT_PAREN, "Expected ')' after expression.");
}

// the parse rules table with it's designated initializers
ParseRule rules[] = {
    [TOKEN_LEFT_PAREN] = {grouping, call, PREC_CALL},
    [TOKEN_RIGHT_PAREN] = {NULL, NULL, PREC_NONE},
    [TOKEN_LEFT_BRACE] = {NULL, NULL, PREC_NONE},
    [TOKEN_RIGHT_BRACE] = {NULL, NULL, PREC_NONE},
    [TOKEN_COMMA] = {NULL, NULL, PREC_NONE},
    [TOKEN_DOT] = {NULL, dot, PREC_CALL},
    [TOKEN_MINUS] = {unary, binary, PREC_TERM},
    [TOKEN_PLUS] = {NULL, binary, PREC_TERM},
    [TOKEN_SEMICOLON] = {NULL, NULL, PREC_NONE},
    [TOKEN_SLASH] = {NULL, binary, PREC_FACTOR},
    [TOKEN_STAR] = {NULL, binary, PREC_FACTOR},
    [TOKEN_BANG] = {unary, NULL, PREC_NONE},
    [TOKEN_BANG_EQUAL] = {NULL, binary, PREC_COMPARISON},
    [TOKEN_EQUAL] = {NULL, NULL, PREC_NONE},
    [TOKEN_EQUAL_EQUAL] = {NULL, binary, PREC_EQUALITY},
    [TOKEN_GREATER] = {NULL, binary, PREC_COMPARISON},
    [TOKEN_GREATER_EQUAL] = {NULL, binary, PREC_COMPARISON},
    [TOKEN_LESS] = {NULL, binary, PREC_COMPARISON},
    [TOKEN_LESS_EQUAL] = {NULL, binary, PREC_COMPARISON},
    [TOKEN_IDENTIFIER] = {variable, NULL, PREC_NONE},
    [TOKEN_STRING] = {string, NULL, PREC_NONE},
    [TOKEN_NUMBER] = {number, NULL, PREC_NONE},
    [TOKEN_AND] = {NULL, and_, PREC_AND},
    [TOKEN_CLASS] = {NULL, NULL, PREC_NONE},
    [TOKEN_ELSE] = {NULL, NULL, PREC_NONE},
    [TOKEN_FALSE] = {literal, NULL, PREC_NONE},
    [TOKEN_FOR] = {NULL, NULL, PREC_NONE},
    [TOKEN_FUN] = {NULL, NULL, PREC_NONE},
    [TOKEN_IF] = {NULL, NULL, PREC_NONE},
    [TOKEN_NIL] = {literal, NULL, PREC_NONE},
    [TOKEN_OR] = {NULL, or_, PREC_OR},
    [TOKEN_PRINT] = {NULL, NULL, PREC_NONE},
    [TOKEN_RETURN] = {NULL, NULL, PREC_NONE},
    [TOKEN_SUPER] = {super_, NULL, PREC_NONE},
    [TOKEN_THIS] = {this_, NULL, PREC_NONE},
    [TOKEN_TRUE] = {literal, NULL, PREC_NONE},
    [TOKEN_VAR] = {NULL, NULL, PREC_NONE},
    [TOKEN_WHILE] = {NULL, NULL, PREC_NONE},
    [TOKEN_ERROR] = {NULL, NULL, PREC_NONE},
    [TOKEN_EOF] = {NULL, NULL, PREC_NONE},
};

/// a wrapper function that returns the precedence rule for a token type
/// @param type the token type whose precedence needs to be checked
/// @return     returns the parse rule for that token type
static ParseRule* getRule(TokenType type)
{
    return &rules[type];
}

/// compiles the source code
/// @param source the source code that needs to be compiled
/// @param chunk  the bytecode chunk we feed teh tokenized source code to
/// @return       returns whether the code had error in it
ObjFunction* compile(const char* source)
{
    //initializes the scanner with the source string
    initScanner(source);

    //declares a compiler and initializes it
    Compiler compiler;
    initCompiler(&compiler, TYPE_SCRIPT);

    //initializes the parser
    parser.hadError = false;
    parser.panicMode = false;

    advance();
    while (!match(TOKEN_EOF))
    {
        declaration();
    }

    ObjFunction* function = endCompiler();
    return parser.hadError ? NULL : function;
}

/// a function to mark the compiler roots for the garbage collector
void markCompilerRoots()
{
    for (Compiler* compiler = current; compiler != NULL; compiler = compiler->enclosing)
    {
        markObject((Obj*)compiler->function);
    }
}