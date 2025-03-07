#include <stdio.h>

#include "memory.h"
#include "value.h"
#include <string.h>
#include "object.h"

/// @brief the function initializes the array of values
/// @param array 
void initValueArray(ValueArray *array)
{
    array->values = NULL;
    array->capacity = 0;
    array->count = 0;
}

/// @brief The functions writes a new element into the array
/// @param array 
/// @param value 
void writeValueArray(ValueArray *array, Value value)
{
    //checks if the array needs to be bigger inorder to add a new element
    if (array->capacity < array->count + 1)
    {
        int oldCapacity = array->capacity;
        array->capacity = GROW_CAPACITY(oldCapacity);
        array->values = GROW_ARRAY(Value, array->values, oldCapacity, array->capacity);
    }

    array->values[array->count] = value;
    array->count++;
}

/// @brief The function releases all the memory used by the array
/// @param array 
void freeValueArray(ValueArray *array)
{
    FREE_ARRAY(Value, array->values, array->capacity);
    initValueArray(array);
}

/// @brief prints the value
/// @param value 
void printValue(Value value)
{
    switch (value.type)
    {
        case VAL_BOOL:
            printf(AS_BOOL(value) ? "true" : "false");
            break;
        case VAL_NIL:
            printf("nil");
            break;
        case VAL_NUMBER:
            printf("%g", AS_NUMBER(value));
            break;
        case VAL_OBJ:
            printObject(value);
            break;
    }
}

/// the function checks if the two values that were received are the same
/// @param a
/// @param b
/// @return true - the values are the same. false - not the same
bool valuesEqual(Value a, Value b)
{
    //if the values don't have the same type, so return false
    if (a.type != b.type) return false;
    switch (a.type)
    {
        case VAL_BOOL:
            return AS_BOOL(a) == AS_BOOL(b);
        case VAL_NIL:
            return true;
        case VAL_NUMBER:
            return AS_NUMBER(a) == AS_NUMBER(b);
        case VAL_OBJ:
            return AS_OBJ(a) == AS_OBJ(b);
        default:
            return false; //unreachable
    }
}