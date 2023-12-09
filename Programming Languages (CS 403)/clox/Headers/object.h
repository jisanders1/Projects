#ifndef clox_object_h
#define clox_object_h

#include "common.h"
#include "chunk.h"
#include "value.h"

// Returns an objects type
#define OBJ_TYPE(value)        (AS_OBJ(value)->type)

// 3 lines below checks if an object matches a type
#define IS_FUNCTION(value)     isObjType(value, FUNCTION_OBJ)
#define IS_NATIVE(value)       isObjType(value, NATIVE_OBJ)
#define IS_STRING(value)       isObjType(value, STRING_OBJ)

// Typecasts an object as a certain type depending on macro call.
#define AS_FUNCTION(value)     ((ObjFunction*)AS_OBJ(value))
#define AS_NATIVE(value)       (((ObjNative*)AS_OBJ(value))->function)
#define AS_STRING(value)       ((ObjString*)AS_OBJ(value))
#define AS_CSTRING(value)      (((ObjString*)AS_OBJ(value))->string)

// Represents the different types of objects
typedef enum {
    FUNCTION_OBJ,
    NATIVE_OBJ,
    STRING_OBJ
} ObjType;

struct Obj {
    ObjType type;
    struct Obj* next;
};

// Represents a function object for Lox
typedef struct {
    Obj obj;
    int arity;
    Chunk chunk;
    ObjString* name;
} ObjFunction;

typedef Value (*NativeFn)(int argCount, Value* args);

// Represents the native functions to Lox.
typedef struct {
    Obj object;
    NativeFn function;
} ObjNative;

// Represents a string object
struct ObjString {
    Obj object;
    int size;
    char* string;
    uint32_t hash;
};

// Object functions
ObjFunction* initNewFunction();
ObjNative* initNewNative(NativeFn function);
ObjString* takeString(char* chars, int length);
ObjString* copyString(const char* chars, int length);
void printObject(Value value);

static inline bool isObjType(Value value, ObjType type) {
    return IS_OBJ(value) && AS_OBJ(value)->type == type;
}

#endif