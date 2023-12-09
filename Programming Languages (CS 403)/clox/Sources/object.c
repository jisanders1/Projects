#include <stdio.h>
#include <string.h>
#include "../Headers/memory.h"
#include "../Headers/object.h"
#include "../Headers/table.h"
#include "../Headers/value.h"
#include "../Headers/vm.h"

#define ALLOCATE_OBJ(type, objectType) (type*)allocateObject(sizeof(type), objectType)

// Allocates the object, sort of like allocating a class' superclass
static Obj* allocateObject(size_t size, ObjType type) {
    Obj* object = (Obj*)reallocate(NULL, 0, size);
    object->type = type;

    // Adding the object to the VM's object list
    object->next = vm.objects;
    vm.objects = object;
    return object;
}

// Initializes a new function with no name, no arity, and initializes the chunk inside of the function.
ObjFunction* initNewFunction() {
    ObjFunction* function = ALLOCATE_OBJ(ObjFunction, FUNCTION_OBJ);
    function->arity = 0;
    function->name = NULL;
    initChunk(&function->chunk);
    return function;
}

// Initializes a new native function given a function argument.
ObjNative* initNewNative(NativeFn function) {
    ObjNative* native = ALLOCATE_OBJ(ObjNative, NATIVE_OBJ);
    native->function = function;
    return native;
}

// Allocates a string object
static ObjString* allocateString(char* chars, int length, uint32_t hash) {
    ObjString* string = ALLOCATE_OBJ(ObjString, STRING_OBJ);
    string->size = length;
    string->string = chars;
    string->hash = hash;
    insertPair(&vm.strings, string, NIL_V);
    return string;
}

// Generates a hash value for a string key by iterating through it's letters and applying a function (FNV-1a)
static uint32_t hashString(const char* key, int length) {
    uint32_t hash = 2166136261u;
    for (int i = 0; i < length; i++) {
        hash ^= (uint8_t)key[i];
        hash *= 16777619;
    }
    return hash;
}

// Claims ownership of the string you gave it.
ObjString* takeString(char* chars, int length) {
    uint32_t hash = hashString(chars, length);

    ObjString* interned = tableFindString(&vm.strings, chars, length, hash);
    if (interned != NULL) {
        FREE_ARRAY(char, chars, length + 1);
        return interned;
    }

    return allocateString(chars, length, hash);
}

// Allocates space for a new string and copies it, does not take ownership of the generated string. 
ObjString* copyString(const char* chars, int length) {
    uint32_t hash = hashString(chars, length);
    
    ObjString* interned = tableFindString(&vm.strings, chars, length, hash);
    if (interned != NULL) return interned;

    char* heapChars = ALLOCATE(char, length + 1);
    memcpy(heapChars, chars, length);
    heapChars[length] = '\0';
    return allocateString(heapChars, length, hash);
}

// Prints a function as an object
static void printFunction(ObjFunction* function) {
    // Handles the VM's functions
    if (function->name == NULL) {
        printf("<script>");
        return;
    }
    printf("<fn %s>", function->name->string);
}

// Prints an object differently depending on what type of object it is.
void printObject(Value value) {
  switch (OBJ_TYPE(value)) {
    case FUNCTION_OBJ:
        printFunction(AS_FUNCTION(value));
        break;
    case NATIVE_OBJ:
        printf("<native fn>");
        break;
    case STRING_OBJ:
        printf("%s", AS_CSTRING(value));
        break;
  }
}