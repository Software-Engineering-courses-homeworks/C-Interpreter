#ifndef VALUE_H
#define VALUE_H

#include <stdio.h>

#include "common.h"

typedef enum {
    VAL_BOOL,
    VAL_NIL,
    VAL_NUMBER,
} ValueType;

typedef struct {
    ValueType type;
    union {
        bool boolean;
        double number;
    } as;
} Value;

typedef struct {
    int capacity;
    int count;
    Value* values;
} ValueArray;

///initializes a wrapper ValueArray for the interpreter
/// @param array
void initValueArray(ValueArray* array);

///frees the values in the ValueArray
/// @param array
void freeValueArray(ValueArray* array);

/// adds a value to a ValueArray
/// @param array
/// @param value
void writeValueArray(ValueArray* array, Value value);

/// @brief prints the value
/// @param value 
void printValue(Value value);

#endif //VALUE_H
