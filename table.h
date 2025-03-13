#ifndef TABLE_H
#define TABLE_H

#include "common.h"
#include "value.h"

// an entry in the hash table
typedef struct
{
    ObjString* key;
    Value value;
} Entry;

// a hash table
typedef struct
{
    int count;
    int capacity;
    Entry* entries;
} Table;

void initTable(Table* table);

void freeTable(Table* table);

void markTable(Table* table);

void tableRemoveWhite(Table* table);

bool tableGet(Table* table, ObjString* key, Value* value);

bool tableSet(Table* table, ObjString* key, Value value);

bool tableDelete(Table* table, ObjString* key);

void tableAddAll(Table* from, Table* to);

ObjString* tableFindString(Table* table, const char* chars, int length, uint32_t hash);

#endif //TABLE_H