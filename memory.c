#include <stdlib.h>

#include "memory.h"

/// @brief reallocates memory dynamically using the info given by the macros calling it and realloc
/// @param pointer a pointer to the dynamically allocated array
/// @param oldSize the old size of the array that's used to determine what the new size for the allocation should be
/// @param newSize the new allocation size
/// @return returns an abstract pointer to the reallocated array
void* reallocate(void* pointer, size_t oldSize, size_t newSize)
{
    //if the new size is zero, then free the memory
    if(newSize == 0)
    {
        free(pointer);
        return NULL;
    }

    //use the realloc function to change the memory allocated to the block
    void* result = realloc(pointer, newSize);
    if (result == NULL) exit(1);
    return result;
}