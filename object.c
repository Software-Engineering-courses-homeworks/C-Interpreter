#include <stdio.h>
#include <string.h>

#include "memory.h"
#include "object.h"
#include "value.h"
#include "vm.h"

#define ALLOCATE_OBJ(type, objectType) \
    (type*)allocateObject(sizeof(type), objectType)

/// The function allocated memory for a new object and initializes its type field
/// @param size
/// @param type
/// @return a new object
static Obj* allocateObject(size_t size, ObjType type) {
    Obj* object = (Obj*)reallocate(NULL,0,size);
    object->type = type;
    object->next = vm.objects;
    vm.objects = object;
    return object;
}

/// the function allocates memory and sets a new string
/// @param chars
/// @param length
/// @return a new string
static Obj* allocateString(char* chars, int length, uint32_t hash) {
    ObjString* string = ALLOCATE_OBJ(ObjString, OBJ_STRING);
    string -> length = length;
    string -> chars = chars;
    string -> hash = hash;
    return string;
}

/// hashes a given string using the FNV-1a hash function
/// @param key the string key
/// @param length the string key length
/// @return a uint32_t hash key
static uint32_t hashString(char* key, int length) {
    //the hash function's bse prime
    uint32_t hash = 2166136261u;

    //hashes the string by going over each character in the key
    for (int i = 0; i < length; i++) {
        hash ^= (uint8_t)key[i];
        hash *= 16777619;
    }

    return hash;
}

/// The function receives an array of characters and a length, and returns a new string from the array
/// @param chars
/// @param length
/// @return a new string
ObjString* takeString(char* chars, int length) {
    uint32_t hash = hashString(chars, length);
    return allocateString(chars, length, hash);
}

/// The function copies a string from a given character array into a new allocated memory location
/// @param chars
/// @param length
/// @return a new string
ObjString* copyString(const char* chars, int length) {
    // hashes the new string before caching
    uint32_t hash = hashString(chars, length);

    //allocate memory on the heap for  the new string
    char* heapChars = ALLOCATE(char, length + 1);

    //copies the string contents to the newly allocated memory
    memcpy(heapChars, chars, length);

    //adds a null terminator at the end
    heapChars[length] = '\0';

    return allocateString(heapChars, length, hash);
}

/// prints the given Obj value according to it's type
/// @param value an Obj value pointer from the stack
void printObject(Value value) {
    switch (OBJ_TYPE(value)) {
        case OBJ_STRING: printf("%s", AS_STRING(value)); break;
    }
}