#include <stdio.h>
#include "../Headers/common.h"
#include "../Headers/compiler.h"
#include "../Headers/scanner.h"

void compile(const char* sourceCode) {
    initScanner(sourceCode);
    int line = -1;

    // Below currently does not truly compile anything, It simply prints the token.
    while (true) {
        Token token = scanToken();
        switch (token.lineNumber != line) {
            case true:
                printf("%4d ", token.lineNumber);
                line = token.lineNumber;
                break;
            default:
                printf("   | ");
        }
        
        printf("%2d '%.*s'\n", token.type, token.size, token.sourcePointer); 

        if (token.type == TOKEN_EOF) break;
    }
}