#ifndef clox_value_h
#define clox_value_h

#include "common.h"

// Represents the different type of values
typedef enum {
    BOOL_VAL,
    NIL_VAL, 
    NUMBER_VAL,
} ValueType;

// Represents a wrapper for combing storage for a number and boolean
typedef struct {
    ValueType type;
    union {
        bool boolean;
        double number;
    } as; 
} Value;

// Following 3 lines check if a value's type makes it apropriate to access.
#define IS_BOOL(value)    ((value).type == BOOL_VAL)
#define IS_NIL(value)     ((value).type == NIL_VAL)
#define IS_NUMBER(value)  ((value).type == NUMBER_VAL)

// Following 2 lines converts from a Value type to a C value
#define AS_BOOL(value)    ((value).as.boolean)
#define AS_NUMBER(value)  ((value).as.number)

// Following 3 lines converts from a C value to the Value with the right type tag and underlying value
#define BOOL_V(value)   ((Value){BOOL_VAL, {.boolean = value}})
#define NIL_V           ((Value){NIL_VAL, {.number = 0}})
#define NUMBER_V(value) ((Value){NUMBER_VAL, {.number = value}})

typedef struct {
    int cap;
    int size;
    Value* values;
} ValueArray;

// Functions for values
bool areValuesEqual(Value a, Value b);
void initValueArray(ValueArray* array);
void addValueArray(ValueArray* array, Value value);
void freeValueArray(ValueArray* array);
void printValue(Value value);

#endif