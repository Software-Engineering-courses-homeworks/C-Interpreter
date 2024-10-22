#include <stdio.h>

#include "memory.h"
#include "value.h"

/// @brief the function initializes the array of values
/// @param array 
void initValueArray(ValueArray* array){
    array->values = NULL;
    array->capacity = 0;
    array->count = 0;
}

/// @brief The functions writes a new element into the array
/// @param array 
/// @param value 
void writeValueArray(ValueArray* array, Value value)
{
    //checks if the array needs to be bigger inorder to add a new element
    if (array->capacity < array->count+1)
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
void freeValueArray(ValueArray* array)
{
    FREE_ARRAY(Value, array->values, array->capacity);
    initValueArray(array);
}

/// @brief prints the value
/// @param value 
void printValue(Value value)
{
    printf("%g",value);
}