#include <stdio.h>
#include "common.h"
#include "compiler.h"
#include "scanner.h"

void compile(const char *source) {
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