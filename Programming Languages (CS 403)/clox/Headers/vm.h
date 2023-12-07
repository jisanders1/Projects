#ifndef clox_vm_h
#define clox_vm_h

#include "chunk.h"
#include "table.h"
#include "value.h"

#define STACK_CAP 256

typedef struct {
    Chunk* chunk;
    uint8_t* ip; // Stands for instruction pointer
    Value stack[STACK_CAP];
    Value* stackTop;
    Table strings;
    Obj* objects;
} VM;

// Represents the state of the interpreter after it has interpreted something.
typedef enum {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
} InterpretResult;

extern VM vm; // exposes the global VM so it can be seen by other modules.

// VM Operations
void initVM();
void freeVM();
InterpretResult interpret(const char* sourceCode);

// Stack Operations
void push(Value value);
Value pop();

#endif