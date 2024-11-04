#ifndef VALUE_H
#define VALUE_H

#include <stdio.h>

#include "common.h"

typedef struct Obj Obj;
typedef struct ObjString ObjString;

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

#define IS_BOOL(value)   ((value).type == VAL_BOOL)
#define IS_NIL(value)     ((value).type == VAL_NIL)
#define IS_NUMBER(value) ((value).type == VAL_NUMBER)

#define AS_BOOL(value)   ((value).as.boolean)
#define AS_NUMBER(value) ((value).as.number)

#define BOOL_VAL(value)   ((Value){VAL_BOOL,{.boolean = value}})
#define NIL_VAL           ((Value){VAL_NIL, {.number = 0}})
#define NUMBER_VAL(value) ((Value){VAL_NUMBER,{.number = value}})

typedef struct {
    int capacity;
    int count;
    Value* values;
} ValueArray;

///returns if the two values are the same
bool valuesEqual(Value a, Value b);

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
