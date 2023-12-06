#include "../Headers/common.h"
#include "../Headers/chunk.h"
#include "../Headers/debug.h"

// Main program for testing and debugging
int main(int argc, char **argv) {
    Chunk chunk;
    initChunk(&chunk);

    int constant = addConstant(&chunk, 1.2);
    addChunkInstruction(&chunk, CONSTANT_OP, 123);
    addChunkInstruction(&chunk, constant, 123);

    addChunkInstruction(&chunk, RETURN_OP, 123);

    disassembleChunk(&chunk, "test chunk");

    freeChunk(&chunk);    
    return 0;
}