#include <stdio.h>
#include "../Headers/debug.h"
#include "../Headers/value.h"

// debug.c's purpose is 
// Entry point to printing the chunks, goes through the instructions byte by byte to print them.
void disassembleChunk(Chunk* chunk, const char* name) {
    printf("== %s ==\n", name);

    for (int position = 0; position < chunk->size;) {
        position = disassembleInstruction(chunk, position);
    }
}

// Dissassemble instructions for specific types of OPCODES
static int constantInstruction(const char* name, Chunk* chunk, int position) {
    uint8_t constant = chunk->instructions[position + 1];
    printf("%-16s %4d '", name, constant);
    printValue(chunk->constants.values[constant]);
    printf("'\n");
    return position + 2;
}

// Prints a sumple instruction, which is just one byte and usually the name.
static int simpleInstruction(const char* name, int position) {
    printf("%s\n", name);
    return position + 1;
}

// Calls a different helper function depending on what an instruction's opcode.
// Additionally adds on a line number if one was specified.
int disassembleInstruction(Chunk* chunk, int position) {
    printf("%04d ", position);
    bool printLine = position > 0 && chunk->lineNumbers[position] == chunk->lineNumbers[position - 1];

    // Handles including a line number or not.
    switch (printLine) {
        case true:
            printf("   | ");
            break;
        default:
            printf("%4d ", chunk->lineNumbers[position]);
    }

    uint8_t instruction = chunk->instructions[position];
    // Handles what to print based on which opcode the instruction contains.
    if (instruction == CONSTANT_OP ) {
        return constantInstruction("CONSTANT_OP", chunk, position);
    }
    else if (instruction == NIL_OP) {
        return simpleInstruction("NIL_OP", position);
    }
    else if (instruction == TRUE_OP) {
        return simpleInstruction("TRUE_OP", position);
    }
    else if (instruction == FALSE_OP) {
        return simpleInstruction("FALSE_OP", position);
    }
    else if (instruction == EQUAL_OP) {
        return simpleInstruction("EQUAL_OP", position);
    }
    else if (instruction == GREATER_OP) {
        return simpleInstruction("GREATER_OP", position);
    }
    else if (instruction == LESS_OP) {
        return simpleInstruction("LESS_OP", position);
    }
    else if (instruction == ADD_OP) {
        return simpleInstruction("ADD_OP", position);
    } 
    else if (instruction == SUBTRACT_OP) {
        return simpleInstruction("SUBTRACT_OP", position);
    } 
    else if (instruction == MULTIPLY_OP) {
        return simpleInstruction("MULTIPLY_OP", position);
    } 
    else if (instruction == DIVIDE_OP) {
        return simpleInstruction("DIVIDE_OP", position);
    }
    else if (instruction == NOT_OP) {
        return simpleInstruction("NOT_OP", position);
    }
    else if (instruction == NEGATE_OP) {
        return simpleInstruction("NEGATE_OP", position);
    } 
    else if (instruction == RETURN_OP) {
        return simpleInstruction("RETURN_OP", position);
    } else {
        printf("Unknown opcode %d\n", instruction);
        return position + 1;
    }
}