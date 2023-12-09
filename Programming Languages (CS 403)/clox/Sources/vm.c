#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../Headers/common.h"
#include "../Headers/compiler.h"
#include "../Headers/debug.h"
#include "../Headers/object.h"
#include "../Headers/memory.h"
#include "../Headers/vm.h"

// Represents the virtual machine, which executes code
VM vm;

static Value clockNative(int argCount, Value* args) {
    return NUMBER_V((double)clock() / CLOCKS_PER_SEC);
}

// Resets the stack by setting the top pointer back to the first index of the stack (0 index) and the size to 0
static void resetStack() {
    vm.stackTop = vm.stack;
    vm.frameSize = 0;
}

// Reports a runtime error, ... represents a variable amount of arguments
static void runtimeError(const char* format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    fputs("\n", stderr);

    // Generates trace so the user can follow the path of errors.
    for (int i = vm.frameSize - 1; i >= 0; i--) {
        CallFrame* frame = &vm.frames[i];
        ObjFunction* function = frame->function;
        size_t instruction = frame->ip - function->chunk.instructions - 1;
        fprintf(stderr, "[line %d] in ", function->chunk.lineNumbers[instruction]);

        if (function->name == NULL) {
            fprintf(stderr, "script\n");
        } else {
            fprintf(stderr, "%s()\n", function->name->string);
        }
    }

    resetStack();
}

// Defines native functions in the Lox program
static void defineNative(const char* name, NativeFn function) {
    push(OBJ_V(copyString(name, (int)strlen(name))));
    push(OBJ_V(initNewNative(function)));
    insertPair(&vm.globals, AS_STRING(vm.stack[0]), vm.stack[1]);
    pop();
    pop();
}

// Initializes the stack in the VM to be empty and the objects pointer is empty for now.
void initVM() {
    resetStack(); // Inisializes value stack
    vm.objects = NULL; // Empty object list

    // Initializing tables for global and string values.
    initTable(&vm.globals);
    initTable(&vm.strings);

    // Initializing native functions
    defineNative("clock", clockNative);
}

// Cleans up any leftover objects by freeing them.
// Even though the garbage collector may free some objects, it won't always free all of the objects, hence this function.
void freeVM() {
    freeTable(&vm.globals);
    freeTable(&vm.strings);
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

// Actual function call starts here
static bool call(ObjFunction* function, int argCount) {
    // Reports error if number of arguments does not match function declaration parameters
    if (argCount != function->arity) { 
        runtimeError("Expected %d arguments but got %d.", function->arity, argCount);
        return false;
    }

    // Reports error if there are too many active call frames.
    if (vm.frameSize == FRAMES_CAP) {
        runtimeError("Stack overflow.");
        return false;
    }
    
    CallFrame* frame = &vm.frames[vm.frameSize++];
    frame->function = function;
    frame->ip = function->chunk.instructions;
    frame->slots = vm.stackTop - argCount - 1;
    return true;
}

// Does work for executing the function call
static bool callValue(Value callee, int argCount) {
    if (IS_OBJ(callee)) {
        switch (OBJ_TYPE(callee)) {
            case FUNCTION_OBJ: 
                return call(AS_FUNCTION(callee), argCount);
            case NATIVE_OBJ: {
                NativeFn native = AS_NATIVE(callee);
                Value result = native(argCount, vm.stackTop - argCount);
                vm.stackTop -= argCount + 1;
                push(result);
                return true;
            }
            default:
                break; // Non-callable object type.
        }
    }
    runtimeError("Can only call functions and classes.");
    return false;
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
CallFrame* frame = &vm.frames[vm.frameSize - 1];

#define READ_BYTE() (*frame->ip++)
#define READ_SHORT() (frame->ip += 2, (uint16_t)((frame->ip[-2] << 8) | frame->ip[-1]))
#define READ_CONSTANT() (frame->function->chunk.constants.values[READ_BYTE()])
#define READ_STRING() AS_STRING(READ_CONSTANT())
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
            disassembleInstruction(&frame->function->chunk, (int)(frame->ip - frame->function->chunk.instructions));
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
        else if (instruction == POP_OP) {
            pop();
        }
        else if (instruction == GET_LOCAL_OP) {
            uint8_t slot = READ_BYTE();
            push(frame->slots[slot]); 
        }
        else if (instruction == GET_GLOBAL_OP) {
            ObjString* name = READ_STRING();
            Value value;

            if (!getValue(&vm.globals, name, &value)) { // Reports an error if a reference is made to an undefined variable.
                runtimeError("Undefined variable '%s'.", name->string);
                return INTERPRET_RUNTIME_ERROR;
            }

            push(value);
        }
        else if (instruction == DEFINE_GLOBAL_OP) {
            ObjString* name = READ_STRING();
            insertPair(&vm.globals, name, peek(0));
            pop();
        }
        else if (instruction == SET_LOCAL_OP) {
            uint8_t slot = READ_BYTE();
            frame->slots[slot] = peek(0);
        }
        else if (instruction == SET_GLOBAL_OP) {
            ObjString* name = READ_STRING();
            if (insertPair(&vm.globals, name, peek(0))) {
                deletePair(&vm.globals, name); 
                runtimeError("Undefined variable '%s'.", name->string);
                return INTERPRET_RUNTIME_ERROR;
            }
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
        else if (instruction == PRINT_OP) {
            printValue(pop());
            printf("\n");
        }
        else if (instruction == JUMP_OP) {
            uint16_t offset = READ_SHORT();
            frame->ip += offset;
        }
        else if (instruction == JUMP_IF_FALSE_OP) {
            uint16_t offset = READ_SHORT();
            if (isFalsey(peek(0))) frame->ip += offset;
        }
        else if (instruction == LOOP_OP) {
            uint16_t offset = READ_SHORT();
            frame->ip -= offset;
        }
        else if (instruction == CALL_OP) {
            int argCount = READ_BYTE();
            if (!callValue(peek(argCount), argCount)) {
                return INTERPRET_RUNTIME_ERROR;
            }
            frame = &vm.frames[vm.frameSize - 1];
        }
        else if (instruction == RETURN_OP) {
            Value result = pop();
            vm.frameSize--;

            // Successful case
            if (vm.frameSize == 0) {
                pop();
                return INTERPRET_OK;
            }

            vm.stackTop = frame->slots;
            push(result);
            frame = &vm.frames[vm.frameSize - 1];
        }
    }

#undef BINARY_OP
#undef READ_STRING
#undef READ_SHORT
#undef READ_CONSTANT
#undef READ_BYTE
}

// Interprets sourceCode via compilation and indicates a successful return.
InterpretResult interpret(const char* sourceCode) {
    ObjFunction* function = compile(sourceCode);
    if (function == NULL) return INTERPRET_COMPILE_ERROR;

    push(OBJ_V(function));
    call(function, 0);

    return run();
}