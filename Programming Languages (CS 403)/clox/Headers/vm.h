#ifndef clox_vm_h
#define clox_vm_h

#include "chunk.h"
#include "value.h"

#define STACK_CAP 256

typedef struct {
    Chunk* chunk;
    uint8_t* ip; // Stands for instruction pointer
    Value stack[STACK_CAP];
    Value* stackTop;
} VM;

// Represents the state of the interpreter after it has interpreted something.
typedef enum {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
} InterpretResult;

// VM Operations
void initVM();
void freeVM();
InterpretResult interpret(Chunk* chunk);

// Stack Operations
void push(Value value);
Value pop();

#endif