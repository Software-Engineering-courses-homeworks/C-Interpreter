#ifndef TABLE_H
#define TABLE_H

#include "common.h"
#include "value.h"

typedef struct {
    ObjString* key;
    Value value;
}Entry;

typedef struct {
    int count;
    int capacity;
    Entry* entries;
}Table;

/// initializes the hash table
/// @param table the VM's hash table
void initTable(Table* table);

/// frees the given hash table
/// @param table the VM's hash table
void freeTable(Table* table);

/// the function gets a table, key and value and sets the value pointer value as the saved one in the key
/// @param table the hash table
/// @param key the key
/// @param value the return value
/// @return true if the key ahs been found, false otherwise
bool tableGet(Table* table, ObjString* key, Value* value);

/// the function gets a key/value pair and adds it to the hash table
/// @param table the VM hash table
/// @param key the key string
/// @param value the hash value
/// @return true if the new entry was added, false otherwise
bool tableSet(Table* table, ObjString* key, Value value);

/// the function gets a table and a key and deletes it form the table
/// @param table the hash table
/// @param key the key that needs to be removed
/// @return true if the deletion was successful, false otherwise
bool tableDelete(Table* table, ObjString* key);

/// transfers values from one hash table to another
/// @param from the origin table
/// @param to the destination table
void tableAddAll(Table* from, Table* to);

/// The function checks if the string that was recieved exists in the hash table
/// @param table The table we seek in
/// @param chars the string we seek
/// @param length the length of the string
/// @param hash the hash function
/// @return a pointer to the string if it exists in the table
ObjString* tableFindString(Table* table, const char* chars, int length, uint32_t hash);

#endif //TABLE_H