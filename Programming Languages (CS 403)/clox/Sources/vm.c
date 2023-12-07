#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "../Headers/common.h"
#include "../Headers/compiler.h"
#include "../Headers/debug.h"
#include "../Headers/object.h"
#include "../Headers/memory.h"
#include "../Headers/vm.h"

VM vm;

// Resets the stack by setting the top pointer back to the first index of the stack (0 index)
static void resetStack() {
    vm.stackTop = vm.stack;
}

// Reports a runtime error, ... represents a variable amount of arguments
static void runtimeError(const char* format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    fputs("\n", stderr);

    size_t instruction = vm.ip - vm.chunk->instructions- 1;
    int line = vm.chunk->lineNumbers[instruction];
    fprintf(stderr, "[line %d] in script\n", line);
    resetStack();
}

// Initializes the stack in the VM to be empty and the objects pointer is empty for now.
void initVM() {
    resetStack();
    vm.objects = NULL;
}

// Cleans up any leftover objects by freeing them.
// Even though the garbage collector may free some objects, it won't always free all of the objects, hence this function.
void freeVM() {
    freeObjects();
}

// Adds a value to the top of the VM's stack
void push(Value value) {
    *vm.stackTop = value;
    vm.stackTop++;
}

// Returns the value at the top of the stack and marks its position for re-use.
Value pop() {
    vm.stackTop--;
    return *vm.stackTop;
}

// Returns the value in the stack given an offset from the top.
static Value peek(int distanceFromTop) {
    return vm.stackTop[-1 - distanceFromTop];
}

// Determines what is treated as false
static bool isFalsey(Value value) {
    return IS_NIL(value) || (IS_BOOL(value) && !AS_BOOL(value));
}

// Concatenates two strings together
static void concatenate() {
    ObjString* b = AS_STRING(pop());
    ObjString* a = AS_STRING(pop());

    int size = a->size + b->size;
    char* chars = ALLOCATE(char, size + 1);

    // Copying the values over into the new string and attaching a null terminator at the end.
    memcpy(chars, a->string, a->size);
    memcpy(chars + a->size, b->string, b->size);
    chars[size] = '\0';

    ObjString* result = takeString(chars, size);
    push(OBJ_V(result));
}

// Executes the current chunk of the VM.
static InterpretResult run() {
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
// BINARY_OP checks to ensure both operants are numbers before attempting to perform the operations.
#define BINARY_OP(valueType, op) \
    do { \
        if (!IS_NUMBER(peek(0)) || !IS_NUMBER(peek(1))) { \
            runtimeError("Operands must be numbers."); \
            return INTERPRET_RUNTIME_ERROR; \
        } \
        double b = AS_NUMBER(pop()); \
        double a = AS_NUMBER(pop()); \
        push(valueType(a op b)); \
    } while (false)

    while (true) { // replaces for (;;)
        // If the debug flag is defined, enter debug mode, which prints the "insides" of the VM and the chunk being executed.
        #ifdef DEBUG_TRACE_EXECUTION 
            printf("Val Stack: ");
            for (Value* slot = vm.stack; slot < vm.stackTop; slot++) {
                printf("[ ");
                printValue(*slot);
                printf(" ]");
            }
            printf("\n\n");
            disassembleInstruction(vm.chunk,(int)(vm.ip - vm.chunk->instructions));
        #endif

        // Beginning of the code actually being ran.
        uint8_t instruction = READ_BYTE();
        if (instruction == CONSTANT_OP) {
            Value constant = READ_CONSTANT();
            push(constant);
        } 
        else if (instruction == NIL_OP) {
            push(NIL_V);
        }
        else if (instruction == TRUE_OP) {
            push(BOOL_V(true));
        }
        else if (instruction == FALSE_OP) {
            push(BOOL_V(false));
        }
        else if (instruction == EQUAL_OP) {
            Value b = pop();
            Value a = pop();
            push(BOOL_V(areValuesEqual(a, b)));
        }
        else if (instruction == GREATER_OP) {
            BINARY_OP(BOOL_V, >);
        }
        else if (instruction == LESS_OP) {
            BINARY_OP(BOOL_V, <);
        }
        else if (instruction == ADD_OP) {
            if (IS_STRING(peek(0)) && IS_STRING(peek(1))) { // Both operands are a string
                concatenate();
            } 
            else if (IS_NUMBER(peek(0)) && IS_NUMBER(peek(1))) { // Both operands are a number
                double b = AS_NUMBER(pop());
                double a = AS_NUMBER(pop());
                push(NUMBER_V(a + b));
            } 
            else { // Mismatch in operands, one is a string and the other is a number
                runtimeError("Operands must be two numbers or two strings.");
                return INTERPRET_RUNTIME_ERROR;
            }
        } 
        else if (instruction == SUBTRACT_OP) {
            BINARY_OP(NUMBER_V, -);
        } 
        else if (instruction == MULTIPLY_OP) {
            BINARY_OP(NUMBER_V, *);
        } 
        else if (instruction == DIVIDE_OP) {
            BINARY_OP(NUMBER_V, /);
        }
        else if (instruction == NOT_OP) {
            push(BOOL_V(isFalsey(pop())));
        } 
        else if (instruction == NEGATE_OP) {
            // Ensures the top of the stack is a number before trying to negate it.
            if (!IS_NUMBER(peek(0))) { 
                runtimeError("Operand must be a number.");
                return INTERPRET_RUNTIME_ERROR;
            }
            push(NUMBER_V(-AS_NUMBER(pop())));
        } 
        else if (instruction == RETURN_OP) {
            // 2 lines below will be changed when functions come into play
            printValue(pop());
            printf("\n");
            return INTERPRET_OK;
        }
    }

#undef BINARY_OP
#undef READ_CONSTANT
#undef READ_BYTE
}

// Interprets sourceCode via compilation and indicates a successful return.
// Also creates a new chunk for the sourceCode tokens to be loaded into.
InterpretResult interpret(const char* sourceCode) {
    Chunk chunk;
    initChunk(&chunk);

    if (!compile(sourceCode, &chunk)) {
        freeChunk(&chunk);
        return INTERPRET_COMPILE_ERROR;
    }

    vm.chunk = &chunk;
    vm.ip = vm.chunk->instructions;

    InterpretResult result = run();

    freeChunk(&chunk);
    return result;
}