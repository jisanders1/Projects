#include <stdio.h>
#include "../Headers/memory.h"
#include "../Headers/value.h"

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

// Prints a value.
void printValue(Value value) {
    printf("%g", value);
}