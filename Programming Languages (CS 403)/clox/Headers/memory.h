#ifndef clox_memory_h
#define clox_memory_h

#include "common.h"

#define DOUBLE_CAPACITY(capacity) ((capacity) < 8 ? 8 : (capacity) * 2)

// Handles getting array size and casting result back to the specified type
#define DOUBLE_ARRAY(type, pointer, oldCount, newCount) \
    (type*)reallocate(pointer, sizeof(type) * (oldCount), sizeof(type) * (newCount))

// Wrapper function for the reallocation 0
#define FREE_ARRAY(type, pointer, oldCount) reallocate(pointer, sizeof(type) * (oldCount), 0)

void* reallocate(void* pointer, size_t oldSize, size_t newSize);

#endif