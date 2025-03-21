#include <stdio.h>

#include "memory.h"
#include "value.h"
#include <string.h>
#include "object.h"

/// @brief the function initializes the array of values
/// @param array the array that needs to be initialized
void initValueArray(ValueArray* array)
{
    array->values = NULL;
    array->capacity = 0;
    array->count = 0;
}

/// @brief The functions writes a new element into the array
/// @param array the array that needs to be appended
/// @param value the value that needs to be appended
void writeValueArray(ValueArray* array, Value value)
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
/// @param array the array that needs to be freed
void freeValueArray(ValueArray* array)
{
    FREE_ARRAY(Value, array->values, array->capacity);
    initValueArray(array);
}

/// @brief prints the value
/// @param value the value that needs to be printed
void printValue(Value value)
{
    #ifdef NAN_BOXING
    if (IS_BOOL(value))
    {
        printf(AS_BOOL(value) ? "true" : "false");
    }
    else if (IS_NIL(value))
    {
        printf("nil");
    }
    else if (IS_NUMBER(value))
    {
        printf("%g", AS_NUMBER(value));
    }
    else if (IS_OBJ(value))
    {
        printObject(value);
    }
    #else
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
    #endif
}

/// the function checks if the two values that were received are the same
/// @param a    the first value
/// @param b    the second value
/// @return     true - the values are the same. false - not the same
bool valuesEqual(Value a, Value b)
{
    #ifdef NAN_BOXING
    //checks if the values are both numbers and if so, compares them as numbers
    if (IS_NUMBER(a) && IS_NUMBER(b))
    {
        return AS_NUMBER(a) == AS_NUMBER(b);
    }
    return a==b;
    #else
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
#endif
}