#include <stdio.h>
#include <string.h>

#include "memory.h"
#include "object.h"
#include "value.h"
#include "vm.h"

#define ALLOCATE_OBJ(type, objectType) \
    (type*)allocateObject(sizeof(type), objectType)

/// The function allocated memory for a new object and initalizes its type field
/// @param size
/// @param type
/// @return a new object
static Obj* allocateObject(size_t size, ObjType type) {
    Obj* object = (Obj*)reallocate(NULL,0,size);
    object->type = type;
    return object;
}

/// the function allocates memory and sets a new string
/// @param chars
/// @param length
/// @return a new string
static Obj* allocateString(char* chars, int length) {
    ObjString* string = ALLOCATE_OBJ(ObjString, OBJ_STRING);
    string -> length = length;
    string -> chars = chars;
    return string;
}

/// The function copies a string from a given character array into a new allocated memory location
/// @param chars
/// @param length
/// @return a new string
ObjString* copyString(const char* chars, int length) {

    //allocate memory on the heap for  the new stringa
    char* heapChars = ALLOCATE(char, length + 1);

    //copies the string contents to the newly allocated memory
    memcpy(heapChars, chars, length);

    //adds a null terminator at the end
    heapChars[length] = '\0';

    return allocateString(heapChars,length);
}

/// prints the given Obj value according to it's type
/// @param value an Obj value pointer from the stack
void printObject(Value value) {
    switch (OBJ_TYPE(value)) {
        case OBJ_STRING: printf("%s", AS_STRING(value)); break;
    }
}