#ifndef clox_table_h
#define clox_table_h

#include "common.h"
#include "value.h"

typedef struct {
    ObjString* key;
    Value value;
} Entry;

typedef struct {
    int size;
    int cap;
    Entry* entries;
} Table;

// Hash Table Functions
void initTable(Table* table);
void freeTable(Table* table);
bool getValue(Table* table, ObjString* key, Value* value);
bool insertPair(Table* table, ObjString* key, Value value);
bool deletePair(Table* table, ObjString* key);
void tableAddAll(Table* from, Table* to);
ObjString* tableFindString(Table* table, const char* chars, int length, uint32_t hash);

#endif