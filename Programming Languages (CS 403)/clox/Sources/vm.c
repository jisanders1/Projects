#include <stdio.h>
#include "../Headers/common.h"
#include "../Headers/debug.h"
#include "../Headers/vm.h"

VM vm;

// Resets the stack by setting the top pointer back to the first index of the stack (0 index)
static void resetStack() {
    vm.stackTop = vm.stack;
}

// Initializes the stack in the VM to be empty
void initVM() {
    resetStack();
}

void freeVM() {

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

// Executes the current chunk of the VM.
static InterpretResult run() {
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
#define BINARY_OP(op) \
    do { \
        double b = pop(); \
        double a = pop(); \
        push(a op b); \
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
        else if (instruction == ADD_OP) {
            BINARY_OP(+);
        } 
        else if (instruction == SUBTRACT_OP) {
            BINARY_OP(-);
        } 
        else if (instruction == MULTIPLY_OP) {
            BINARY_OP(*);
        } 
        else if (instruction == DIVIDE_OP) {
            BINARY_OP(/);
        } 
        else if (instruction == NEGATE_OP) {
            push(-pop());
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

InterpretResult interpret(Chunk* chunk) {
    vm.chunk = chunk;
    vm.ip = vm.chunk->instructions;
    return run();
}