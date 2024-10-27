#include <stdio.h>
#include <string.h>
#include "common.h"
#include "scanner.h"

#include <ctype.h>

typedef struct {
    const char* start;
    const char* current;
    int line;
}Scanner;

Scanner scanner;

/// the scanner gets a source code file and initializes the scanner to parse through it
/// @param source the source code
void initScanner(const char* source) {
    scanner.start = source;
    scanner.current = source;
    scanner.line = 1;
}

/// the function determines whether the character given is a letter or an underscore to determine if it's an identifier
/// @param c - a char that was scanned from the source code
/// @return TRUE: the character is a letter or an underscore, FALSE: else
static bool isAlpha(char c) {
    return (c >= 'a' && c <='z') || (c >= 'A' && c <= 'Z') || c == '_';
}

/// the function gets a character and returns whether it's a digit or not
/// @param c the current character in the lexeme
/// @return TRUE - if c is a digit, FALSE - if C isn't a number
static bool isDigit(char c) {
    return c >= '0' && c <= '9';
}

/// checks if the current char is a null byte
/// @return True if null byte, false if not
static bool isAtEnd() {
    return *scanner.current == '\0';
}

/// the function returns the next character
/// @return the next character
static char advance() {
    scanner.current++;
    return scanner.current[-1];
}

/// the function returns the current char without consuming it
/// @return the next char in the scanner
static char peek() {
    return *scanner.current;
}

/// the function returns the next current after the current one but not consuming it
/// @return the next char in the scanner
static char peekNext() {
    //checks if we are at the end of the scanner
    if(isAtEnd()) return '\0';
    return scanner.current[1];
}

/// the function receives a char and checks if the current char matches it
/// @param expected
/// @return true if there is a match with expected, false if not
static bool match(char expected) {
    //checks if we reached the end of the string
    if (isAtEnd()) return false;
    //checks if they not the same
    if (*scanner.current != expected) return false;
    scanner.current++;
    return true;
}

/// the function receives a token type and returns a new token
/// @param type
/// @return a new token that represents the type that was received
static Token makeToken(TokenType type) {
    Token token;
    token.type = type;
    token.start = scanner.current;
    token.length = (int)(scanner.current - scanner.start);
    token.line = scanner.line;
    return token;
}

/// the function receives an error message and returns an error token that includes the message
/// @param message
/// @return an errorToken
static Token errorToken(const char* message) {
    Token token;
    token.start = message;
    token.length = (int)(strlen(message));
    token.line = scanner.line;
    return token;
}

/// a mini scanner to skip all the whitespaces and new lines with the appropriate updates
static void skipWhiteSpaces() {

    // a loop to churn through the whitespaces between lexemes
    for(;;) {
        //looks at the current char without consuming it
        char c = peek();

        //check whether it is a white space or a new line and deals with it appropriately
        switch (c) {
            case ' ':
            case '\r':
            case '\t':
                advance();
                break;
            case '\n':
                scanner.line++;
                advance();
                break;
            case '/':
                if(peekNext() == '/') {
                    //runs until the end of the comment line
                    while(peek() != '\n' && !isAtEnd()) advance();
                }
                else
                    return;
                break;
            default:
                return;
        }
    }
}
/// the function gets a lexeme from the scanner and checks if it's a keyword or an identifier and returns the approproate token type
/// @param start  - the start of the word
/// @param length - the length of the keyword we want to compare the scanned word to
/// @param rest - the rest of the keyword
/// @param type - the type of the keyword, represented with it's TokenType
/// @return returns the keyword token type or an identifier token type
static TokenType checkKeyword(int start, int length, const char* rest, TokenType type) {
    if(scanner.current - scanner.start == start + length && memcmp(scanner.start + start, rest, length) == 0) {
        return type;
    }
    return TOKEN_IDENTIFIER;
}


/// The function returns a token type, if it's a saved keyword
/// then a specific token will be returned. if it's not so an identifier token will be returned
/// @return a token type for a saved keyword or TOKEN_IDENTIFIER for ones that are not
static TokenType identifierType() {
    switch(scanner.current[0]) {
        case 'a': return checkKeyword(1,2,"nd",TOKEN_AND);
        case 'c': return checkKeyword(1,4,"lass",TOKEN_CLASS);
        case 'e': return checkKeyword(1,3,"lse",TOKEN_ELSE);
        case 'f':
            if(scanner.current - scanner.start > 1) {
                switch(scanner.current[1]) {
                    case 'a': return checkKeyword(2,3,"lse",TOKEN_FALSE);
                    case 'o': return checkKeyword(2,1,"r",TOKEN_FOR);
                    case 'u': return checkKeyword(2,1,"n",TOKEN_FUN);
                }
            }
            break;
        case 'i': return checkKeyword(1,1,"f",TOKEN_IF);
        case 'n': return checkKeyword(1,2,"il",TOKEN_NIL);
        case 'o': return checkKeyword(1,1,"r",TOKEN_OR);
        case 'p': return checkKeyword(1,4,"rint",TOKEN_PRINT);
        case 'r': return checkKeyword(1,5,"eturn",TOKEN_RETURN);
        case 's': return checkKeyword(1,4,"uper",TOKEN_SUPER);
        case 't':
            if (scanner.current - scanner.start > 1) {
                switch(scanner.current[1]) {
                    case 'h': return checkKeyword(2,2,"is",TOKEN_THIS);
                    case 'r': return checkKeyword(2,2,"ue",TOKEN_TRUE);
                }
            }
            break;
        case 'v': return checkKeyword(1,2,"ar",TOKEN_VAR);
        case 'w': return checkKeyword(1,4,"hile",TOKEN_WHILE);
    }
    return TOKEN_IDENTIFIER;
}

/// The function creates a token for identifier that includes numbers,letters and underscore
/// @return a new token for identifier
static Token identifier() {
    while(isAlpha(peek()) || isDigit(peek()))advance();
    return makeToken(identifierType());
}

/// the function makes a new token for the next number that appears
/// @return a new token for number
static Token number() {
    while (isDigit(peek())) advance();

    //Look for a fractional part.
    if (peek() == '.' && isDigit(peekNext())) {
        //consume the ".".
        advance();

        //runs ahead as long as the next character is a digit
        while (isDigit(peek())) advance();
    }

    return makeToken(TOKEN_NUMBER);
}

/// a function to handle the processing of strings
/// @return returns a string token at the end of the string scanning
static Token string() {

    //while the next character isn't a closing quote and there are still characters to scan through, advance
    while(peek() != '"' && !isAtEnd()) {
        if(peek() == '\n') scanner.line++;
        advance();
    }

    //if reached the end of the source code without reaching the closing quote, return an error token
    if(isAtEnd()) return errorToken("Unterminated string");

    //the closing quote
    advance();
    return makeToken(TOKEN_STRING);
}

/// scans the current token
/// @return returns a token that represent the state of the current char in the scanner.
Token scanToken() {
    //first skips all the white spaces before the start of the lexeme
    skipWhiteSpaces();
    //resets the scanner pointers to the start of the new lexeme
    scanner.start = scanner.current;

    //returns an EOF token if reached the end of the source code
    if(isAtEnd()) return makeToken(TOKEN_EOF);

    //saves the first character in the lexeme to c
    char c = advance();

    //if c is a number, turn it into one
    if(isDigit(c)) return number();

    //switches the "special" characters that separate between lexemes and aren't whitespaces
    switch(c) {
        case '(': return makeToken(TOKEN_LEFT_PAREN);
        case ')': return makeToken(TOKEN_RIGHT_PAREN);
        case '{': return makeToken(TOKEN_LEFT_BRACE);
        case '}': return makeToken(TOKEN_RIGHT_BRACE);
        case ';': return makeToken(TOKEN_SEMICOLON);
        case '.': return makeToken(TOKEN_DOT);
        case '-': return makeToken(TOKEN_MINUS);
        case '+': return makeToken(TOKEN_PLUS);
        case '/': return makeToken(TOKEN_SLASH);
        case '*': return makeToken(TOKEN_STAR);
        case ',': return makeToken(TOKEN_COMMA);
        case '!': return makeToken(match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
        case '=': return makeToken(match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
        case '<': return makeToken(match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
        case '>': return makeToken(match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
        case '"': return string();
    }

    //returns an error token in case a given character isn't recognized
    return errorToken("Unexpected character.");
}