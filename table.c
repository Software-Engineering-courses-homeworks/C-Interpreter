#include <stdlib.h>
#include <string.h>
#include "memory.h"
#include "table.h"
#include "object.h"
#include "value.h"

#define TABLE_MAX_LOAD 0.75

/// initializes the hash table
/// @param table the VM's hash table
void initTable(Table *table) {
    table->count = 0;
    table->capacity = 0;
    table->entries = NULL;
}

/// frees the given hash table
/// @param table the VM's hash table
void freeTable(Table *table) {
    FREE_ARRAY(Entry, table->entries, table->capacity);
    initTable(table);
}

/// gets the entry array from the hash table in addition to its capacity and a key and searches for the entry if it exists
/// @param entries the entry array
/// @param capacity the table's capacity
/// @param key the key we want to find
/// @return a pointer to the entry in the array that fits the given key
static Entry* findEntry(Entry* entries, int capacity, ObjString *key) {
    uint32_t index = key->hash % capacity;
    Entry* tombstone = NULL;

    for(;;) {
        Entry* entry = &entries[index];
        //if the spot at the hash index is free or equal to the given key, return the index.
        //else, move to the next spot in the table
        if (entry->key == NULL) {
            if (IS_NIL(entry->value)) {
                // Empty entry.
                return tombstone != NULL ? tombstone : entry;
            }
            else {
                // We found a tombstone.
                if (tombstone == NULL) tombstone = entry;
            }
        }
        else if (entry->key == key) {
            // We found the key.
            return entry;
        }

        index = (index + 1) % capacity;
    }
}

/// the function gets a table, key and value and sets the value pointer value as the saved one in the key
/// @param table the hash table
/// @param key the key
/// @param value the return value
/// @return true if the key ahs been found, false otherwise
bool tableGet(Table* table, ObjString* key, Value* value) {
    //if the table isn't initialized, return false
    if (table->count == 0) return false;

    //search for the entry and save it
    Entry* entry = findEntry(table->entries, table->capacity, key);
    //if the entry is empty return false
    if (entry->key == NULL) return false;
    //otherwise, set teh return value to the entry's value and return true
    *value = entry->value;
    return true;
}

static void adjustCapacity(Table *table, int capacity) {
    //allocates memory for the entry array
    Entry* entries = ALLOCATE(Entry, capacity);

    //initializes the new entry array
    for(int i = 0; i < capacity; i++) {
        entries[i].key = NULL;
        entries[i].value = NIL_VAL;
    }

    //transfers the old hash values to the new entry array and resets the count
    table->count = 0;
    for (int i = 0; i < table->capacity; i++) {
        Entry* entry = &table->entries[i];
        if (entry->key == NULL) continue;

        Entry* dest = findEntry(entries, capacity, entry->key);
        dest->key = entry->key;
        dest->value = entry->value;
        table->count++;
    }

    //frees the old entry array
    FREE_ARRAY(Entry, table->entries, table->capacity);

    //sets the new entry array as the hash array
    table->entries = entries;
    table->capacity = capacity;
}

/// the function gets a key/value pair and adds it to the hash table
/// @param table the VM hash table
/// @param key the key string
/// @param value the hash value
/// @return true if the new entry was added, false otherwise
bool tableSet(Table* table, ObjString* key, Value value) {
    if(table->count + 1 > table->capacity * TABLE_MAX_LOAD) {
        int capacity = GROW_CAPACITY(table->capacity);
        adjustCapacity(table, capacity);
    }

    //searches for the entry if it exists
    Entry* entry = findEntry(table->entries, table->capacity, key);

    //if the entry doesn't exist, flag it as a new entry and increase the table count by 1.
    bool inNewKey = entry->key == NULL;
    if (inNewKey && IS_NIL(entry->value)) table->count++;

    //add the entry to the table (or override the old version)
    entry->key = key;
    entry->value = value;
    return inNewKey;
}

/// the function gets a table and a key and deletes it form the table
/// @param table the hash table
/// @param key the key that needs to be removed
/// @return true if the deletion was successful, false otherwise
bool tableDelete(Table* table, ObjString* key) {
    if (table->count == 0) return false;

    // Find the entry.
    Entry* entry = findEntry(table->entries, table->capacity, key);
    if (entry->key == NULL) return false;

    // Place a tombstone in the entry.
    entry->key = NULL;
    entry->value = BOOL_VAL(true);
    return true;
}

/// transfers values from one hash table to another
/// @param from the origin table
/// @param to the destination table
void tableAddAll(Table* from, Table* to) {
    for (int i = 0; i < from->capacity; i++) {
        Entry* entry = &from->entries[i];
        if (entry->key != NULL) {
            tableSet(to, entry->key, entry->value);
        }
    }
}

/// The function checks if the string that was recieved exists in the hash table
/// @param table the table we seek in
/// @param chars the string we seek
/// @param length the string's length
/// @param hash hash function
/// @return a pointer to the string if it exists in the table
ObjString* tableFindString(Table* table, const char* chars, int length, uint32_t hash) {
    if (table->count == 0) return NULL;

    uint32_t index = hash % table->capacity;
    for(;;) {
        Entry* entry = &table->entries[index];
        if (entry->key == NULL) {
            // Stop if we find an empty non-tombstone entry.
            if (IS_NIL(entry->value)) return NULL;
        }
        else if (entry->key->length == length && entry->key->hash==hash &&
            memcmp(entry->key->chars, chars, length) == 0) {
            //We found it
            return entry->key;
        }

        index = (index + 1) % table->capacity;
    }
}