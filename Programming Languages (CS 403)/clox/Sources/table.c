#include <stdlib.h>
#include <string.h>
#include "../Headers/memory.h"
#include "../Headers/object.h"
#include "../Headers/table.h"
#include "../Headers/value.h"
#define TABLE_MAX_LOAD 0.75

// Constructor-like function that initializes an empty table.
void initTable(Table* table) {
    table->size = 0;
    table->cap = 0;
    table->entries = NULL;
}

// Destructor-like function that frees all the table's memory and starts it anew.
void freeTable(Table* table) {
    FREE_ARRAY(Entry, table->entries, table->cap);
    initTable(table);
}

// Works to find an entry in a hash table's entry list and returns a pointer to it.
// Can be used for adding a new entry or updating an existing one.
static Entry* findEntry(Entry* entries, int capacity, ObjString* key) {
    uint32_t index = key->hash % capacity;
    Entry* tombstone = NULL;
    while (true) {
        Entry* entry = &entries[index];
        if (entry->key == NULL) {
            if (IS_NIL(entry->value)) {// Empty entry.
                // returns the tombstone if one was found before the empty bucket, 
                // otherwise if there are no tombstones we return the empty bucket.
                return tombstone != NULL ? tombstone : entry;
            } else { // We found a tombstone.
                
                if (tombstone == NULL) tombstone = entry; // Updates tombstone entry to be the first tombstone
            }
        } 
        else if (entry->key == key) { // We found the key.
            
            return entry;
        }

        index = (index + 1) % capacity; // performs linear probing in the case of collisions
    }
}

// Retrieves a value from the table if the key is present, points the value parameter towards it, and returns true.
// If the table is empty or the key is not in the table, it returns false and does not set a pointer.
bool getValue(Table* table, ObjString* key, Value* value) {
    if (table->size == 0) return false;

    Entry* entry = findEntry(table->entries, table->cap, key);
    if (entry->key == NULL) return false;

    *value = entry->value;
    return true;
}

// Handles the resizing of the hash table by allocating new space and re-hashing all the existing values in the table.
static void adjustCapacity(Table* table, int capacity) {
    Entry* entries = ALLOCATE(Entry, capacity);
    for (int i = 0; i < capacity; i++) {
        entries[i].key = NULL;
        entries[i].value = NIL_V;
    }

    // Re-inserting hash table values
    table->size = 0;
    for (int i = 0; i < table->cap; i++) {
        Entry* entry = &table->entries[i];
        if (entry->key == NULL) continue;

        Entry* dest = findEntry(entries, capacity, entry->key);
        dest->key = entry->key;
        dest->value = entry->value;
        table->size++;
    }

    // Resetting points and capacity value and freeing old entry array
    FREE_ARRAY(Entry, table->entries, table->cap);
    table->entries = entries;
    table->cap = capacity;
}

// Inserts a key-value pair into the hash table, overwriting an old value if the key is already present.
bool insertPair(Table* table, ObjString* key, Value value) {
    if (table->size + 1 > table->cap * TABLE_MAX_LOAD) {
        int capacity = DOUBLE_CAPACITY(table->cap);
        adjustCapacity(table, capacity);
    }
    Entry* entry = findEntry(table->entries, table->cap, key);
    bool isNewKey = entry->key == NULL;
    if (isNewKey && IS_NIL(entry->value)) table->size++; //counts tombstones and new keys as full buckets

    entry->key = key;
    entry->value = value;
    return isNewKey;
}

// Deletes a pair in a table if the pair is found in the table and returns true.
// If the table is empty or if teh key is not found, it returns false
// The pair is deleted by overwriting it's value with a tombstone.
bool deletePair(Table* table, ObjString* key) {
    if (table->size == 0) return false;

    // Find the entry.
    Entry* entry = findEntry(table->entries, table->cap, key);
    if (entry->key == NULL) return false;

    // Place a tombstone in the entry.
    entry->key = NULL;
    entry->value = BOOL_V(true);
    return true;
}

// Copies all entries of the from hash table into the to hash table.
void tableAddAll(Table* from, Table* to) {
    for (int i = 0; i < from->cap; i++) {
        Entry* entry = &from->entries[i];
        if (entry->key != NULL) {
            insertPair(to, entry->key, entry->value);
        }
    }
}

// matches raw strings using hash codes
ObjString* tableFindString(Table* table, const char* chars, int length, uint32_t hash) {
    if (table->size == 0) return NULL;

    uint32_t index = hash % table->cap;
    while (true) {
        Entry* entry = &table->entries[index];
        if (entry->key == NULL) {
            // Stop if we find an empty non-tombstone entry.
            if (IS_NIL(entry->value)) return NULL;
        } 
        else if (entry->key->size == length && entry->key->hash == hash && memcmp(entry->key->string, chars, length) == 0) {
            // We found it.
            return entry->key;
        }

        index = (index + 1) % table->cap;
    }
}