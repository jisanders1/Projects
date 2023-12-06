#ifndef clox_value_h
#define clox_value_h

#include "common.h"

typedef double Value;

typedef struct {
    int cap;
    int size;
    Value* values;
} ValueArray;

// Functions for values
void initValueArray(ValueArray* array);
void addValueArray(ValueArray* array, Value value);
void freeValueArray(ValueArray* array);
void printValue(Value value);

#endif