#ifndef clox_chunk_h
#define clox_chunk_h

#include "common.h"
#include "value.h"

// Contains the operation code name that control what kind of operation we are doing.
typedef enum {
    CONSTANT_OP,
    NIL_OP,
    TRUE_OP,
    FALSE_OP,
    EQUAL_OP,
    GREATER_OP,
    LESS_OP,
    ADD_OP,
    SUBTRACT_OP,
    MULTIPLY_OP,
    DIVIDE_OP,
    NOT_OP,
    NEGATE_OP,
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