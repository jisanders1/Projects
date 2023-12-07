#include <stdlib.h>
#include "../Headers/memory.h"
#include "../Headers/vm.h"

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

static void freeObject(Obj* object) {
    switch (object->type) {
        case STRING_OBJ: {
            ObjString* string = (ObjString*)object;
            FREE_ARRAY(char, string->string, string->size + 1);
            FREE(ObjString, object);
            break;
        }
    }
}

// Base call for cleanup; iterates through object list, freeing any memory used by each object.
void freeObjects() {
    Obj* object = vm.objects;
    while (object != NULL) {
        Obj* next = object->next;
        freeObject(object);
        object = next;
    }
}