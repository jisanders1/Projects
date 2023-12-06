#include <stdlib.h>
#include "../Headers/memory.h"

// Handles resizing the array that pointer points to.
void* reallocate(void* pointer, size_t oldSize, size_t newSize) {
    // Handles the case where the new size is 0
    if (newSize == 0) {
        free(pointer);
        return NULL;
    }
    
    // Handles other 3 cases: sizing up, sizing down, and allocating a new block.
    void* result = realloc(pointer, newSize); 

    // realloc can fail, this catches when it fails.
    if (result == NULL) exit(1); 
    return result;
}