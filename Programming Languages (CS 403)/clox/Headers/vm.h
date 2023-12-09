#ifndef clox_vm_h
#define clox_vm_h

#include "object.h"
#include "table.h"
#include "value.h"

#define FRAMES_CAP 64
#define STACK_CAP (FRAMES_CAP * UINT8_COUNT)

// represents the call frame for function calls
typedef struct {
    ObjFunction* function;
    uint8_t* ip; // Stands for instruction pointer
    Value* slots;
} CallFrame;

typedef struct {
    CallFrame frames[FRAMES_CAP];
    int frameSize; 
    Value stack[STACK_CAP];
    Value* stackTop;
    Table globals;
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