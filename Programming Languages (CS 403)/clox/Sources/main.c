#include "../Headers/common.h"
#include "../Headers/chunk.h"
#include "../Headers/debug.h"
#include "../Headers/vm.h"

// Main program for testing and debugging
int main(int argc, char **argv) {
    initVM();

    Chunk chunk;
    initChunk(&chunk);

    // Lines 14-28 represent the arithmetic expression: -((1.2 + 3.4) / 5.6)
    // Should output -0.821429 as the last line of output
    int constant = addConstant(&chunk, 1.2); // adds a constant to the values list of the chunk
    addChunkInstruction(&chunk, CONSTANT_OP, 123); // These lines indicate a constant operation with the value supplied by constant.
    addChunkInstruction(&chunk, constant, 123);

    constant = addConstant(&chunk, 3.4);
    addChunkInstruction(&chunk, CONSTANT_OP, 123);
    addChunkInstruction(&chunk, constant, 123);

    addChunkInstruction(&chunk, ADD_OP, 123);

    constant = addConstant(&chunk, 5.6);
    addChunkInstruction(&chunk, CONSTANT_OP, 123);
    addChunkInstruction(&chunk, constant, 123);

    addChunkInstruction(&chunk, DIVIDE_OP, 123);

    addChunkInstruction(&chunk, NEGATE_OP, 123); // Negates the constant.

    addChunkInstruction(&chunk, RETURN_OP, 123); // Indicates a return

    disassembleChunk(&chunk, "test chunk"); // Prints the chunk we just made.
    interpret(&chunk); // Interprets the chunk

    freeVM();
    freeChunk(&chunk);    
    return 0;
}