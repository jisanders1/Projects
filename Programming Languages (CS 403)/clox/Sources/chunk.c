#include <stdlib.h>
#include "../Headers/chunk.h"
#include "../Headers/memory.h"

// Initializes an empty chunk
void initChunk(Chunk* chunk) {
    chunk->size = 0;
    chunk->cap = 0;
    chunk->instructions = NULL;
    chunk->lineNumbers = NULL;
    initValueArray(&chunk->constants);
}

// Deallocates a chunk's memory and then reinitializes it to the empty state
void freeChunk(Chunk* chunk) {
    FREE_ARRAY(uint8_t, chunk->instructions, chunk->cap);
    FREE_ARRAY(int, chunk->lineNumbers, chunk->cap);
    freeValueArray(&chunk->constants);
    initChunk(chunk);
}

// Adds an instruction in the form of a byte to the chunk's instructions with a specified line number.
void addChunkInstruction(Chunk* chunk, uint8_t byte, int line) {
    // Resizing both the instructions and line numbers array.
    if (chunk->cap < chunk->size + 1) {
        int oldCap = chunk->cap;
        chunk->cap = DOUBLE_CAPACITY(oldCap);
        chunk->instructions = DOUBLE_ARRAY(uint8_t, chunk->instructions, oldCap, chunk->cap);
        chunk->lineNumbers = DOUBLE_ARRAY(int, chunk->lineNumbers, oldCap, chunk->cap);
    }

    chunk->instructions[chunk->size] = byte;
    chunk->lineNumbers[chunk->size] = line;
    chunk->size++;
}

// Adds a constant to the constants array inside of the chunk.
int addConstant(Chunk* chunk, Value value) {
    addValueArray(&chunk->constants, value);
    return chunk->constants.size - 1;
}