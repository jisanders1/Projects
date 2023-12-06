#ifndef clox_chunk_h
#define clox_chunk_h

#include "common.h"
#include "value.h"

// Controls what kind of operation we are doing.
typedef enum {
    CONSTANT_OP,
    RETURN_OP,
} OpCode;

// Holds additional data along with the instructions
typedef struct {
    int size;
    int cap;
    uint8_t* instructions;
    int* lineNumbers;
    ValueArray constants;
} Chunk;

// Functions for Chuncks
void initChunk(Chunk* chunk);
void addChunkInstruction(Chunk* chunk, uint8_t byte, int line);
void freeChunk(Chunk* chunk);

// Functions for Constants
int addConstant(Chunk* chunk, Value value);

#endif