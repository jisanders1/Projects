#include <stdio.h>
#include <string.h>
#include "../Headers/memory.h"
#include "../Headers/object.h"
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

// Allocates a string object
static ObjString* allocateString(char* chars, int length) {
    ObjString* string = ALLOCATE_OBJ(ObjString, STRING_OBJ);
    string->size = length;
    string->string = chars;
    return string;
}

// Claims ownership of the string you gave it.
ObjString* takeString(char* chars, int length) {
    return allocateString(chars, length);
}

// Allocates space for a new string and copies 
ObjString* copyString(const char* chars, int length) {
    char* heapChars = ALLOCATE(char, length + 1);
    memcpy(heapChars, chars, length);
    heapChars[length] = '\0';
    return allocateString(heapChars, length);
}

// Prints an object differently depending on what type of object it is.
void printObject(Value value) {
  switch (OBJ_TYPE(value)) {
    case STRING_OBJ:
        printf("%s", AS_CSTRING(value));
        break;
  }
}