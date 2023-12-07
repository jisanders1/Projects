#ifndef clox_object_h
#define clox_object_h

#include "common.h"
#include "value.h"

#define OBJ_TYPE(value)        (AS_OBJ(value)->type)

// Checks if an object is a string
#define IS_STRING(value)       isObjType(value, STRING_OBJ)

#define AS_STRING(value)       ((ObjString*)AS_OBJ(value))
#define AS_CSTRING(value)      (((ObjString*)AS_OBJ(value))->string)

// Represents the different types of objects
typedef enum {
    STRING_OBJ,
} ObjType;

struct Obj {
    ObjType type;
    struct Obj* next;
};

// Represents a string object
struct ObjString {
    Obj object;
    int size;
    char* string;
};

// Object functions
ObjString* takeString(char* chars, int length);
ObjString* copyString(const char* chars, int length);
void printObject(Value value);

static inline bool isObjType(Value value, ObjType type) {
    return IS_OBJ(value) && AS_OBJ(value)->type == type;
}

#endif