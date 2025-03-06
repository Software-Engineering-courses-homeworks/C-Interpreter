#include <stdlib.h>
#include "memory.h"
#include "object.h"
#include "value.h"
#include "vm.h"

/// @brief reallocates memory dynamically using the info given by the macros calling it and realloc
/// @param pointer a pointer to the dynamically allocated array
/// @param oldSize the old size of the array that's used to determine what the new size for the allocation should be
/// @param newSize the new allocation size
/// @return returns an abstract pointer to the reallocated array
void *reallocate(void *pointer, size_t oldSize, size_t newSize)
{
    //if the new size is zero, then free the memory
    if (newSize == 0)
    {
        free(pointer);
        return NULL;
    }

    //use the realloc function to change the memory allocated to the block
    void *result = realloc(pointer, newSize);
    if (result == NULL) exit(1);
    return result;
}

/// a helper function to free allocated objects
/// @param object the object that needs to be freed
static void freeObject(Obj *object)
{
    switch (object->type)
    {
        case OBJ_STRING:
        {
            ObjString *string = (ObjString *) object;
            FREE_ARRAY(char, string->chars, string->length + 1);
            FREE(ObjString, object);
            break;
        }
        case OBJ_FUNCTION:
        {
            ObjFunction *function = (ObjFunction *) object;
            freeChunk(&function->chunk);
            FREE(ObjFunction, object);
            break;
        }
        case OBJ_NATIVE:
        {
            FREE(OBJ_NATIVE, object);
            break;
        }
        case OBJ_CLOSURE:
        {
            FREE(ObjClosure, object);
            break;
        }
        case OBJ_UPVALUE:
        {
            ObjClosure* closure = (ObjClosure*)object;
            FREE_ARRAY(ObjUpvalue*,closure->upvalues,closure->upvalueCount);
            FREE(ObjUpvalue, object);
            break;
        }
    }
}

/// the function goes over the object list and frees all of them at the end of the program
void freeObjects()
{
    Obj *object = vm.objects;
    while (object != NULL)
    {
        Obj *next = object->next;
        freeObject(object);
        object = next;
    }
}