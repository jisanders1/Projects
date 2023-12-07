#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Headers/common.h"
#include "../Headers/chunk.h"
#include "../Headers/debug.h"
#include "../Headers/vm.h"

// Represents the interactive program in the terminal
static void repl() {
    char line[1024]; // limits amount of characters to 1,024 per line of code for the REPL
    while (true) {
        printf("> ");

        if (!fgets(line, sizeof(line), stdin)) {
            printf("\n");
            break;
        }

        interpret(line);
    }
}

// Represents reading bytes from a file.
// Throws errors in he event of a non-existent file/file path, running out of space for the buffer, 
// and the inability to access all of the file.
static char* readFile(const char* path) {
    FILE* file = fopen(path, "rb");
    if (file == NULL) {
        fprintf(stderr, "Could not open file \"%s\".\n", path);
        exit(74);
    }

    fseek(file, 0L, SEEK_END);
    size_t fileSize = ftell(file);
    rewind(file);

    char* buffer = (char*)malloc(fileSize + 1);
    if (buffer == NULL) {
        fprintf(stderr, "Not enough memory to read \"%s\".\n", path);
        exit(74);
    }

    size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);
    if (bytesRead < fileSize) {
        fprintf(stderr, "Could not read file \"%s\".\n", path);
        exit(74);
    }
    buffer[bytesRead] = '\0';

    fclose(file);
    return buffer;
}

static void runFile(const char* path) {
    char* source = readFile(path);
    InterpretResult result = interpret(source);
    free(source); 

    if (result == INTERPRET_COMPILE_ERROR) exit(65);
    if (result == INTERPRET_RUNTIME_ERROR) exit(70);
}

// Main program for testing and debugging
int main(int argc, char **argv) {
    initVM();

    if (argc == 1) { // Run REPL if there is only one argument
        repl();
    } 
    else if (argc == 2) { // Run a file
        runFile(argv[1]);
    } else { // Invalid number of arguments
        fprintf(stderr, "Usage: clox [path]\n");
        exit(64);
    }

    freeVM();   
    return 0;
}