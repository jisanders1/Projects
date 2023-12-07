#include <stdio.h>
#include <string.h>
#include "../Headers/memory.h"
#include "../Headers/value.h"
#include "../Headers/object.h"

// Initializes the empty state.
void initValueArray(ValueArray* array) {
    array->values = NULL;
    array->cap = 0;
    array->size = 0;
}

// Adds a value to the values array, increasing with size if needed.
void addValueArray(ValueArray* array, Value value) {
    if (array->cap < array->size + 1) {
        int oldCap = array->cap;
        array->cap = DOUBLE_CAPACITY(oldCap);
        array->values = DOUBLE_ARRAY(Value, array->values, oldCap, array->cap);
    }

    array->values[array->size] = value;
    array->size++;
}

// Represents the initial state of the program.
void freeValueArray(ValueArray* array) {
    FREE_ARRAY(Value, array->values, array->cap);
    initValueArray(array);
}

// Prints a value. Also handles different types.
void printValue(Value value) {
    switch (value.type) {
        case BOOL_VAL:
            printf(AS_BOOL(value) ? "true" : "false");
            break;
        case NIL_VAL: printf("nil"); break;
        case NUMBER_VAL: printf("%g", AS_NUMBER(value)); break;
        case OBJ_VAL: printObject(value); break;
  }
}

// Checks if two values are equal. Returns true if they are equal; otherwise, false.
bool areValuesEqual(Value a, Value b) {
    if (a.type != b.type) return false;
    switch (a.type) {
        case BOOL_VAL:   return AS_BOOL(a) == AS_BOOL(b);
        case NIL_VAL:    return true;
        case NUMBER_VAL: return AS_NUMBER(a) == AS_NUMBER(b);
        case OBJ_VAL: {
            ObjString* aString = AS_STRING(a);
            ObjString* bString = AS_STRING(b);
            return aString->size == bString->size && memcmp(aString->string, bString->string, aString->size) == 0;
        }
        default:         return false; // Unreachable.
    }
}